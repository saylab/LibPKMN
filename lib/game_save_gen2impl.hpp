/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_GEN2IMPL_HPP
#define INCLUDED_GAME_SAVE_GEN2IMPL_HPP

#include <pkmn/config.hpp>

#include "game_save_impl.hpp"

#include <pkmn/native/checksum.hpp>
#include <pkmn/native/items.hpp>
#include <pkmn/native/pokemon.hpp>

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

    enum offsets
    {
        GEN2_PLAYER_ID,
        GEN2_PLAYER_NAME,
        GEN2_RIVAL_NAME,
        GEN2_DAYLIGHT_SAVINGS,
        GEN2_TIME_PLAYED,
        GEN2_MONEY,
        GEN2_ITEM_BAG,
        GEN2_ITEM_PC,
        GEN2_CURRENT_POKEMON_BOX_NUM,
        GEN2_PC_BOX_NAMES,
        GEN2_POKEMON_PARTY,
        GEN2_POKEDEX_OWNED,
        GEN2_POKEDEX_SEEN,
        GEN2_CURRENT_POKEMON_BOX,
        GEN2_PLAYER_GENDER,
        GEN2_POKEMON_PC,
        GEN2_CHECKSUM1,
        GEN2_CHECKSUM2
    };

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
        std::pair<uint16_t, uint16_t> checksums = native::get_gen2_save_checksums(data, false);

        return ((checksums.first == *reinterpret_cast<const uint16_t*>(&data[offsets[0][GEN2_CHECKSUM1]])) and
                (checksums.second == *reinterpret_cast<const uint16_t*>(&data[offsets[0][GEN2_CHECKSUM2]])));
    }

    bool PKMN_INLINE crystal_check(const std::vector<uint8_t> &data)
    {
        std::pair<uint16_t, uint16_t> checksums = native::get_gen2_save_checksums(data, true);

        return ((checksums.first == *reinterpret_cast<const uint16_t*>(&data[offsets[1][GEN2_CHECKSUM1]])) and
                (checksums.second == *reinterpret_cast<const uint16_t*>(&data[offsets[1][GEN2_CHECKSUM2]])));
    }

    class game_save_gen2impl: public game_save_impl
    {
        public:

            game_save_gen2impl(const pkmn::pkstring& filename, bool crystal);

            void load();
            void save_as(const pkmn::pkstring& filename);
            bool check();

        private:

            uint16_t _player_id_offset, _player_name_offset, _rival_name_offset;
            uint16_t _daylight_savings_offset, _time_played_offset, _money_offset;
            uint16_t _item_bag_offset, _item_pc_offset, _current_pokemon_box_num_offset;
            uint16_t _pc_box_names_offset, _pokemon_party_offset, _pokedex_owned_offset;
            uint16_t _pokedex_seen_offset, _current_pokemon_box_offset, _player_gender_offset;
            uint16_t _pokemon_pc_offset, _checksum1_offset, _checksum2_offset;

            native::gen2_item_bag_t*      _item_bag;
            native::gen2_item_pc_t*       _item_pc;
            native::gen2_pokemon_party_t* _pokemon_party;
            native::gen2_pokemon_pc_t*    _pokemon_pc;
    };
}
#endif /* INCLUDED_GAME_SAVE_GEN2IMPL_HPP */
