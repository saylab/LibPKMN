/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_POKEDEX_MOVE_ENTRY_HPP
#define INCLUDED_PKMN_POKEDEX_MOVE_ENTRY_HPP

#include <cstdint>
#include <vector>

#include <pkmn/config.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/pkstring.hpp>

namespace pkmn
{
    struct PKMN_API move_entry_t
    {
        move_entry_t() {};
        move_entry_t(uint16_t version_id,
                     uint16_t move_id);
        move_entry_t(const pkmn::pkstring& version_name,
                     const pkmn::pkstring& move_name);

        pkmn::pkstring name;
        pkmn::pkstring type;
        pkmn::pkstring description;
        pkmn::pkstring damage_class;
        pkmn::pkstring target;

        uint16_t power;
        uint16_t pp;
        float accuracy;
        int8_t priority;

        pkmn::pkstring effect;
        float effect_chance;

        pkmn::pkstring contest_type;
        pkmn::pkstring contest_effect;
        pkmn::pkstring super_contest_effect;
    };

    typedef std::vector<move_entry_t> moveset_t;
}

#endif /* INCLUDED_PKMN_POKEDEX_MOVE_ENTRY_HPP */