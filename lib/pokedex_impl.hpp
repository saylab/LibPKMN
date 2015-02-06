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

            pokemon_entry_t get_pokemon_entry(uint16_t species_id,
                                              uint16_t form_id = 0);

            pokemon_entry_t get_pokemon_entry(const pkmn::pkstring& species_name,
                                              const pkmn::pkstring& form_name = "");

            move_entry_t get_move_entry(uint16_t move_id);

            move_entry_t get_move_entry(const pkmn::pkstring& move_name);

            item_entry_t get_item_entry(uint16_t item_id);

            item_entry_t get_item_entry(const pkmn::pkstring& item_name);

            uint16_t get_game_id() const;

        private:

            uint16_t _version_id, _generation, _version_group_id;

            static pkmn::dict<uint16_t, pkmn::dict<uint16_t, pokemon_entry_t> > _pokemon_entry_cache;
            static pkmn::dict<uint16_t, pkmn::dict<uint16_t, move_entry_t> >    _move_entry_cache;
            static pkmn::dict<uint16_t, pkmn::dict<uint16_t, item_entry_t> >    _item_entry_cache;
            static pkmn::shared_ptr<SQLite::Database> _db;
    };
}

#endif /* INCLUDED_POKEDEX_IMPL_HPP */