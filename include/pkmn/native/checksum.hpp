/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_NATIVE_CHECKSUM_HPP
#define INCLUDED_PKMN_NATIVE_CHECKSUM_HPP

#include <cstdint>
#include <utility>
#include <vector>

#include <pkmn/config.hpp>
#include <pkmn/native/gen3_save.hpp>
#include <pkmn/native/pokemon.hpp>

namespace pkmn
{
    namespace native
    {
        uint8_t PKMN_API get_gen1_save_checksum(const std::vector<uint8_t> &data);

        void PKMN_API set_gen1_save_checksum(std::vector<uint8_t> &data);

        std::pair<uint16_t, uint16_t> get_gen2_save_checksums(const std::vector<uint8_t> &data,
                                                              bool crystal);

        uint16_t PKMN_API get_gen3_pokemon_checksum(const native::gen3_pokemon_blocks_t &blocks);

        void PKMN_API set_gen3_pokemon_checksum(native::gen3_pc_pokemon_t &pkmn);

        uint16_t PKMN_API get_gen3_section_checksum(const native::gen3_save_section_t &section,
                                                    int section_num);

        void PKMN_API set_gen3_section_checksum(native::gen3_save_section_t &section,
                                                int section_num);

        void PKMN_API set_gen3_save_checksums(native::gen3_save_t* save);

        uint16_t PKMN_API get_nds_pokemon_checksum(const native::nds_pokemon_blocks_t &blocks);

        void PKMN_API set_nds_pokemon_checksum(native::nds_pc_pokemon_t &pkmn);

        uint16_t PKMN_API get_nds_block_checksum(const uint8_t* block, int len);

        uint16_t PKMN_API get_gen6_pokemon_checksum(const native::gen6_pokemon_blocks_t &pkmn);

        void PKMN_API set_gen6_pokemon_checksum(native::gen6_pc_pokemon_t &pkmn);
    }
}
#endif /* INCLUDED_PKMN_NATIVE_CHECKSUM_HPP */
