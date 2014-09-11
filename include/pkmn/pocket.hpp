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
#include <pkmn/item.hpp>
#include <pkmn/types/pkstring.hpp>
#include <pkmn/types/shared_ptr.hpp>

namespace pkmn
{
    typedef std::pair<item::sptr, unsigned int> bag_slot_t;
    typedef std::vector<bag_slot_t> item_list_t;

    class PKMN_API pocket
    {
        public:

            typedef pkmn::shared_ptr<pocket> sptr;
            static sptr make(unsigned int game, const pkmn::pkstring &name, unsigned int size);
            static sptr make(const pkmn::pkstring &game, const pkmn::pkstring &name, unsigned int size);

            pocket() {};
            virtual ~pocket() {};

            virtual pkmn::pkstring get_game() const = 0;
            virtual unsigned int get_generation() const = 0;

            virtual pkmn::pkstring get_name() const = 0;
            virtual unsigned int size() const = 0;

            virtual void add_item(const pkmn::pkstring &item_name, unsigned int amount) = 0;
            virtual void add_item(unsigned int item_id, unsigned int amount) = 0;
            virtual void add_item(item::sptr item_sptr, unsigned int amount) = 0;
            virtual void remove_item(const pkmn::pkstring &item_name, unsigned int amount) = 0;
            virtual void remove_item(unsigned int item_id, unsigned int amount) = 0;
            virtual void remove_item(item::sptr item_sptr, unsigned int amount) = 0;
            virtual unsigned int get_item_amount(const pkmn::pkstring &item_name) const = 0;
            virtual unsigned int get_item_amount(unsigned int item_id) const = 0;
            virtual unsigned int get_item_amount(item::sptr item_sptr) const = 0;
            virtual void get_item_list(item_list_t &item_list) const = 0;

            virtual unsigned int get_game_id() const = 0;
    };
}

#endif /* INCLUDED_PKMN_POCKET_HPP */
