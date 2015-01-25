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

            bag_impl(uint16_t game);
            bag_impl(const bag_impl& other);
            bag_impl& operator=(const bag_impl& other);
            ~bag_impl() {};

            pkmn::pkstring get_game() const;
            uint16_t get_generation() const;

            void add_item(const pkmn::pkstring &item_name, uint16_t amount);
            void add_item(uint16_t item_id, uint16_t amount);
            void remove_item(const pkmn::pkstring &item_name, uint16_t amount);
            void remove_item(uint16_t item_id, uint16_t amount);
            uint16_t get_item_amount(const pkmn::pkstring &item_name) const;
            uint16_t get_item_amount(uint16_t item_id) const;

            pocket::sptr get_pocket(const pkmn::pkstring &name) const;
            pkmn::dict<pkmn::pkstring, pocket::sptr> get_pockets() const;

            uint16_t get_game_id() const;

        private:

            static pkmn::shared_ptr<SQLite::Database> _db;

            uint16_t _game_id, _generation;
            pkmn::dict<pkmn::pkstring, pocket::sptr> _pockets; 

            pkmn::pkstring _get_pocket_name(uint16_t item_id) const;
    };
}

#endif /* INCLUDED_BAG_IMPL_HPP */
