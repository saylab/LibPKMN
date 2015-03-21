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
            int get_generation() const;

            const pokemon_entry_t& get_pokemon_entry(int species_id,
                                                     int form_id = 0);

            const pokemon_entry_t& get_pokemon_entry(const pkmn::pkstring &species_name,
                                                     const pkmn::pkstring &form_name = "");

            const move_entry_t& get_move_entry(int move_id);

            const move_entry_t& get_move_entry(const pkmn::pkstring &move_name);

            const item_entry_t& get_item_entry(int item_id);

            const item_entry_t& get_item_entry(const pkmn::pkstring &item_name);

            int get_game_id() const;

        private:

            int _version_id, _generation, _version_group_id;

            static pkmn::dict<int, pkmn::dict<int, pokemon_entry_t> > _pokemon_entry_cache;
            static pkmn::dict<int, pkmn::dict<int, move_entry_t> >    _move_entry_cache;
            static pkmn::dict<int, pkmn::dict<int, item_entry_t> >    _item_entry_cache;
            static pkmn::shared_ptr<SQLite::Database>                 _db;
    };
}

#endif /* INCLUDED_POKEDEX_IMPL_HPP */
