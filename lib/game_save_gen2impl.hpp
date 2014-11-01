/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_GEN2IMPL_HPP
#define INCLUDED_GAME_SAVE_GEN2IMPL_HPP

#include <pkmn/config.hpp>

#include "game_save_impl.hpp"

#include "conversions/items.hpp"
#include "conversions/pokemon.hpp"
#include "conversions/trainer.hpp"

namespace pkmn
{
    namespace gen2_games
    {
        enum games
        {
            GS,
            CRYSTAL
        };
    }

    namespace gen2_offsets
    {
        enum offsets
        {
            PLAYER_ID,
            PLAYER_NAME,
            RIVAL_NAME,
            DAYLIGHT_SAVINGS,
            TIME_PLAYED,
            MONEY,
            ITEM_BAG,
            ITEM_PC,
            CURRENT_POKEMON_BOX_NUM,
            PC_BOX_NAMES,
            POKEMON_PARTY,
            POKEDEX_OWNED,
            POKEDEX_SEEN,
            CURRENT_POKEMON_BOX,
            PLAYER_GENDER,
            POKEMON_PC,
            CHECKSUM1,
            CHECKSUM2
        };
    }

    /*
     * Source: http://bulbapedia.bulbagarden.net/wiki/Save_data_structure_in_Generation_II#File_structure
     */
    static const uint16_t offsets[2][18] =
    {
        {0x2009,0x200B,0x2021,0x2050,0x2053,0x23DB,0x23E6,0x247E,0x2724,0x2727,0x288A,0x2A4C,0x2A6C,0x2D6C,0x3E3D,0x4000,0x2D69,0x7E6D},
        {0x2009,0x200B,0x2021,0x2050,0x2054,0x23Db,0x23E7,0x247F,0x2700,0x2703,0x2865,0x2A27,0x2A47,0x2D11,0x3E3D,0x4000,0x2D0D,0x1F0D}
    };

    /*
     * Source: http://bulbapedia.bulbagarden.net/wiki/Save_data_structure_in_Generation_II#Checksums
     */
    bool PKMN_INLINE gs_check(const std::vector<uint8_t> &data)
    {
        uint16_t checksum1 = 0;
        uint16_t checksum2 = 0;

        //Checksum 1
        for(size_t i = 0x2009; i <= 0x2D68; i++) checksum1 += data[i];

        //Checksum 2
        for(size_t i = 0x0C6B; i <= 0x17EC; i++) checksum2 += data[i];
        for(size_t i = 0x3D96; i <= 0x3F3F; i++) checksum2 += data[i];
        for(size_t i = 0x7E39; i <= 0x7E6C; i++) checksum2 += data[i];

        return ((checksum1 == *reinterpret_cast<const uint16_t*>(&data[offsets[0][gen2_offsets::CHECKSUM1]])) and 
                (checksum2 == *reinterpret_cast<const uint16_t*>(&data[offsets[0][gen2_offsets::CHECKSUM2]])));
    }

    bool PKMN_INLINE crystal_check(const std::vector<uint8_t> &data)
    {
        uint16_t checksum1 = 0;
        uint16_t checksum2 = 0;

        //Checksum 1
        for(size_t i = 0x2009; i <= 0x2B82; i++) checksum1 += data[i];

        //Checksum 2
        for(size_t i = 0x1209; i <= 0x1D82; i++) checksum2 += data[i];

        return ((checksum1 == *reinterpret_cast<const uint16_t*>(&data[offsets[1][gen2_offsets::CHECKSUM1]])) and 
                (checksum2 == *reinterpret_cast<const uint16_t*>(&data[offsets[1][gen2_offsets::CHECKSUM2]])));
    }

    class game_save_gen2impl: public game_save_impl
    {
        public:

            game_save_gen2impl(const std::string &filename, bool crystal);

            void load();
            void save_as(const std::string &filename);
            bool check();

        private:

            uint16_t _player_id_offset, _player_name_offset, _rival_name_offset;
            uint16_t _daylight_savings_offset, _time_played_offset, _money_offset;
            uint16_t _item_bag_offset, _item_pc_offset, _current_pokemon_box_num_offset;
            uint16_t _pc_box_names_offset, _pokemon_party_offset, _pokedex_owned_offset;
            uint16_t _pokedex_seen_offset, _current_pokemon_box_offset, _player_gender_offset;
            uint16_t _pokemon_pc_offset, _checksum1_offset, _checksum2_offset;

            gen2_item_bag_t* _item_bag;
            gen2_item_pc_t* _item_pc;
            gen2_pokemon_party_t* _pokemon_party;
            gen2_pokemon_pc_t* _pokemon_pc;
    };
}
#endif /* INCLUDED_GAME_SAVE_GEN2IMPL_HPP */
