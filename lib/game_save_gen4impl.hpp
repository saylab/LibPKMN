/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_GEN4IMPL_HPP
#define INCLUDED_GAME_SAVE_GEN4IMPL_HPP

#include <pkmn/config.hpp>
#include <pkmn/enums.hpp>

#include "game_save_impl.hpp"

#include "conversions/pokemon.hpp"
#include "conversions/structs/gen4_save.hpp"

namespace pkmn
{
    typedef enum
    {
        GEN4_NONE,
        DP,
        PLAT,
        HGSS,
    } gen4_games_t;

    typedef enum
    {
        MAIN,
        BACKUP
    } gen4_positions_t;

    typedef enum
    {
        A,
        B
    } gen4_blocks_t;

    typedef enum
    {
        gen4_blockA,
        gen4_blockB,
        gen4_footerA,
        gen4_footerB,
        gen4_party,
        gen4_items,
        gen4_name,
        gen4_tid,
        gen4_money,
        gen4_gender,
        gen4_country,
        gen4_badges1,
        gen4_badges2,
        gen4_rivalname
    } gen4_offsets_t;

    static uint32_t gen4_offsets[14][4] =
    {
        {0x0,     0x0,     0x0,     0x0}, // Block A
        {0x0,  0xC100,  0xCF2C,  0xF700}, // Block B
        {0x0,  0xC0EC,  0xCF18,  0xF618}, // Footer A
        {0x0, 0x1E2CC, 0x1F0FC, 0x21A00}, // Footer B
        {0x0,    0x98,    0xA0,    0x98}, // Party
        {0x0,   0x624,   0x630,   0x644}, // Items
        {0x0,  0x0064,  0x0068,  0x0064}, // Trainer Name
        {0x0,  0x0074,  0x0078,  0x0074}, // Trainer ID
        {0x0,  0x0078,  0x007C,  0x0078}, // Money
        {0x0,  0x007C,  0x0080,  0x007C}, // Gender
        {0x0,  0x007D,  0x0081,  0x007D}, // Country
        {0x0,  0x007E,  0x0082,  0x007E}, // Badges
        {0x0,  0x007E,  0x0082,  0x0083}, // Badges 2 (HGSS Kanto only)
        {0x0,  0x25A8,  0x27E8,  0x22D4}  // Rival Name
    };

    static uint32_t gen4_block_sizes[4][2] =
    {
        {   0x0,    0x0},
        {0xC100,0x121E0},
        {0xCF2C,0x121E4},
        {0xF628,0x12310}
    };

    static uint8_t gen4_footer_sizes[4] = {0x0,0x14,0x14,0x10};

    static uint8_t libpkmn_version_ids[3] =
    {
        Versions::DIAMOND,
        Versions::PLATINUM,
        Versions::HEARTGOLD
    };

    gen4_games_t PKMN_INLINE get_gen4_save_type(std::vector<uint8_t> &data)
    {
        uint32_t* data32 = reinterpret_cast<uint32_t*>(&data);

        if(data32[0xC0F4/4] == gen4_block_sizes[DP][A])   return DP;
        if(data32[0xCF20/4] == gen4_block_sizes[PLAT][A]) return PLAT;
        if(data32[0xF61C/4] == gen4_block_sizes[HGSS][A]) return HGSS;
        return GEN4_NONE;
    }

    class game_save_gen4impl: public game_save_impl
    {
        public:

            game_save_gen4impl();
            game_save_gen4impl(const pkmn::pkstring &filename, gen4_games_t game, bool small);

            void load();
            void save_as(const pkmn::pkstring &filename);

        private:

            gen4_games_t _game_type;
            uint8_t      _game_id;
            bool         _small;

            uint8_t*       _blocks[2];
            gen4_footer_t* _footers[2];

            nds_pokemon_party_t* _party;
            void*                _items;
    };
}

#endif /* INCLUDED_GAME_SAVE_GEN4IMPL_HPP */
