/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_NATIVE_POKEMON_HPP
#define INCLUDED_PKMN_NATIVE_POKEMON_HPP

#include <cstdint>

#include <pkmn/types/contest_stats.hpp>

namespace pkmn
{
    namespace native
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
            contest_stats_t contest_stats;
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
            uint8_t blocks16[24];
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
            uint32_t current_box;
            gen3_pokemon_box_t boxes[14];
            uint8_t box_names[14][9];
            uint8_t wallpapers[14];
        } gen3_pokemon_pc_t;

        /*
         * Nintendo DS (Generations IV-V)
         *
         * Source: http://projectpokemon.org/wiki/Pokemon_NDS_Structure
         */
        typedef struct
        {
            uint16_t species;
            uint16_t held_item;
            union
            {
                uint32_t ot_id;
                struct
                {
                    uint16_t ot_pid;
                    uint16_t ot_sid;
                };
            };
            uint32_t exp;
            uint8_t friendship;
            uint8_t ability;
            uint8_t markings;
            uint8_t country;
            uint8_t ev_hp;
            uint8_t ev_atk;
            uint8_t ev_def;
            uint8_t ev_spd;
            uint8_t ev_spatk;
            uint8_t ev_spdef;
            contest_stats_t contest_stats;
            uint16_t sinnoh_ribbons1;
            union
            {
                uint16_t sinnoh_ribbons2; // Generation IV
                uint16_t unova_ribbons;   // Generation V
            };
        } nds_pokemon_blockA_t;

        typedef struct
        {
            uint16_t moves[4];
            uint8_t move_pps[4];
            uint8_t move_pp_ups[4];
            uint32_t iv_isegg_isnicknamed;
            uint32_t hoenn_ribbons;
            uint8_t form_encounterinfo; // Fateful encounter, is female, is genderless
            union
            {
                uint8_t shiny_leaf; // Generation IV
                uint8_t nature;     // Generation V
            };
            uint8_t gen5_info; // Has Dream World ability, N's Pokemon flag
            uint8_t unknown_0x43;
            uint16_t eggmet_plat;
            uint16_t met_plat;
        } nds_pokemon_blockB_t;

        typedef struct
        {
            uint16_t nickname[11];
            uint8_t unknown_0x5E;
            uint8_t hometown;
            uint32_t sinnoh_ribbons3;
            uint8_t unknown_0x64;
            uint8_t unknown_0x65;
            uint8_t unknown_0x66;
            uint8_t unknown_0x67;
        } nds_pokemon_blockC_t;

        typedef struct
        {
            uint16_t otname[8];
            uint8_t eggmet_date[3];
            uint8_t met_date[3];
            uint16_t eggmet_dp;
            uint16_t met_dp;
            uint8_t pokerus;
            uint8_t ball;
            uint8_t metlevel_otgender;
            uint8_t encounter_info;
            uint8_t ball_hgss;
            uint8_t unknown_0x87;
        } nds_pokemon_blockD_t;

        typedef union
        {
            uint8_t blocks8[136];
            uint32_t blocks16[68];
            uint32_t blocks32[34];
            uint8_t blocks[4][34];
            struct
            {
                nds_pokemon_blockA_t blockA;
                nds_pokemon_blockB_t blockB;
                nds_pokemon_blockC_t blockC;
                nds_pokemon_blockD_t blockD;
            };
        } nds_pokemon_blocks_t;

        typedef struct
        {
            uint32_t personality;
            uint8_t isdecrypted_isegg; // Is party data decrypted, is box data decrypted, is egg
            uint8_t unknown_0x05;
            uint16_t checksum;
            nds_pokemon_blocks_t blocks;
        } nds_pc_pokemon_t;

        typedef struct
        {
            nds_pc_pokemon_t pc;
            uint8_t status;
            uint8_t unknown_x89;
            uint8_t unknown_x8A;
            uint8_t unknown_x8B;
            uint8_t level;
            uint8_t capsule;
            uint16_t current_hp;
            uint16_t max_hp;
            uint16_t atk;
            uint16_t def;
            uint16_t spd;
            uint16_t spatk;
            uint16_t spdef;
            uint8_t unknown_0x9C[80];
        } nds_party_pokemon_t;

        typedef struct
        {
            uint32_t count;
            nds_party_pokemon_t party[6];
            uint16_t checksum;
        } nds_pokemon_party_t;

        typedef struct
        {
            nds_pc_pokemon_t entries[30];
            uint16_t checksum;
            uint8_t unknown[12];
        } nds_pokemon_box_t;

        /*
         * Generation VI
         *
         * Source: http://projectpokemon.org/wiki/Pokemon_X/Y_3DS_Structure
         */
        typedef struct
        {
            uint16_t species;
            uint16_t held_item;
            union
            {
                uint32_t ot_id;
                struct
                {
                    uint16_t ot_pid;
                    uint16_t ot_sid;
                };
            };
            uint32_t exp;
            uint8_t ability;
            uint8_t ability_num;
            uint16_t hits_remaining; // On Training Bag
            uint32_t personality;
            uint8_t nature;
            uint8_t form_encounterinfo;
            uint8_t ev_hp;
            uint8_t ev_atk;
            uint8_t ev_def;
            uint8_t ev_spd;
            uint8_t ev_spatk;
            uint8_t ev_spdef;
            contest_stats_t contest_stats;
            uint8_t markings;
            uint8_t pokerus;
            uint32_t super_training_medals;
            uint8_t ribbons[6];
            uint16_t unused_x36;
            uint8_t contest_memory_ribbon_count;
            uint8_t battle_memory_ribbon_count;
            uint8_t distribution_super_training_flags;
            uint32_t unused_x3B;
        } gen6_pokemon_blockA_t;

        typedef struct
        {
            uint16_t nickname[12];
            uint16_t null_terminator;
            uint16_t moves[4];
            uint8_t move_pps[4];
            uint16_t relearn_moves[4];
            uint8_t secret_super_training_flag;
            uint8_t unused_x73;
            uint32_t iv_isegg_isnicknamed;
        } gen6_pokemon_blockB_t;

        typedef struct
        {
            uint8_t latest_not_ot_handler[24];
            uint16_t null_terminator;
            uint8_t not_ot_gender;
            uint8_t current_handler;
            uint16_t geolocations[5];
            uint16_t unused_x9E;
            uint16_t unused_xA0;
            uint8_t not_ot_friendship;
            uint8_t not_ot_affection;
            uint8_t not_ot_memory_intensity;
            uint8_t not_ot_memory_line;
            uint8_t not_ot_memory_feeling;
            uint8_t unused_xA7;
            uint16_t not_ot_memory_textvar;
            uint16_t unused_xAA;
            uint16_t unused_xAC;
            uint8_t fullness;
            uint8_t enjoyment;
        } gen6_pokemon_blockC_t;

        typedef struct
        {
            uint16_t otname[12];
            uint16_t null_terminator;
            uint8_t ot_friendship;
            uint8_t ot_affection;
            uint8_t ot_memory_intensity;
            uint8_t ot_memory_line;
            uint16_t ot_memory_textvar;
            uint8_t ot_memory_feeling;
            uint8_t eggmet_date[3];
            uint8_t met_date[3];
            uint8_t unused_D7;
            uint16_t eggmet_location;
            uint16_t met_location;
            uint8_t ball;
            uint8_t metlevel_otgender;
            uint8_t gen4_encounterinfo;
            uint8_t ot_game;
            uint8_t ot_country;
            uint8_t region;
            uint8_t region_3ds;
            uint8_t ot_language;
            uint32_t unused_0xE4;
        } gen6_pokemon_blockD_t;

        typedef union
        {
            uint8_t blocks8[232];
            uint32_t blocks16[116];
            uint32_t blocks32[58];
            uint8_t blocks[4][58];
            struct
            {
                gen6_pokemon_blockA_t blockA;
                gen6_pokemon_blockB_t blockB;
                gen6_pokemon_blockC_t blockC;
                gen6_pokemon_blockD_t blockD;
            };
        } gen6_pokemon_blocks_t;

        typedef struct
        {
            uint32_t encryption_key;
            uint16_t placeholder;
            uint16_t checksum;
            gen6_pokemon_blocks_t blocks;
        } gen6_pc_pokemon_t;

        typedef struct
        {
            gen6_pc_pokemon_t pc;
            uint8_t status;
            uint8_t unknown_xE9;
            uint16_t unused_xEA;
            uint8_t level;
            uint8_t unused_xED;
            uint16_t unused_xEE;
            uint16_t current_hp;
            uint16_t max_hp;
            uint16_t atk;
            uint16_t def;
            uint16_t spd;
            uint16_t spatk;
            uint16_t spdef;
            uint16_t unused_xFE;
            uint32_t unused_x100;
        } gen6_party_pokemon_t;
        #pragma pack(pop)
    }
}

#endif /* INCLUDED_PKMN_NATIVE_POKEMON_HPP */
