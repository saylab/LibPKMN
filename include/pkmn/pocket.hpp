/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_POCKET_HPP
#define INCLUDED_PKMN_POCKET_HPP

#include <utility>
#include <vector>

#include <pkmn/config.hpp>
#include <pkmn/pokedex.hpp>
#include <pkmn/types/pkstring.hpp>
#include <pkmn/types/shared_ptr.hpp>

namespace pkmn
{
    typedef std::pair<item_entry_t, uint16_t> bag_slot_t;
    typedef std::vector<bag_slot_t> item_list_t;

    class PKMN_API pocket
    {
        public:

            typedef pkmn::shared_ptr<pocket> sptr;
            static sptr make(uint16_t game, const pkmn::pkstring& name, uint16_t size);
            static sptr make(const pkmn::pkstring& game, const pkmn::pkstring& name, uint16_t size);

            pocket() {};
            virtual ~pocket() {};

            virtual pkmn::pkstring get_game() const = 0;
            virtual uint16_t get_generation() const = 0;

            virtual pkmn::pkstring get_name() const = 0;
            virtual uint16_t size() const = 0;

            virtual void add_item(const pkmn::pkstring& item_name, uint16_t amount) = 0;
            virtual void add_item(uint16_t item_id, uint16_t amount) = 0;
            virtual void remove_item(const pkmn::pkstring& item_name, uint16_t amount) = 0;
            virtual void remove_item(uint16_t item_id, uint16_t amount) = 0;
            virtual uint16_t get_item_amount(const pkmn::pkstring& item_name) const = 0;
            virtual uint16_t get_item_amount(uint16_t item_id) const = 0;
            virtual void get_item_list(item_list_t& item_list) const = 0;

            virtual uint16_t get_game_id() const = 0;
    };
}

#endif /* INCLUDED_PKMN_POCKET_HPP */
