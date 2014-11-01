/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_GEN1IMPL_HPP
#define INCLUDED_GAME_SAVE_GEN1IMPL_HPP

#include <pkmn/config.hpp>

#include "game_save_impl.hpp"

#include "conversions/items.hpp"
#include "conversions/pokemon.hpp"
#include "conversions/trainer.hpp"

namespace pkmn
{
    /*
     * Source: http://bulbapedia.bulbagarden.net/wiki/Save_data_structure_in_Generation_I#File_structure
     */
    namespace gen1_offsets
    {
        enum offsets
        {
            PLAYER_NAME             = 0x2598,
            POKEDEX_OWNED           = 0x25A3,
            POKEDEX_SEEN            = 0x25B6,
            ITEM_BAG                = 0x25C9,
            MONEY                   = 0x25F3,
            RIVAL_NAME              = 0x25F6,
            PLAYER_ID               = 0x2605,
            PIKACHU_FRIENDSHIP      = 0x271C,
            ITEM_PC                 = 0x27E6,
            CURRENT_POKEMON_BOX_NUM = 0x284C,
            TIME_PLAYED             = 0x2CEE,
            POKEMON_PARTY           = 0x2F2C,
            CURRENT_POKEMON_BOX     = 0x30C0,
            CHECKSUM                = 0x3523,
            POKEMON_PC              = 0x4000
        };
    }

    /*
     * Source: http://bulbapedia.bulbagarden.net/wiki/Save_data_structure_in_Generation_I#Checksum
     */
    bool PKMN_INLINE gen1_check(const std::vector<uint8_t> &data)
    {
        uint8_t checksum = 255;
        for(size_t i = 0x2598; i < gen1_offsets::CHECKSUM; i++) checksum -= data[i];

        return (checksum == data[gen1_offsets::CHECKSUM]);
    }

    class game_save_gen1impl: public game_save_impl
    {
        public:

            game_save_gen1impl();
            game_save_gen1impl(const std::string &filename);

            void load();
            void save_as(const std::string &filename);
            bool check();

        private:

            gen1_item_bag_t* _item_bag;
            gen1_item_pc_t* _item_pc;
            gen1_pokemon_party_t* _pokemon_party;
            gen1_pokemon_pc_t* _pokemon_pc;
    };
}
#endif /* INCLUDED_GAME_SAVE_GEN1IMPL_HPP */
