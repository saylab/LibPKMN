/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_GEN1IMPL_HPP
#define INCLUDED_GAME_SAVE_GEN1IMPL_HPP

#include <pkmn/config.hpp>

#include "game_save_impl.hpp"

#include <pkmn/native/checksum.hpp>
#include <pkmn/native/items.hpp>
#include <pkmn/native/pokemon.hpp>

namespace pkmn
{
    /*
     * Source: http://bulbapedia.bulbagarden.net/wiki/Save_data_structure_in_Generation_I#File_structure
     */
    enum gen1_offsets
    {
        GEN1_PLAYER_NAME             = 0x2598,
        GEN1_POKEDEX_OWNED           = 0x25A3,
        GEN1_POKEDEX_SEEN            = 0x25B6,
        GEN1_ITEM_BAG                = 0x25C9,
        GEN1_MONEY                   = 0x25F3,
        GEN1_RIVAL_NAME              = 0x25F6,
        GEN1_PLAYER_ID               = 0x2605,
        GEN1_PIKACHU_FRIENDSHIP      = 0x271C,
        GEN1_ITEM_PC                 = 0x27E6,
        GEN1_CURRENT_POKEMON_BOX_NUM = 0x284C,
        GEN1_TIME_PLAYED             = 0x2CEE,
        GEN1_POKEMON_PARTY           = 0x2F2C,
        GEN1_CURRENT_POKEMON_BOX     = 0x30C0,
        GEN1_CHECKSUM                = 0x3523,
        GEN1_POKEMON_PC              = 0x4000
    };

    /*
     * Source: http://bulbapedia.bulbagarden.net/wiki/Save_data_structure_in_Generation_I#Checksum
     */
    bool PKMN_INLINE gen1_check(const std::vector<uint8_t> &data)
    {
        return (*reinterpret_cast<const uint16_t*>(&data[GEN1_CHECKSUM]) == native::get_gen1_save_checksum(data));
    }

    class game_save_gen1impl: public game_save_impl
    {
        public:

            game_save_gen1impl();
            game_save_gen1impl(const pkmn::pkstring &filename);

            void load();
            void save_as(const pkmn::pkstring &filename);
            bool check();

        private:

            native::gen1_item_bag_t*      _item_bag;
            native::gen1_item_pc_t*       _item_pc;
            native::gen1_pokemon_party_t* _pokemon_party;
            native::gen1_pokemon_pc_t*    _pokemon_pc;
    };
}
#endif /* INCLUDED_GAME_SAVE_GEN1IMPL_HPP */
