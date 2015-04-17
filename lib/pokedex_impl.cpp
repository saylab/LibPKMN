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

#include "internal.hpp"
#include "pokedex_impl.hpp"

namespace pkmn
{
    pokedex::sptr pokedex::make(const pkmn::pkstring &game)
    {
        return sptr(new pokedex_impl(game));
    }

    pokedex_impl::pokedex_impl(const pkmn::pkstring &game):
        pokedex(),
        _version_id(database::get_version_id(game))
    {
        CONNECT_TO_DB(_db);

        _generation       = database::get_generation(_version_id);
        _version_group_id = database::get_version_group_id(_version_id);
    }

    pkmn::dict<int, pkmn::dict<int, pokemon_entry_t> > pokedex_impl::_pokemon_entry_cache;
    pkmn::dict<int, pkmn::dict<int, move_entry_t> >    pokedex_impl::_move_entry_cache;
    pkmn::dict<int, pkmn::dict<int, item_entry_t> >    pokedex_impl::_item_entry_cache;
    pkmn::shared_ptr<SQLite::Database>                 pokedex_impl::_db;

    pkmn::pkstring pokedex_impl::get_game() const
    {
        return database::get_version_name(_version_id);
    }

    int pokedex_impl::get_generation() const
    {
        return database::get_generation(_version_id);
    }

    const pokemon_entry_t& pokedex_impl::get_pokemon_entry(int species_id,
                                                           int form_id)
    {
        if(species_id == Species::NONE or species_id == Species::INVALID)
        {
            _pokemon_entry_cache[_version_id][species_id] = pokemon_entry_t(_version_id,
                                                                            species_id,
                                                                            form_id);
            return _pokemon_entry_cache[_version_id][species_id];
        }

        int pokemon_id;
        if(form_id == 0 or form_id == species_id)
        {
            // Use default form if nothing given
            pokemon_id = species_id;
            form_id = species_id;
        }
        else
        {
            std::ostringstream query_stream;
            query_stream << "SELECT pokemon_id,introduced_in_version_group_id FROM pokemon_forms WHERE id="
                         << form_id;
            SQLite::Statement query(*_db, query_stream.str().c_str());
            if(query.executeStep())
            {
                if(database::get_version_group_id(_version_id) < int(query.getColumn("introduced_in_version_group_id")))
                    throw std::runtime_error("This form did not exist in this version.");

                pokemon_id = query.getColumn("pokemon_id");
            }
            else throw std::runtime_error("Invalid form.");
        }

        if(not _pokemon_entry_cache[_version_id].has_key(pokemon_id))
        {
            _pokemon_entry_cache[_version_id][pokemon_id] = pokemon_entry_t(_version_id,
                                                                            species_id,
                                                                            form_id);
        }
        return _pokemon_entry_cache[_version_id][pokemon_id];
    }

    const pokemon_entry_t& pokedex_impl::get_pokemon_entry(const pkmn::pkstring &species_name,
                                                           const pkmn::pkstring &form_name)
    {
        int form_id;
        if(form_name == "Standard" or form_name == species_name or form_name == "")
            form_id = database::get_species_id(species_name);
        else
            form_id = database::get_form_id(species_name, form_name);

        return get_pokemon_entry(database::get_species_id(species_name), form_id);
    }

    const move_entry_t& pokedex_impl::get_move_entry(int move_id)
    {
        if(move_id == Moves::NONE or move_id == Moves::INVALID)
        {
            _move_entry_cache[_version_id][move_id] = move_entry_t(_version_id,
                                                                   move_id);

            return _move_entry_cache[_version_id][move_id];
        }

        std::ostringstream query_stream;
        query_stream << "SELECT generation_id FROM moves WHERE id=" << move_id;
        if(int(_db->execAndGet(query_stream.str().c_str())) > _generation)
            throw std::runtime_error("This move did not exist in this generation.");

        if(not _move_entry_cache[_version_id].has_key(move_id))
        {
            _move_entry_cache[_version_id][move_id] = move_entry_t(_version_id,
                                                                   move_id);
        }
        return _move_entry_cache[_version_id][move_id];
    }

    const move_entry_t& pokedex_impl::get_move_entry(const pkmn::pkstring &move_name)
    {
        return get_move_entry(database::get_move_id(move_name));
    }

    const item_entry_t& pokedex_impl::get_item_entry(int item_id)
    {
        if(item_id == Items::NONE or item_id == Items::INVALID)
        {
            _item_entry_cache[_version_id][item_id] = item_entry_t(_version_id,
                                                                   item_id);

            return _item_entry_cache[_version_id][item_id];
        }

        std::ostringstream query_stream;
        query_stream << "SELECT generation_id FROM item_game_indices WHERE item_id="
                     << item_id;
        SQLite::Statement query(*_db, query_stream.str().c_str());
        if(not query.executeStep())
            throw std::runtime_error("This item did not exist in this game.");

        if(not _item_entry_cache[_version_id].has_key(item_id))
        {
            _item_entry_cache[_version_id][item_id] = item_entry_t(_version_id,
                                                                   item_id);
        }
        return _item_entry_cache[_version_id][item_id];
    }

    const item_entry_t& pokedex_impl::get_item_entry(const pkmn::pkstring &item_name)
    {
        return get_item_entry(database::get_item_id(item_name));
    }

    int pokedex_impl::get_game_id() const
    {
        return _version_id;
    }
} /* namespace pkmn */
