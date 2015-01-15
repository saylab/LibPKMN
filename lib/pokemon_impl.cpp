/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <sstream>
#include <stdexcept>

#include <boost/format.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/pokemon.hpp>
#include <pkmn/database.hpp>

#include "copy_sptrs.hpp"

#include "pokemon_impl.hpp"
#include "pokemon_gen1impl.hpp"
#include "pokemon_gen2impl.hpp"
#include "pokemon_gen3impl.hpp"
//#include "pokemon_ndsimpl.hpp"

#include "SQLiteCpp/SQLiteC++.h"

namespace pkmn
{
    pokemon::sptr pokemon::make(uint16_t species, uint16_t version, uint16_t level,
                                uint16_t move1,   uint16_t move2,   uint16_t move3,
                                uint16_t move4)
    {
        uint8_t generation = database::get_generation(version);

        // Check if Pokémon is valid and was present in given version
        std::ostringstream query_stream;
        query_stream << "SELECT generation_id FROM pokemon_species WHERE id="
                     << species;
        SQLite::Database db(get_database_path());
        SQLite::Statement query(db, query_stream.str().c_str());
        if(not query.executeStep())
            throw std::runtime_error("Invalid Pokémon species.");
        if(uint8_t(query.getColumn(0)) > generation)
            throw std::runtime_error("This Pokémon was not present in the given version.");

        switch(generation)
        {
            case 1:
                return sptr(new pokemon_gen1impl(species, version, level,
                                                 move1, move2, move3, move4));

            case 2:
                return sptr(new pokemon_gen2impl(species, version, level,
                                                 move1, move2, move3, move4));

            case 3:
                return sptr(new pokemon_gen3impl(species, version, level,
                                                 move1, move2, move3, move4));

            //default:
            //    return sptr(new pokemon_ndsimpl(species, version, level,
            //                                    move1, move2, move3, move4));
        }
    }

    pokemon::sptr pokemon::make(const pkmn::pkstring& name,  const pkmn::pkstring& game, uint16_t level,
                                const pkmn::pkstring& move1, const pkmn::pkstring& move2,
                                const pkmn::pkstring& move3, const pkmn::pkstring& move4)
    {
        return make(database::get_species_id(name),
                    database::get_version_id(game),
                    level,
                    database::get_move_id(move1),
                    database::get_move_id(move2),
                    database::get_move_id(move3),
                    database::get_move_id(move4));
    }

    pkmn::shared_ptr<SQLite::Database> pokemon_impl::_db;

    pokemon_impl::pokemon_impl(uint16_t species_id, uint16_t version_id)
        pokemon(),
        _pokedex(pokedex::make(version_id)),
        _prng(prng::make(database::get_generation(version_id))),
        _species_id(_species_id),
        _version_id(version_id)
    {
        if(!_db) _db = pkmn::shared_ptr<SQLite::Database>(new SQLite::Database(get_database_path()));

        _pokedex_entry = _pokedex->get_pokemon_entry(_species_id);
    }

    pokemon_impl::pokemon_impl(const pokemon_impl& other):
        _pokedex(copy_pokedex(other._pokedex),
        _pokedex_entry(other._pokedex_entry),
        _prng(copy_prng(other._prng)),
        _species_id(other._species_id),
        _version_id(other._version_id),
        _attributes(other._attributes) {};

    pokemon_impl& pokemon_impl::operator=(const pokemon_impl& other)
    {
        _pokedex       = copy_pokedex(other._pokedex);
        _pokedex_entry = other._pokedex_entry;
        _prng          = copy_prng(other._prng);
        _species_id    = other._species_id;
        _version_id    = other._version_id;
        _attributes    = other._attributes;
    }

    /*
     * Getting Non-battle info
     */

    pkmn::pokemon_entry_t pokemon_impl::get_pokedex_entry() const
    {
        return _pokedex->get_pokemon_entry(_species_id, get_form_id());
    }

    /*
     * Getting LibPKMN info
     */

    pkmn::pkstring pokemon_impl::get_game() const
    {
        return database::get_version_name(_version_id);
    }

    uint16_t pokemon_impl::get_generation() const
    {
        return database::get_generation(_version_id);
    }

    int pokemon_impl::get_attribute(const pkmn::pkstring& attribute) const
    {
        return _attributes.at(attribute);
    }

    pkmn::dict<pkmn::pkstring, int> pokemon_impl::get_attributes() const
    {
        return _attributes;
    }

    bool pokemon_impl::has_attribute(const pkmn::pkstring& attribute) const
    {
        return _attributes.has_key(attribute);
    }

    void pokemon_impl::set_attribute(const pkmn::pkstring& attribute, int value)
    {
        _attributes[attribute] = value;
    }

    void pokemon_impl::get_species_id() const
    {
        return _species_id;
    }

    void pokemon_impl::get_game_id() const
    {
        return _version_id;
    }
} /* namespace pkmn */
