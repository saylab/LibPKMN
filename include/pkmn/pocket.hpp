/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_POCKET_HPP
#define INCLUDED_PKMN_POCKET_HPP

#include <vector>

#include <pkmn/config.hpp>
#include <pkmn/pokedex.hpp>
#include <pkmn/types/pkstring.hpp>
#include <pkmn/types/shared_ptr.hpp>

namespace pkmn
{
    typedef struct
    {
        item_entry_t item;
        int amount;
    } bag_slot_t;

    typedef std::vector<bag_slot_t> item_list_t;

    class PKMN_API pocket
    {
        public:

            typedef pkmn::shared_ptr<pocket> sptr;
            static sptr make(int game, const pkmn::pkstring &name, int size);
            static sptr make(const pkmn::pkstring &game, const pkmn::pkstring &name, int size);

            pocket() {};
            virtual ~pocket() {};

            virtual pkmn::pkstring get_game() const = 0;
            virtual int get_generation() const = 0;

            virtual pkmn::pkstring get_name() const = 0;
            virtual int get_size() const = 0;

            virtual int get_item_amount(int item_id) const = 0;
            virtual int get_item_amount(const pkmn::pkstring &item_name) const = 0;
            virtual void set_item_amount(int item_id, int amount) = 0;
            virtual void set_item_amount(const pkmn::pkstring &item_name, int amount) = 0;

            virtual const item_list_t& get_item_list() const = 0;

            virtual int get_game_id() const = 0;
    };
}

#endif /* INCLUDED_PKMN_POCKET_HPP */
