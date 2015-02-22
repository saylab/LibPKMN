/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifdef _MSC_VER
#pragma warning(disable: 4800) // forcing value to bool 'true' or 'false'
#endif

#include <cstdint>

#include <pkmn/types/super_training_medals.hpp>

namespace pkmn
{
    super_training_medals_t::super_training_medals_t(uint32_t medalint)
    {
        uint8_t* medal_bytes = reinterpret_cast<uint8_t*>(&medalint);

        // 0x01 unused
        // 0x02 unused
        spatk_level1          = medal_bytes[0] & 0x04;
        hp_level1             = medal_bytes[0] & 0x08;
        atk_level1            = medal_bytes[0] & 0x10;
        spdef_level1          = medal_bytes[0] & 0x20;
        spd_level1            = medal_bytes[0] & 0x40;
        def_level1            = medal_bytes[0] & 0x80;

        spatk_level2          = medal_bytes[1] & 0x01;
        hp_level2             = medal_bytes[1] & 0x02;
        atk_level2            = medal_bytes[1] & 0x04;
        spdef_level2          = medal_bytes[1] & 0x08;
        spd_level2            = medal_bytes[1] & 0x10;
        def_level2            = medal_bytes[1] & 0x20;
        spatk_level3          = medal_bytes[1] & 0x40;
        hp_level3             = medal_bytes[1] & 0x80;

        atk_level3            = medal_bytes[2] & 0x01;
        spdef_level3          = medal_bytes[2] & 0x02;
        spd_level3            = medal_bytes[2] & 0x04;
        def_level3            = medal_bytes[2] & 0x08;
        secret_troubles       = medal_bytes[2] & 0x10;
        secret_leafstone      = medal_bytes[2] & 0x20;
        secret_firestone      = medal_bytes[2] & 0x40;
        secret_waterstone     = medal_bytes[2] & 0x80;

        secret_followgoals    = medal_bytes[3] & 0x01;
        secret_watchout       = medal_bytes[3] & 0x02;
        secret_lightningquick = medal_bytes[3] & 0x04;
        secret_longshots      = medal_bytes[3] & 0x08;
        secret_scatterbug     = medal_bytes[3] & 0x10;
        secret_bitbots        = medal_bytes[3] & 0x20;
        secret_hydreigon      = medal_bytes[3] & 0x40;
        secret_battlebest     = medal_bytes[3] & 0x80;
    }

    super_training_medals_t::operator uint32_t()
    {
        uint32_t medalint = 0;
        uint8_t* medal_bytes = reinterpret_cast<uint8_t*>(&medalint);

        // 0x01 unused
        // 0x02 unused
        if(spatk_level1)          medal_bytes[0] |= 0x04;
        if(hp_level1)             medal_bytes[0] |= 0x08;
        if(atk_level1)            medal_bytes[0] |= 0x10;
        if(spdef_level1)          medal_bytes[0] |= 0x20;
        if(spd_level1)            medal_bytes[0] |= 0x40;
        if(def_level1)            medal_bytes[0] |= 0x80;

        if(spatk_level2)          medal_bytes[1] |= 0x01;
        if(hp_level2)             medal_bytes[1] |= 0x02;
        if(atk_level2)            medal_bytes[1] |= 0x04;
        if(spdef_level2)          medal_bytes[1] |= 0x08;
        if(spd_level2)            medal_bytes[1] |= 0x10;
        if(def_level2)            medal_bytes[1] |= 0x20;
        if(spatk_level3)          medal_bytes[1] |= 0x40;
        if(hp_level3)             medal_bytes[1] |= 0x80;

        if(atk_level3)            medal_bytes[2] |= 0x01;
        if(spdef_level3)          medal_bytes[2] |= 0x02;
        if(spd_level3)            medal_bytes[2] |= 0x04;
        if(def_level3)            medal_bytes[2] |= 0x08;
        if(secret_troubles)       medal_bytes[2] |= 0x10;
        if(secret_leafstone)      medal_bytes[2] |= 0x20;
        if(secret_firestone)      medal_bytes[2] |= 0x40;
        if(secret_waterstone)     medal_bytes[2] |= 0x20;

        if(secret_followgoals)    medal_bytes[3] |= 0x01;
        if(secret_watchout)       medal_bytes[3] |= 0x02;
        if(secret_lightningquick) medal_bytes[3] |= 0x04;
        if(secret_longshots)      medal_bytes[3] |= 0x08;
        if(secret_scatterbug)     medal_bytes[3] |= 0x10;
        if(secret_bitbots)        medal_bytes[3] |= 0x20;
        if(secret_hydreigon)      medal_bytes[3] |= 0x40;
        if(secret_battlebest)     medal_bytes[3] |= 0x80;

        return medalint;
    }
}
