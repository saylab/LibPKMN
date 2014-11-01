/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_CONVERSIONS_STRUCTS_GEN3_SAVE_HPP
#define INCLUDED_PKMN_CONVERSIONS_STRUCTS_GEN3_SAVE_HPP

#include <cstdint>

#include "pokemon.hpp"

namespace pkmn
{
    #pragma pack(push,1)
    /*
     * Source: http://bulbapedia.bulbagarden.net/wiki/Save_data_structure_in_Generation_III#Time_played
     */
    typedef struct
    {
        uint16_t hours;
        uint8_t minutes;
        uint8_t seconds;
        uint8_t frames;
    } gen3_time_t;

    typedef struct
    {
        uint8_t name[7];
        uint8_t padding_0x2;
        uint8_t gender;
        uint8_t padding_0x4;
        union
        {
            uint32_t tid;
            struct
            {
                uint16_t pid;
                uint16_t sid;
            };
        };
        gen3_time_t time_played;
    } gen3_trainer_t;

    typedef struct
    {
        uint8_t section_id;
        uint8_t padding;
        uint16_t checksum;
        uint32_t validation; //0x08012025
        uint32_t save_index;
    } gen3_section_footer_t;

    typedef struct
    {
        union
        {
            uint8_t data8[3968];
            uint32_t data32[3968/4];
        };
        uint8_t padding[116];
        gen3_section_footer_t footer;
    } gen3_save_section_t;

    typedef struct
    {
        union
        {
            gen3_save_section_t sections[14];
            struct
            {
                union
                {
                    gen3_save_section_t section0;
                    gen3_trainer_t trainer_info;
                };
                gen3_save_section_t section1; //Team/items
                gen3_save_section_t section2; //Unknown
                gen3_save_section_t section3; //Unknown
                gen3_save_section_t section4; //Rival info
                union
                {
                    gen3_pokemon_pc_t pokemon_pc;
                    struct
                    {
                        gen3_save_section_t section_5;
                        gen3_save_section_t section_6;
                        gen3_save_section_t section_7;
                        gen3_save_section_t section_8;
                        gen3_save_section_t section_9;
                        gen3_save_section_t section_10;
                        gen3_save_section_t section_11;
                        gen3_save_section_t section_12;
                        gen3_save_section_t section_13;
                    };
                };
            };
        };
    } gen3_save_t;
    #pragma pack(pop)
}

#endif /* INCLUDED_PKMN_CONVERSIONS_STRUCTS_GEN3_SAVE_HPP */
