/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_CONVERSIONS_STRUCTS_GEN4_SAVE_HPP
#define INCLUDED_PKMN_CONVERSIONS_STRUCTS_GEN4_SAVE_HPP

#include <cstdint>

namespace pkmn
{
    #pragma pack(push,1)
    typedef struct
    {
        uint32_t save_storage;
        uint32_t save_general;
        uint32_t block_size;
        uint32_t unknown0;
        uint16_t unknown1;
        uint16_t checksum;
    } dppt_footer_t;

    typedef struct
    {
        uint32_t save_num;
        uint32_t block_size;
        uint32_t unknown0;
        uint16_t unknown1;
        uint16_t checksum;
    } hgss_footer_t;

    typedef union
    {
        dppt_footer_t dppt;
        hgss_footer_t hgss;
    } gen4_footer_t;
    #pragma pack(pop)
}

#endif /* INCLUDED_PKMN_CONVERSIONS_STRUCTS_GEN4_SAVE_HPP */
