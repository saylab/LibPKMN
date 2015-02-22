/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_NATIVE_CHECKSUM_HPP
#define INCLUDED_PKMN_NATIVE_CHECKSUM_HPP

#include <pkmn/config.hpp>
#include <pkmn/native/gen3_save.hpp>
#include <pkmn/native/pokemon.hpp>

namespace pkmn
{
    namespace native
    {
        uint16_t PKMN_API gen3_pokemon_checksum(const native::gen3_pokemon_blocks_t &pkmn);

        uint16_t PKMN_API gen3_section_checksum(const native::gen3_save_section_t &section,
                                                int section_num);

        uint16_t PKMN_API nds_pokemon_checksum(const native::nds_pokemon_blocks_t &pkmn);

        uint16_t PKMN_API nds_block_checksum(const uint8_t* block, int len);

        uint16_t PKMN_API gen6_pokemon_checksum(const native::gen6_pokemon_blocks_t &pkmn);
    }
}
#endif /* INCLUDED_PKMN_NATIVE_CHECKSUM_HPP */
