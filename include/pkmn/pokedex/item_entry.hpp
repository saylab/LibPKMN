/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_POKEDEX_ITEM_ENTRY_HPP
#define INCLUDED_PKMN_POKEDEX_ITEM_ENTRY_HPP

#include <cstdint>

#include <pkmn/config.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/pkstring.hpp>

namespace pkmn
{
    struct PKMN_API item_entry_t
    {
        item_entry_t() {};
        item_entry_t(uint16_t version_id,
                     uint16_t item_id);
        item_entry_t(const pkmn::pkstring& version_name,
                     const pkmn::pkstring& item_name);

        pkmn::pkstring name;
        pkmn::pkstring category;
        pkmn::pkstring pocket;
        pkmn::pkstring description;

        uint16_t cost;

        uint8_t fling_power;
        pkmn::pkstring fling_effect;
    };
}

#endif /* INCLUDED_PKMN_POKEDEX_ITEM_ENTRY_HPP */
