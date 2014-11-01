/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_CONVERSIONS_STRUCTS_POKEMON_HPP
#define INCLUDED_PKMN_CONVERSIONS_STRUCTS_POKEMON_HPP

#include <cstdint>

namespace pkmn
{
    #pragma pack(push,1)
    /*
     * Generation I
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Pok%C3%A9mon_data_structure_in_Generation_I
     */
    typedef struct
    {
        uint8_t species;
        uint16_t current_hp;
        uint8_t level; //As viewed from the PC
        uint8_t status;
        uint8_t types[2];
        uint8_t catch_rate;
        uint8_t moves[4];
        uint16_t ot_id;
        uint8_t exp[3];
        uint16_t ev_hp;
        uint16_t ev_atk;
        uint16_t ev_def;
        uint16_t ev_spd;
        uint16_t ev_spcl;
        uint16_t iv_data;
        uint8_t move_pps[4];
    } gen1_pc_pokemon_t;

    typedef struct
    {
        gen1_pc_pokemon_t pc;
        uint8_t level;
        uint16_t max_hp;
        uint16_t atk;
        uint16_t def;
        uint16_t spd;
        uint16_t spcl;
    } gen1_party_pokemon_t;

    typedef struct
    {
        uint8_t count;
        uint8_t species[7];
        gen1_party_pokemon_t party[6];
        uint8_t otnames[6][11];
        uint8_t nicknames[6][11];
    } gen1_pokemon_party_t;

    typedef struct
    {
        uint8_t count;
        uint8_t species[21];
        gen1_pc_pokemon_t entries[20];
        uint8_t otnames[20][11];
        uint8_t nicknames[20][11];
    } gen1_pokemon_box_t;

    typedef struct
    {
        gen1_pokemon_box_t boxes[12];
    } gen1_pokemon_pc_t;

    /*
     * Generation II
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Pok%C3%A9mon_data_structure_in_Generation_II
     */
    typedef struct
    {
        uint8_t species;
        uint8_t held_item;
        uint8_t moves[4];
        uint16_t ot_id;
        uint8_t exp[3];
        uint16_t ev_hp;
        uint16_t ev_atk;
        uint16_t ev_def;
        uint16_t ev_spd;
        uint16_t ev_spcl;
        uint16_t iv_data;
        uint8_t move_pps[4];
        uint8_t friendship;
        uint8_t pokerus;
        uint16_t caught_data;
        uint8_t level;
    } gen2_pc_pokemon_t;

    typedef struct
    {
        gen2_pc_pokemon_t pc;
        uint8_t status;
        uint8_t unused;
        uint16_t current_hp;
        uint16_t max_hp;
        uint16_t atk;
        uint16_t def;
        uint16_t spd;
        uint16_t spatk;
        uint16_t spdef;
    } gen2_party_pokemon_t;

    typedef struct
    {
        uint8_t count;
        uint8_t species[7];
        gen2_party_pokemon_t party[6];
        uint8_t otnames[6][11];
        uint8_t nicknames[6][11];
    } gen2_pokemon_party_t;

    typedef struct
    {
        uint8_t count;
        uint8_t species[21];
        gen2_pc_pokemon_t entries[20];
        uint8_t otnames[20][11];
        uint8_t nicknames[20][11];
    } gen2_pokemon_box_t;

    typedef struct
    {
        gen2_pokemon_box_t boxes[15];
    } gen2_pokemon_pc_t;

    /*
     * Generation III
     *
     * Sources: http://bulbapedia.bulbagarden.net/wiki/Pok%C3%A9mon_data_structure_in_Generation_III
     *          http://bulbapedia.bulbagarden.net/wiki/Pok%C3%A9mon_data_substructures_in_Generation_III
     */
    typedef struct
    {
        uint16_t species;
        uint16_t held_item;
        uint32_t exp;
        uint8_t pp_up;
        uint8_t friendship;
        uint16_t unknown_0xA;
    } gen3_pokemon_growth_t;

    typedef struct
    {
        uint16_t moves[4];
        uint8_t move_pps[4];
    } gen3_pokemon_attacks_t;

    typedef struct
    {
        uint8_t ev_hp;
        uint8_t ev_atk;
        uint8_t ev_def;
        uint8_t ev_spd;
        uint8_t ev_spatk;
        uint8_t ev_spdef;
        uint8_t coolness;
        uint8_t beauty;
        uint8_t cuteness;
        uint8_t smartness;
        uint8_t toughness;
        uint8_t feel;
    } gen3_pokemon_effort_t;

    typedef struct
    {
        uint8_t pokerus;
        uint8_t met_location;
        uint16_t origin_info;
        uint32_t iv_egg_ability;
        uint32_t ribbons_obedience;
    } gen3_pokemon_misc_t;

    typedef union
    {
        uint8_t blocks8[48];
        uint32_t blocks32[12];
        uint8_t blocks[4][12];
        struct
        {
            gen3_pokemon_growth_t growth;
            gen3_pokemon_attacks_t attacks;
            gen3_pokemon_effort_t effort;
            gen3_pokemon_misc_t misc;
        };
    } gen3_pokemon_blocks_t;

    typedef struct
    {
        uint32_t personality;
        union
        {
            uint32_t ot_id;
            struct
            {
                uint16_t ot_pid;
                uint16_t ot_sid;
            };
        };
        uint8_t nickname[10];
        uint16_t language;
        uint8_t otname[7];
        uint8_t markings;
        uint16_t checksum;
        uint16_t unknown_0x1E;
        gen3_pokemon_blocks_t blocks;
    } gen3_pc_pokemon_t;

    typedef struct
    {
        gen3_pc_pokemon_t pc;
        uint32_t condition;
        uint8_t level;
        uint8_t pokerus_time;
        uint16_t current_hp;
        uint16_t max_hp;
        uint16_t atk;
        uint16_t def;
        uint16_t spd;
        uint16_t spatk;
        uint16_t spdef;
    } gen3_party_pokemon_t;

    typedef struct
    {
        uint32_t count;
        gen3_party_pokemon_t party[6];
    } gen3_pokemon_party_t;

    typedef struct
    {
        gen3_pc_pokemon_t entries[30];
    } gen3_pokemon_box_t;

    typedef struct
    {
        uint32_t current_box_num;
        gen3_pokemon_box_t boxes[14];
        uint8_t box_names[14][9];
        uint8_t wallpapers[14];
    } gen3_pokemon_pc_t;
    #pragma pack(pop)
}

#endif /* INCLUDED_PKMN_CONVERSIONS_STRUCTS_POKEMON_HPP */
