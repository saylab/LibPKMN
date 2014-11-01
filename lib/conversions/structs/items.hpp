/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_CONVERSIONS_STRUCTS_ITEMS_HPP
#define INCLUDED_PKMN_CONVERSIONS_STRUCTS_ITEMS_HPP

#include <cstdint>

namespace pkmn
{
    #pragma pack(push,1)
    /*
     * Generation I
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Save_data_structure_in_Generation_I#Item_lists
     */
    typedef struct
    {
        uint8_t index;
        uint8_t count;
    } gen1_item_t;

    typedef struct
    {
        uint8_t count;
        gen1_item_t items[20];
        uint8_t terminator;
    } gen1_item_bag_t;

    typedef struct
    {
        uint8_t count;
        gen1_item_t items[50];
        uint8_t terminator;
    } gen1_item_pc_t;

    /*
     * Generation II
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Save_data_structure_in_Generation_II#Item_lists
     */
    typedef struct
    {
        uint8_t count;
        uint8_t index;
    } gen2_item_t;

    /*
     * "Each byte specifies the quantity of the corresponding TM that the
     *  player is holding. Should be 0 to 99.
     *
     *  Indexes match item numbers, meaning 0 corresponds with TM01
     *  and 49 corresponds with TM50."
     */
    typedef struct
    {
        uint8_t tm[50];
        uint8_t hm[7];
    } gen2_tmhm_pocket_t;

    typedef struct
    {
        uint8_t count;
        gen2_item_t items[20];
        uint8_t terminator;
    } gen2_item_pocket_t;

    typedef struct
    {
        uint8_t count;
        gen2_item_t items[26];
        uint8_t terminator;
    } gen2_key_item_pocket_t;

    typedef struct
    {
        uint8_t count;
        gen2_item_t items[12];
        uint8_t terminator;
    } gen2_ball_pocket_t;

    typedef struct
    {
        gen2_tmhm_pocket_t tmhm_pocket;
        gen2_item_pocket_t item_pocket;
        gen2_key_item_pocket_t key_item_pocket;
        gen2_ball_pocket_t ball_pocket;
    } gen2_item_bag_t;

    typedef struct
    {
        uint8_t count;
        gen2_item_t items[50];
        uint8_t terminator;
    } gen2_item_pc_t;

    /*
     * Generation III
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Save_data_structure_in_Generation_III#Item_pockets
     */
    typedef struct
    {
        uint16_t index;
        uint16_t count;
    } modern_item_t;

    typedef struct
    {
        modern_item_t pc_items[50];
        modern_item_t items[20];
        modern_item_t key_items[20];
        modern_item_t balls[16];
        modern_item_t tms_hms[64];
        modern_item_t berries[46];
    } rs_item_storage_t;

    typedef struct
    {
        modern_item_t pc_items[50];
        modern_item_t items[30];
        modern_item_t key_items[30];
        modern_item_t balls[16];
        modern_item_t tms_hms[64];
        modern_item_t berries[46];
    } e_item_storage_t;

    typedef struct
    {
        modern_item_t pc_items[50];
        modern_item_t items[42];
        modern_item_t key_items[30];
        modern_item_t balls[13];
        modern_item_t tms_hms[58];
        modern_item_t berries[43];
    } frlg_item_storage_t;
    #pragma pack(pop)
}

#endif /* INCLUDED_PKMN_CONVERSIONS_STRUCTS_ITEMS_HPP */
