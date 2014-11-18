/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TYPES_MARKINGS_HPP
#define INCLUDED_PKMN_TYPES_MARKINGS_HPP

#include <cstdint>

#include <pkmn/config.hpp>

namespace pkmn
{
    struct PKMN_API markings
    {
        markings() {};
        markings(const uint8_t markint);

        enum
        {
            CIRCLE,
            TRIANGLE,
            SQUARE,
            HEART,
            STAR,
            DIAMOND
        };

        bool circle;
        bool triangle;
        bool square;
        bool heart;
        bool star;
        bool diamond;

        operator uint8_t();
    };
}

#endif /* INCLUDED_PKMN_TYPES_MARKINGS_HPP */
