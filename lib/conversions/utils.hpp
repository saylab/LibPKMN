/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_CONVERSIONS_UTILS_HPP
#define INCLUDED_PKMN_CONVERSIONS_UTILS_HPP

#include <boost/assign/list_of.hpp>

#include <pkmn/types/dict.hpp>
#include <pkmn/types/pkstring.hpp>

#include "structs/pokemon.hpp"

namespace pkmn
{
    namespace conversions
    {
        /*   
         * Catch rates
         * http://bulbapedia.bulbagarden.net/wiki/List_of_Pok%C3%A9mon_by_catch_rate
         */
        static const uint8_t gen1_catch_rates[] = { 
            0,45,45,45,45,45,45,45,45,45,255,120,45,255,120,45,255,120,45,255,127,
            255,90,255,90,190,75,255,90,235,120,45,235,120,45,150,25,190,75,170,
            50,255,90,255,120,45,190,75,190,255,50,255,90,190,75,190,75,190,75,
            255,120,45,200,100,50,180,90,45,255,120,45,190,60,255,120,45,190,60,
            190,75,190,60,45,190,45,190,75,190,75,190,60,190,90,45,45,190,75,225,
            60,190,60,90,45,190,75,45,45,45,190,60,120,60,30,45,45,225,75,225,60,
            45,45,45,45,45,45,45,255,45,45,35,45,45,45,45,45,45,45,45,45,45,25,3,3,
            3,45,45,45,3,45,45,45,45,45,45,45,45,45,255,90,255,90,255,90,255,90,90,
            190,75,190,150,170,190,75,190,75,235,120,45,45,190,75,65,45,255,120,45,
            45,235,120,75,255,90,45,45,30,70,45,225,45,60,190,75,190,60,25,190,
            75,45,25,190,45,60,120,60,190,75,225,75,60,190,75,45,25,25,120,45,45,
            120,60,45,45,45,75,45,45,45,45,45,30,3,3,3,45,45,45,3,3,45
        };

        static const pkmn::dict<uint8_t, pkmn::pkstring> retro_statuses = boost::assign::map_list_of
            (0x00, "OK")
            (0x04, "Asleep")
            (0x08, "Poisoned")
            (0x10, "Burned")
            (0x20, "Frozen")
            (0x40, "Paralyzed")
        ;

        static const pkmn::dict<pkmn::pkstring, uint8_t> reverse_retro_statuses = boost::assign::map_list_of
            ("OK",        0x00)
            ("Asleep",    0x04)
            ("Poisoned",  0x08)
            ("Burned",    0x10)
            ("Frozen",    0x20)
            ("Paralyzed", 0x40)
        ;

        static const pkmn::dict<uint8_t, pkmn::pkstring> modern_statuses = boost::assign::map_list_of
            (0x00, "OK")
            (0x01, "Asleep")
            (0x02, "Asleep")
            (0x03, "Asleep")
            (0x04, "Asleep")
            (0x05, "Asleep")
            (0x06, "Asleep")
            (0x07, "Asleep")
            (0x08, "Poisoned")
            (0x10, "Burned")
            (0x20, "Frozen")
            (0x40, "Paralyzed")
            (0x80, "Badly Poisoned")
        ;

        static const pkmn::dict<uint8_t, pkmn::pkstring> modern_statuses = boost::assign::map_list_of
            ("OK",             0x00)
            ("Asleep",         0x01)
            ("Asleep",         0x02)
            ("Asleep",         0x03)
            ("Asleep",         0x04)
            ("Asleep",         0x05)
            ("Asleep",         0x06)
            ("Asleep",         0x07)
            ("Poisoned",       0x08)
            ("Burned",         0x10)
            ("Frozen",         0x20)
            ("Paralyzed",      0x40)
            ("Badly Poisoned", 0x80)
        ;

        uint8_t get_retro_IV(uint8_t stat, const uint16_t iv_data);

        void set_retro_IV(uint8_t stat, uint16_t &iv_data, uint8_t new_iv);

        std::string crystal_get_otgender(const uint16_t &caught_data);

        unsigned int crystal_get_metlevel(const uint16_t &caught_data);

        //TODO: time of day, met location
        void crystal_set_caughtdata(uint16_t &caught_data, bool ot_is_female, uint8_t met_level);

        unsigned int get_modern_IV(uint8_t stat, const uint32_t &IVint);

        void set_modern_IV(uint8_t stat, uint32_t &IVint, uint8_t val);

        #define get_gen3_ability_slot(iv_egg_ability) (uint16_t(iv_egg_ability) & 0x1)

        #define set_gen3_ability_slot(iv_egg_ability, num) iv_egg_ability = num ? (iv_egg_ability | 1) : ~(iv_egg_ability & 1);
    }
}

#endif /* INCLUDED_PKMN_CONVERSIONS_UTILS_HPP */
