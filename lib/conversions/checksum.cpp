/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cstdlib>

#include <pkmn/conversions/checksum.hpp>
#include <pkmn/native/pokemon.hpp>

namespace pkmn
{
    namespace conversions
    {
        uint16_t gen3_pokemon_checksum(const native::gen3_pokemon_blocks_t &blocks)
        {
            uint16_t checksum = 0;

            for(size_t i = 0; i < sizeof(blocks)/2; i++)
                checksum += blocks.blocks16[i];

            return checksum;
        }

        uint16_t nds_pokemon_checksum(const native::nds_pokemon_blocks_t &blocks)
        {
            uint32_t checksum = 0;

            for(size_t i = 0; i < sizeof(blocks)/2; i++)
                checksum += blocks.blocks16[i];

            return uint16_t(checksum & 0xFFFF);
        }
    }
}
