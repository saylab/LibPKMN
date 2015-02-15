/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TYPES_CONTEST_STATS_HPP
#define INCLUDED_PKMN_TYPES_CONTEST_STATS_HPP

#include <cstdint>

namespace pkmn
{
    //! Pokémon's <a href="http://bulbapedia.bulbagarden.net/wiki/Contest">contest</a> stats.
    /*!
     * This class represents a Pokémon's <a href="http://bulbapedia.bulbagarden.net/wiki/Contest">contest</a>
     * stats. On the C++ level, this struct is packed, matching the in-game representation of these stats.
     */
    #pragma pack(push,1)
    typedef struct
    {
        uint8_t coolness;
        uint8_t beauty;
        uint8_t cuteness;
        uint8_t smartness;
        uint8_t toughness;
        union
        {
            uint8_t feel;
            uint8_t sheen;
        };
    } contest_stats_t;
    #pragma pack(pop)
}

#endif /* INCLUDED_PKMN_TYPES_CONTEST_STATS_HPP */
