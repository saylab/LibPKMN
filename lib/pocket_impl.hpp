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

            pocket_impl(uint16_t game, const pkmn::pkstring& name, uint16_t size);

            pkmn::pkstring get_game() const;
            uint16_t get_generation() const;

            pkmn::pkstring get_name() const;
            uint16_t size() const;

            void add_item(const pkmn::pkstring& item_name, uint16_t amount);
            void add_item(uint16_t item_id, uint16_t amount);
            void remove_item(const pkmn::pkstring& item_name, uint16_t amount);
            void remove_item(uint16_t item_id, uint16_t amount);
            uint16_t get_item_amount(const pkmn::pkstring& item_name) const;
            uint16_t get_item_amount(uint16_t item_id) const;
            void get_item_list(item_list_t& item_list) const;

            uint16_t get_game_id() const;

        private:

            uint16_t _version_id, _generation, _pocket_size;
            pkmn::pkstring _pocket_name;
            pokedex::sptr _pokedex;

            pkmn::dict<uint16_t, uint8_t> _item_dict;
    };
}

#endif /* INCLUDED_POCKET_IMPL_HPP */
