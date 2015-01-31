/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_POKEMON_IMPL_HPP
#define INCLUDED_POKEMON_IMPL_HPP

#include <boost/assign.hpp>
#include <boost/format.hpp>

#include <pkmn/pokemon.hpp>
#include <pkmn/types/prng.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include "internal.hpp"
#include "SQLiteCpp/SQLiteC++.h"

#define UPPERCASE_SPECIES_NAME(id,ver) PKSTRING_UPPERCASE(database::get_species_name( \
                                                          database::get_pokemon_id(id,ver)))

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

            // Getting Individual Stat Info
            pkmn::pkstring get_form() const;

            // Getting LibPKMN info
            pkmn::pkstring get_game() const;
            uint16_t get_generation() const;
            int get_attribute(const pkmn::pkstring& attribute) const;
            pkmn::dict<pkmn::pkstring, int> get_attributes() const;
            bool has_attribute(const pkmn::pkstring& attribute) const;
			void set_attribute(const pkmn::pkstring& attribute, int value);
            pkmn::pkstring get_icon_path() const;
            pkmn::pkstring get_sprite_path() const;

            // Database Info
            uint16_t get_species_id() const;
            uint16_t get_pokemon_id() const;
            uint16_t get_game_id() const;
            uint16_t get_form_id() const;

        protected:

            static pkmn::shared_ptr<SQLite::Database> _db;
            static pkmn::dict<uint8_t, std::string> _version_dirs;
            static boost::format _generation_format;
            static boost::format _pokemon_format;
            static boost::format _pokemon_form_format;

            uint16_t _species_id, _form_id, _version_id;
            pkmn::dict<pkmn::pkstring, int> _attributes;
            pokedex::sptr _pokedex;
            pkmn::pokemon_entry_t _pokedex_entry;
            prng::sptr    _prng;

    };
}

#endif /* INCLUDED_POKEMON_IMPL_HPP */
