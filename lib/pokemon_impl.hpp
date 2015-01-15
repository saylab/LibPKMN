/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_POKEMON_IMPL_HPP
#define INCLUDED_POKEMON_IMPL_HPP

#include <pkmn/pokemon.hpp>
#include <pkmn/types/prng.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include "SQLiteCpp/SQLiteC++.h"

namespace pkmn
{
    class pokemon_impl: public pokemon
    {
        public:

            pokemon_impl(uint16_t species_id, uint16_t version_id);
            pokemon_impl(const pokemon_impl& other);
            pokemon_impl& operator=(const pokemon_impl& other);
            ~pokemon_impl() {};

            // Getting Non-battle Info
            pkmn::pokemon_entry_t get_pokedex_entry() const;

            // Getting LibPKMN info
            pkmn::pkstring get_game() const;
            uint16_t get_generation() const;
            int get_attribute(const pkmn::pkstring& attribute) const;
            pkmn::dict<pkmn::pkstring, int> get_attributes() const;
            bool has_attribute(const pkmn::pkstring& attribute) const;
			void set_attribute(const pkmn::pkstring& attribute, int value);

            // Database Info
            uint16_t get_species_id() const;
            uint16_t get_game_id() const;

        private:

            static pkmn::shared_ptr<SQLite::Database> _db;

            uint16_t _species_id, _version_id;
            pkmn::dict<pkmn::pkstring, int> _attributes;
            pokedex::sptr _pokedex;
            pkmn::pokemon_entry_t _pokedex_entry;
            prng::sptr    _prng;
    };
}

#endif /* INCLUDED_POKEMON_IMPL_HPP */
