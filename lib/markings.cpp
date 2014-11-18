/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/types/markings.hpp>

#include <cstdint>

namespace pkmn
{
    markings::markings(const uint8_t markint)
    {
        circle =    (markint >> 7)      ? true : false;
        triangle = ((markint >> 6) & 1) ? true : false;
        square =   ((markint >> 5) & 1) ? true : false;
        heart =    ((markint >> 4) & 1) ? true : false;
        star =     ((markint >> 3) & 1) ? true : false;
        diamond =  ((markint >> 2) & 1) ? true : false;
    }

    markings::operator uint8_t()
    {
        uint8_t markint = 0;

        if(circle)   markint |= (1 << 7);
        if(triangle) markint |= (1 << 6);
        if(square)   markint |= (1 << 5);
        if(heart)    markint |= (1 << 4);
        if(star)     markint |= (1 << 3);
        if(diamond)  markint |= (1 << 2);

        return markint;
    }
}
