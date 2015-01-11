/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_POKEDEX_ITEM_ENTRY_HPP
#define INCLUDED_PKMN_POKEDEX_ITEM_ENTRY_HPP

#include <cstdint>

#include <pkmn/types/dict.hpp>
#include <pkmn/types/pkstring.hpp>

namespace pkmn
{
    typedef struct
    {
        pkmn::pkstring name;
        pkmn::pkstring category;
        pkmn::pkstring pocket;
        pkmn::pkstring description;

        uint16_t cost;

        uint8_t fling_power;
        pkmn::pkstring fling_effect;
    } item_entry_t;
}

#endif /* INCLUDED_PKMN_POKEDEX_ITEM_ENTRY_HPP */
