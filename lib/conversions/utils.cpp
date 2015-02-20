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
        uint8_t get_retro_IV(uint8_t stat, const uint16_t iv_data)
        {
            switch(stat)
            {
                case Stats::HP:
                    return ((get_retro_IV(Stats::ATTACK,  iv_data) & 1) << 3) |
                           ((get_retro_IV(Stats::DEFENSE, iv_data) & 1) << 2) |
                           ((get_retro_IV(Stats::SPEED,   iv_data) & 1) << 1) |
                            (get_retro_IV(Stats::SPECIAL, iv_data) & 1);

                case Stats::ATTACK:
                    return ((iv_data & 0xF000) >> 12);

                case Stats::DEFENSE:
                    return ((iv_data & 0xF00) >> 8);

                case Stats::SPEED:
                    return ((iv_data & 0xF0) >> 4);

                case Stats::SPECIAL:
                case Stats::SPECIAL_ATTACK:
                case Stats::SPECIAL_DEFENSE:
                    return (iv_data & 0xF);

                default:
                    return 0;
            }
        }

        void set_retro_IV(uint8_t stat, uint16_t &iv_data, uint8_t new_iv)
        {   
            // Maximum IV of 15
            if(new_iv > 15) new_iv = 15; 

            switch(stat)
            {   
                case Stats::HP:
                    iv_data = ((iv_data & 0xEEEE) | ((new_iv & 0x8) << 9)
                                                  | ((new_iv & 0x4) << 6)
                                                  | ((new_iv & 0x2) << 3)
                                                  |  (new_iv & 0x1));
                    break;

                case Stats::ATTACK:
                    iv_data = ((iv_data & 0xFFF) | (new_iv << 12));
                    break;

                case Stats::DEFENSE:
                    iv_data = ((iv_data & 0xF0FF) | (new_iv << 8));
                    break;

                case Stats::SPEED:
                    iv_data = ((iv_data & 0xFF0F) | (new_iv << 4));
                    break;

                case Stats::SPECIAL:
                case Stats::SPECIAL_ATTACK:
                case Stats::SPECIAL_DEFENSE:
                    iv_data = ((iv_data & 0xFFF0) | new_iv);
                    break;

                default:
                    break;
            }
        }

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

        enum gen3_substructs
        {
            G, //Growth
            A, //Attacks
            E, //Effort
            M  //Misc
        };

        static const uint8_t gen3_block_orders[24][4] =
        {
            {G,A,E,M},{G,A,M,E},{G,E,A,M},{G,E,M,A},{G,M,A,E},{G,M,E,A},
            {A,G,E,M},{A,G,M,E},{A,E,G,M},{A,E,M,G},{A,M,G,E},{A,M,E,G},
            {E,G,A,M},{E,G,M,A},{E,A,G,M},{E,A,M,G},{E,M,G,A},{E,M,A,G},
            {M,G,A,E},{M,G,E,A},{M,A,G,E},{M,A,E,G},{M,E,G,A},{M,E,A,G}
        };

        uint16_t gen3_crypt(native::gen3_pc_pokemon_t& pkmn)
        {
            uint16_t checksum = 0;
            uint8_t index = pkmn.personality % 24; 

            uint32_t security_key = pkmn.ot_id ^ pkmn.personality;
            for(uint8_t i = 0; i < 48; i++) checksum += pkmn.blocks.blocks8[i];
            for(uint8_t i = 0; i < 12; i++) pkmn.blocks.blocks32[i] ^= security_key;

            return checksum;
        }
    }
}
