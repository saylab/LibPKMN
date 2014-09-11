/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_POCKET_IMPL_HPP
#define INCLUDED_POCKET_IMPL_HPP

#include <cstdint>
#include <vector>

#include <pkmn/pocket.hpp>
#include <pkmn/types/dict.hpp>

namespace pkmn
{
    class pocket_impl: public pocket
    {
        public:

            pocket_impl(unsigned int game, const pkmn::pkstring &name, unsigned int size);

            pkmn::pkstring get_game() const;
            unsigned int get_generation() const;

            pkmn::pkstring get_name() const;
            unsigned int size() const;

            void add_item(const pkmn::pkstring &item_name, unsigned int amount);
            void add_item(unsigned int item_id, unsigned int amount);
            void add_item(item::sptr item_sptr, unsigned int amount);
            void remove_item(const pkmn::pkstring &item_name, unsigned int amount);
            void remove_item(unsigned int item_id, unsigned int amount);
            void remove_item(item::sptr item_sptr, unsigned int amount);
            unsigned int get_item_amount(const pkmn::pkstring &item_name) const;
            unsigned int get_item_amount(unsigned int item_id) const;
            unsigned int get_item_amount(item::sptr item_sptr) const;
            void get_item_list(item_list_t &item_list) const;

            unsigned int get_game_id() const;

        private:

            unsigned int _game_id, _generation, _pocket_size;
            pkmn::pkstring _pocket_name;

            pkmn::dict<uint16_t, uint8_t> _item_dict;
    };
}

#endif /* INCLUDED_POCKET_IMPL_HPP */
