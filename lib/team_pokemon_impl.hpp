/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TEAM_POKEMON_IMPL_HPP
#define INCLUDED_TEAM_POKEMON_IMPL_HPP

#include <pkmn/team_pokemon.hpp>
#include <pkmn/types/prng.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include "SQLiteCpp/SQLiteC++.h"

namespace pkmn
{
    class team_pokemon_impl: public team_pokemon
    {
        public:

            team_pokemon_impl(base_pokemon::sptr base);
            virtual ~team_pokemon_impl() {};

            // Game-specific Info
            pkmn::pkstring get_game() const;
            unsigned int get_generation() const;

            // Non-battle Stats
            base_pokemon::sptr get_base_pokemon(bool copy = false) const;
            pkmn::pkstring get_species_name() const;

            // Getting Individual Stat Info
            virtual pkmn::pkstring get_gender() const = 0;
            virtual bool is_shiny() const = 0;

            // Misc
            int get_attribute(const pkmn::pkstring &attribute) const;
            pkmn::dict<pkmn::pkstring, int> get_attributes() const;
            bool has_attribute(const pkmn::pkstring &attribute) const;
			void set_attribute(const pkmn::pkstring &attribute, int value);
            pkmn::pkstring get_icon_path() const;
            pkmn::pkstring get_sprite_path() const;
            void set_form(const pkmn::pkstring &form);
            void set_form(unsigned int form);

            // Database Info
            unsigned int get_pokemon_id() const;
            unsigned int get_species_id() const;
            unsigned int get_game_id() const;
            unsigned int get_form_id() const;

        protected:

            static pkmn::shared_ptr<SQLite::Database> _db;

            base_pokemon::sptr _base;
            prng::sptr _prng;
            unsigned int _species_id, _game_id;
            pkmn::dict<pkmn::pkstring, int> _attributes;

            virtual void _set_experience(const uint32_t exp) = 0;
            virtual void _set_level(const uint8_t level) = 0;
            virtual void _set_stats() = 0;
    };
}

#endif /* INCLUDED_TEAM_POKEMON_IMPL_HPP */
