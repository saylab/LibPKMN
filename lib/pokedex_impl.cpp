/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <sstream>

#include <boost/format.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/database.hpp>
#include <pkmn/paths.hpp>

#include "pokedex_impl.hpp"

namespace pkmn
{
    pokedex::sptr pokedex::make(const pkmn::pkstring& game)
    {
        return sptr(new pokedex_impl(game));
    }

    pokedex_impl::pokedex_impl(const pkmn::pkstring& game):
        pokedex(),
        _version_id(database::get_version_id(game))
    {
        if(!_db) _db = pkmn::shared_ptr<SQLite::Database>(new SQLite::Database(get_database_path()));

        _generation       = database::get_generation(_version_id);
        _version_group_id = database::get_version_group_id(_version_id);
    }

    pkmn::dict<uint16_t, pkmn::dict<uint16_t, pokemon_entry_t> > pokedex_impl::_pokemon_entry_cache;
    pkmn::dict<uint16_t, pkmn::dict<uint16_t, move_entry_t> >    pokedex_impl::_move_entry_cache;
    pkmn::dict<uint16_t, pkmn::dict<uint16_t, item_entry_t> >    pokedex_impl::_item_entry_cache;
    pkmn::shared_ptr<SQLite::Database>                           pokedex_impl::_db;

    pkmn::pkstring pokedex_impl::get_game() const
    {
        return database::get_version_name(_version_id);
    }

    uint16_t pokedex_impl::get_generation() const
    {
        return database::get_generation(_version_id);
    }

    pokemon_entry_t pokedex_impl::get_pokemon_entry(const uint16_t species_id,
                                                    const uint16_t form_id)
    {
        uint16_t pokemon_id;
        // Use default form if nothing given
        if(form_id == 0 or form_id == species_id) pokemon_id = species_id;
        else
        {
            std::ostringstream query_stream;
            query_stream << "SELECT pokemon_id,introduced_in_version_group_id FROM pokemon_forms WHERE id="
                         << form_id;
            SQLite::Statement query(*_db, query_stream.str().c_str());
            if(query.executeStep())
            {
                if(database::get_version_group_id(_version_id) < int(query.getColumn(1)))
                    throw std::runtime_error("This form did not exist in this version.");

                pokemon_id = query.getColumn(0);
            }
            else throw std::runtime_error("Invalid form.");
        }

        if(_pokemon_entry_cache[_version_id].has_key(pokemon_id))
            return _pokemon_entry_cache[_version_id][pokemon_id];
        else
        {
            _pokemon_entry_cache[_version_id][pokemon_id] = pokemon_entry_t(_version_id,
                                                                            species_id,
                                                                            form_id);
        }
    }

    pokemon_entry_t pokedex_impl::get_pokemon_entry(const pkmn::pkstring& species_name,
                                                    const pkmn::pkstring& form_name)
    {
        uint16_t form_id;
        if(form_name == "Standard" or form_name == species_name or form_name == "")
            form_id = database::get_species_id(species_name);
        else
            form_id = database::get_form_id(species_name, form_name);

        return get_pokemon_entry(database::get_species_id(species_name), form_id);
    }

    move_entry_t pokedex_impl::get_move_entry(const uint16_t move_id)
    {
        std::ostringstream query_stream;
        query_stream << "SELECT generation_id FROM moves WHERE id=" << move_id;
        if(uint16_t(_db->execAndGet(query_stream.str().c_str())) > _generation)
            throw std::runtime_error("This move did not exist in this generation.");

        if(_move_entry_cache[_version_id].has_key(move_id))
            return _move_entry_cache[_version_id][move_id];
        else
        {
            _move_entry_cache[_version_id][move_id] = move_entry_t(_version_id,
                                                                   move_id);
        }
    }

    move_entry_t pokedex_impl::get_move_entry(const pkmn::pkstring& move_name)
    {
        return get_move_entry(database::get_move_id(move_name));
    }

    item_entry_t pokedex_impl::get_item_entry(const uint16_t item_id)
    {
        std::ostringstream query_stream;
        query_stream << "SELECT generation_id FROM item_game_indices WHERE item_id="
                     << item_id;
        SQLite::Statement query(*_db, query_stream.str().c_str());
        if(not query.executeStep())
            throw std::runtime_error("This item did not exist in this game.");

        if(_item_entry_cache[_version_id].has_key(item_id))
            return _item_entry_cache[_version_id][item_id];
        else
        {
            _item_entry_cache[_version_id][item_id] = item_entry_t(_version_id,
                                                                   item_id);
        }
    }

    item_entry_t pokedex_impl::get_item_entry(const pkmn::pkstring& item_name)
    {
        return get_item_entry(database::get_item_id(item_name));
    }
} /* namespace pkmn */
