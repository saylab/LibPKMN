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

            pokedex_entry_t& get_entry(const uint16_t species_id,
                                       const uint16_t form_id = 0);

            pokedex_entry_t& get_entry(const pkmn::pkstring& species_name,
                                       const pkmn::pkstring& form_name = "");

        private:

            uint16_t _version_id;

            static pkmn::dict<uint16_t, pkmn::dict<uint16_t, pokedex_entry_t> > _entry_cache;
            static pkmn::shared_ptr<SQLite::Database> _db;

            void _create_entry(const uint16_t pokemon_id);
            float _get_gender_chance(const uint16_t species_id,
                                     uint8_t gender_rate,
                                     bool male) const;
            void _adjust_entry(pokedex_entry_t& entry);
    };
}

#endif /* INCLUDED_POKEDEX_IMPL_HPP */
