/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifdef _MSC_VER
#pragma warning(disable:4800) //Type-narrowing is inevitable
#endif

#include <string>

#include <pkmn/enums.hpp>
#include <pkmn/native/pokemon.hpp>

#include "utils.hpp"

namespace pkmn
{
    namespace conversions
    {
        std::string crystal_get_otgender(const uint16_t &caught_data)
        {
            return ((caught_data & 0x80) >> 7) ? "Female" : "Male";
        }

        unsigned int crystal_get_metlevel(const uint16_t &caught_data)
        {
            return ((caught_data & 0x3F00) >> 8);
        }

        void crystal_set_caughtdata(uint16_t &caught_data, bool ot_is_female, uint8_t met_level)
        {
            if(ot_is_female) caught_data |= 0x80;
            else caught_data &= ~0x80;

            caught_data |= ((met_level & 0x3F) << 8);
        }

        unsigned int get_modern_IV(uint8_t stat, const uint32_t &IVint)
        {
            switch(stat)
            {
                case Stats::HP:
                    return ((IVint >> 2) & 31);

                case Stats::ATTACK:
                    return ((IVint >> 7) & 31);

                case Stats::DEFENSE:
                    return ((IVint >> 12) & 31);

                case Stats::SPEED:
                    return ((IVint >> 17) & 31);

                case Stats::SPECIAL_ATTACK:
                    return ((IVint >> 22) & 31);

                case Stats::SPECIAL_DEFENSE:
                    return ((IVint >> 27) & 31);

                default:
                    return 0;
            }
        }

        void set_modern_IV(uint8_t stat, uint32_t &IVint, uint8_t val)
        {
            switch(stat)
            {   
                case Stats::HP:
                    IVint = ((IVint & 0xFFFFFF83) | (val << 2));
                    break;

                case Stats::ATTACK:
                    IVint = ((IVint & 0xFFFFF07F) | (val << 7));
                    break;

                case Stats::DEFENSE:
                    IVint = ((IVint & 0xFFFE0FFF) | (val << 12));
                    break;

                case Stats::SPEED:
                    IVint = ((IVint & 0xFFC1FFFF) | (val << 17));
                    break;

                case Stats::SPECIAL_ATTACK:
                    IVint = ((IVint & 0xF83FFFFF) | (val << 22));
                    break;

                case Stats::SPECIAL_DEFENSE:
                    IVint = ((IVint & 0x7FFFFFF) | (val << 27));
                    break;
            }
        }
    }
}
