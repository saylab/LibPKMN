/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_POCKET_IMPL_HPP
#define INCLUDED_POCKET_IMPL_HPP

#include <vector>

#include <pkmn/pocket.hpp>
#include <pkmn/types/dict.hpp>

namespace pkmn
{
    class pocket_impl: public pocket
    {
        public:

            pocket_impl(int game, const pkmn::pkstring &name, int size);

            pkmn::pkstring get_game() const;
            int get_generation() const;

            pkmn::pkstring get_name() const;
            int get_size() const;

            int get_item_amount(int item_id) const;
            int get_item_amount(const pkmn::pkstring &item_name) const;
            void set_item_amount(int item_id, int amount);
            void set_item_amount(const pkmn::pkstring &item_name, int amount);

            const item_list_t& get_item_list() const;

            int get_game_id() const;

        private:

            int _version_id, _generation, _pocket_size;
            pkmn::pkstring _pocket_name;
            pokedex::sptr _pokedex;

            item_list_t _item_list;
    };
}

#endif /* INCLUDED_POCKET_IMPL_HPP */
