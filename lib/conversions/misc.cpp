/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/assign.hpp>

#include <pkmn/database.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/conversions/misc.hpp>

namespace pkmn
{
    namespace conversions
    {
        static uint16_t get_gb_IV(int stat, uint16_t iv_data)
        {
            switch(stat)
            {
                case Stats::HP:
                    return ((get_gb_IV(Stats::ATTACK,  iv_data) & 1) << 3) |
                           ((get_gb_IV(Stats::DEFENSE, iv_data) & 1) << 2) |
                           ((get_gb_IV(Stats::SPEED,   iv_data) & 1) << 1) |
                            (get_gb_IV(Stats::SPECIAL, iv_data) & 1); 

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

        pkmn::dict<pkmn::pkstring, uint16_t> import_gb_IVs(uint16_t iv_data)
        {
            pkmn::dict<pkmn::pkstring, uint16_t> IVs = boost::assign::map_list_of
                ("HP",      get_gb_IV(Stats::HP, iv_data))
                ("Attack",  get_gb_IV(Stats::HP, iv_data))
                ("Defense", get_gb_IV(Stats::HP, iv_data))
                ("Speed",   get_gb_IV(Stats::HP, iv_data))
                ("Special", get_gb_IV(Stats::HP, iv_data))
            ;

            return IVs;
        }

        void export_gb_IV(const pkmn::pkstring &stat, uint8_t value, uint16_t &iv_data)
        {
            switch(database::get_stat_id(stat))
            {
                case Stats::HP:
                    iv_data = ((iv_data & 0xEEEE) | ((value & 0x8) << 9)
                                                  | ((value & 0x4) << 6)
                                                  | ((value & 0x2) << 3)
                                                  |  (value & 0x1));
                    break;

                case Stats::ATTACK:
                    iv_data = ((iv_data & 0xFFF) | (value << 12));
                    break;

                case Stats::DEFENSE:
                    iv_data = ((iv_data & 0xF0FF) | (value << 8));
                    break;

                case Stats::SPEED:
                    iv_data = ((iv_data & 0xFF0F) | (value << 4));
                    break;

                case Stats::SPECIAL:
                case Stats::SPECIAL_ATTACK:
                case Stats::SPECIAL_DEFENSE:
                    iv_data = ((iv_data & 0xFFF0) | value);
                    break;

                default:
                    break;
            }
        }

        uint32_t import_gb_money(uint8_t* money_buf)
        {
            return uint32_t(    (money_buf[0] >> 4) * 100000
                             + ((money_buf[0] >> 0) & 0x0F) * 10000
                             +  (money_buf[1] >> 4) * 1000
                             + ((money_buf[1] >> 0) & 0x0F) * 100
                             +  (money_buf[2] >> 4) * 10
                             + ((money_buf[2] >> 0) & 0x0F)
                           );
        }

        void export_gb_money(uint32_t money, uint8_t* money_buf)
        {
            money_buf[0] = ((money / 100000) << 4) | ((money % 100000) / 10000);
            money_buf[1] = (((money % 10000) / 1000) << 4) | ((money % 1000) / 100);
            money_buf[2] = (((money % 100) / 10) << 4) | (money % 10);
        }
    } /* namespace conversions */
} /* namespace pkmn */
