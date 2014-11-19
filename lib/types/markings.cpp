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
        circle =   (markint & 0x01) ? true : false;
        triangle = (markint & 0x02) ? true : false;
        square =   (markint & 0x04) ? true : false;
        heart =    (markint & 0x08) ? true : false;
        star =     (markint & 0x10) ? true : false;
        diamond =  (markint & 0x20) ? true : false;
    }

    markings::operator uint8_t()
    {
        uint8_t markint = 0;

        if(circle)   markint |= 0x01;
        if(triangle) markint |= 0x02;
        if(square)   markint |= 0x04;
        if(heart)    markint |= 0x08;
        if(star)     markint |= 0x10;
        if(diamond)  markint |= 0x20;

        return markint;
    }
}
