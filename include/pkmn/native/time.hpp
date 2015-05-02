/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_NATIVE_TIME_HPP
#define INCLUDED_PKMN_NATIVE_TIME_HPP

#include <cstdint>

namespace pkmn
{
    namespace native
    {
        #pragma pack(push,1)
        typedef struct
        {
            uint16_t hours;
            uint8_t minutes;
            uint8_t seconds;
        } gen1_time_t;

        typedef struct
        {
            uint8_t hours;
            uint8_t minutes;
            uint8_t seconds;
            uint8_t frames;
        } gen2_time_t;

        typedef struct
        {
            uint16_t hours;
            uint8_t minutes;
            uint8_t seconds;
            uint8_t frames;
        } gen3_time_t;

        typedef struct
        {
            uint16_t hours;
            uint8_t minutes;
            uint8_t seconds;
        } nds_time_t;
        #pragma pack(pop)
    }
}
#endif /* INCLUDED_PKMN_NATIVE_TIME_HPP */
