/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_CONVERSIONS_ITEMS_HPP
#define INCLUDED_PKMN_CONVERSIONS_ITEMS_HPP

#include <cstdint>

#include <pkmn/bag.hpp>
#include <pkmn/config.hpp>

#include "structs/items.hpp"

namespace pkmn
{
    namespace conversions
    {
        void import_gen1_bag(bag::sptr libpkmn_bag, const gen1_item_bag_t* raw_bag);
        void export_gen1_bag(bag::sptr libpkmn_bag, gen1_item_bag_t* raw_bag);

        void import_gen2_bag(bag::sptr libpkmn_bag, const gen2_item_bag_t* raw_bag);
        void export_gen2_bag(bag::sptr libpkmn_bag, gen2_item_bag_t* raw_bag);

        void import_gen3_bag(bag::sptr libpkmn_bag, const void* raw_bag, const uint16_t security_key);
        void export_gen3_bag(bag::sptr libpkmn_bag, void* raw_bag, const uint16_t security_key);

        void import_gen4_bag(bag::sptr libpkmn_bag, const void* raw_bag);
        void export_gen4_bag(bag::sptr libpkmn_bag, void* raw_bag);
    }
}

#endif /* INCLUDED_PKMN_CONVERSIONS_ITEMS_HPP */
