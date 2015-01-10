/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_POKEDEX_MOVE_ENTRY_HPP
#define INCLUDED_PKMN_POKEDEX_MOVE_ENTRY_HPP

#include <cstdint>

#include <pkmn/types/dict.hpp>
#include <pkmn/types/pkstring.hpp>

namespace pkmn
{
    typedef struct
    {
        pkmn::pkstring name;
        pkmn::pkstring type;
        pkmn::pkstring description;
        pkmn::pkstring damage_class;
        pkmn::pkstring target;

        uint8_t power;
        uint8_t pp;
        float accuracy;
        int8_t priority;

        pkmn::pkstring effect;
        float effect_chance;

        pkmn::pkstring contest_type;
        pkmn::pkstring contest_effect;
        pkmn::pkstring super_contest_effect;
    } move_entry_t;
}

#endif /* INCLUDED_PKMN_POKEDEX_MOVE_ENTRY_HPP */
