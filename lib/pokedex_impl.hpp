/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_POKEDEX_IMPL_HPP
#define INCLUDED_POKEDEX_IMPL_HPP

#include <pkmn/pokedex.hpp>

#include "SQLiteCpp/SQLiteC++.h"

namespace pkmn
{
    class pokedex_impl: public pokedex
    {
        public:

            pokedex_impl();
            pokedex_impl(const pkmn::pkstring &game);

            //Game-specific Info
            pkmn::pkstring get_game() const;
            uint16_t get_generation() const;

            pokemon_entry_t& get_pokemon_entry(const uint16_t species_id,
                                               const uint16_t form_id = 0);

            pokemon_entry_t& get_pokemon_entry(const pkmn::pkstring& species_name,
                                               const pkmn::pkstring& form_name = "");

            move_entry_t& get_move_entry(const uint16_t move_id);

            move_entry_t& get_move_entry(const pkmn::pkstring& move_name);

        private:

            uint16_t _version_id;

            static pkmn::dict<uint16_t, pkmn::dict<uint16_t, pokemon_entry_t> > _pokemon_entry_cache;
            static pkmn::dict<uint16_t, pkmn::dict<uint16_t, move_entry_t> > _move_entry_cache;
            static pkmn::shared_ptr<SQLite::Database> _db;

            void _create_pokemon_entry(const uint16_t pokemon_id);
            float _get_gender_chance(const uint16_t species_id,
                                     uint8_t gender_rate,
                                     bool male) const;
            void _adjust_pokemon_entry(pokemon_entry_t& entry);

            void _create_move_entry(const uint16_t move_id);
            void _adjust_move_entry(move_entry_t& entry);
    };
}

#endif /* INCLUDED_POKEDEX_IMPL_HPP */
