/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_BAG_IMPL_HPP
#define INCLUDED_BAG_IMPL_HPP

#include <pkmn/bag.hpp>

#include "SQLiteCpp/SQLiteC++.h"

namespace pkmn
{
    class bag_impl: public bag
    {
        public:

            bag_impl(int game);
            bag_impl(const bag_impl& other);
            bag_impl& operator=(const bag_impl& other);
            ~bag_impl() {};

            pkmn::pkstring get_game() const;
            int get_generation() const;

            void add_item(const pkmn::pkstring &item_name, int amount);
            void add_item(int item_id, int amount);
            void remove_item(const pkmn::pkstring &item_name, int amount);
            void remove_item(int item_id, int amount);
            int get_item_amount(const pkmn::pkstring &item_name) const;
            int get_item_amount(int item_id) const;

            pocket::sptr get_pocket(const pkmn::pkstring &name) const;
            pkmn::dict<pkmn::pkstring, pocket::sptr> get_pockets() const;

            int get_game_id() const;

        private:

            static pkmn::shared_ptr<SQLite::Database> _db;

            int _game_id, _generation;
            pkmn::dict<pkmn::pkstring, pocket::sptr> _pockets; 

            pkmn::pkstring _get_pocket_name(int item_id) const;
    };
}

#endif /* INCLUDED_BAG_IMPL_HPP */
