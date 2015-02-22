/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cstddef>
#include <cstring>

#include <pkmn/enums.hpp>
#include <pkmn/native/crypt.hpp>
#include <pkmn/native/pokemon.hpp>
#include <pkmn/types/prng.hpp>

namespace pkmn
{
    namespace native
    {
        /*
         * Generation III
         */

        enum gen3_substructs
        {
            G, //Growth
            A, //Attacks
            E, //Effort
            M  //Misc
        };

        static const uint8_t gen3_block_orders[24][4] =
        {
            {G,A,E,M},{G,A,M,E},{G,E,A,M},{G,E,M,A},{G,M,A,E},{G,M,E,A},
            {A,G,E,M},{A,G,M,E},{A,E,G,M},{A,E,M,G},{A,M,G,E},{A,M,E,G},
            {E,G,A,M},{E,G,M,A},{E,A,G,M},{E,A,M,G},{E,M,G,A},{E,M,A,G},
            {M,G,A,E},{M,G,E,A},{M,A,G,E},{M,A,E,G},{M,E,G,A},{M,E,A,G}
        };

        static void gen3_crypt(native::gen3_pc_pokemon_t &pkmn, bool encrypt)
        {
            uint32_t security_key = pkmn.ot_id ^ pkmn.personality;
            for(size_t i = 0; i < 12; i++) pkmn.blocks.blocks32[i] ^= security_key;

            uint8_t index = pkmn.personality % 24;
            uint8_t growth_index  = gen3_block_orders[index][G];
            uint8_t attacks_index = gen3_block_orders[index][A];
            uint8_t effort_index  = gen3_block_orders[index][E];
            uint8_t misc_index    = gen3_block_orders[index][M];

            native::gen3_pokemon_blocks_t blocks;
            if(encrypt)
            {
                memcpy(&blocks.blocks[growth_index][0], &pkmn.blocks.growth, sizeof(native::gen3_pokemon_growth_t));
                memcpy(&blocks.blocks[attacks_index][1], &pkmn.blocks.attacks, sizeof(native::gen3_pokemon_attacks_t));
                memcpy(&blocks.blocks[effort_index][2], &pkmn.blocks.effort, sizeof(native::gen3_pokemon_effort_t));
                memcpy(&blocks.blocks[misc_index][3], &pkmn.blocks.misc, sizeof(native::gen3_pokemon_misc_t));
            }
            else
            {
                memcpy(&blocks.growth, &pkmn.blocks.blocks[growth_index][0], sizeof(native::gen3_pokemon_growth_t));
                memcpy(&blocks.attacks, &pkmn.blocks.blocks[attacks_index][1], sizeof(native::gen3_pokemon_attacks_t));
                memcpy(&blocks.effort, &pkmn.blocks.blocks[effort_index][2], sizeof(native::gen3_pokemon_effort_t));
                memcpy(&blocks.misc, &pkmn.blocks.blocks[misc_index][3], sizeof(native::gen3_pokemon_misc_t));
            }

            pkmn.blocks = blocks;
        }

        void gen3_encrypt(native::gen3_pc_pokemon_t &pkmn)
        {
            gen3_crypt(pkmn, true);
        }

        void gen3_decrypt(native::gen3_pc_pokemon_t &pkmn)
        {
            gen3_crypt(pkmn, false);
        }

        /*
         * Generation IV-V
         */

        enum nds_blocks
        {
            bA,
            bB,
            bC,
            bD
        };

        static const int modern_block_orders[24][4] = {
            {bA,bB,bC,bD}, {bA,bB,bD,bC}, {bA,bC,bB,bD}, {bA,bC,bD,bB},
            {bA,bD,bB,bC}, {bA,bD,bC,bB}, {bB,bA,bC,bD}, {bB,bA,bD,bC},
            {bB,bC,bA,bD}, {bB,bC,bD,bA}, {bB,bD,bA,bC}, {bB,bD,bC,bA},
            {bC,bA,bB,bD}, {bC,bA,bD,bB}, {bC,bB,bA,bD}, {bC,bB,bD,bA},
            {bC,bD,bA,bB}, {bC,bD,bB,bA}, {bD,bA,bB,bC}, {bD,bA,bC,bB},
            {bD,bB,bA,bC}, {bD,bB,bC,bA}, {bD,bC,bA,bB}, {bD,bC,bB,bA}
        };

        static void nds_crypt(native::nds_pc_pokemon_t &pkmn, bool encrypt)
        {
            prng::sptr rng = prng::make(4);
            rng->seed_lcrng(pkmn.checksum);
            native::nds_pokemon_blocks_t blocks;

            uint8_t index = ((pkmn.personality & 0x3E000) >> 0xD) % 24;
            uint8_t blockA_index = modern_block_orders[index][bA];
            uint8_t blockB_index = modern_block_orders[index][bB];
            uint8_t blockC_index = modern_block_orders[index][bC];
            uint8_t blockD_index = modern_block_orders[index][bD];

            if(encrypt)
            {
                for(size_t i = 0; i < 64; i++)
                    pkmn.blocks.blocks16[i] ^= rng->lcrng();

                memcpy(&blocks.blocks[blockA_index][0], &pkmn.blocks.blockA, sizeof(native::nds_pokemon_blockA_t));
                memcpy(&blocks.blocks[blockB_index][1], &pkmn.blocks.blockB, sizeof(native::nds_pokemon_blockB_t));
                memcpy(&blocks.blocks[blockC_index][2], &pkmn.blocks.blockC, sizeof(native::nds_pokemon_blockC_t));
                memcpy(&blocks.blocks[blockD_index][3], &pkmn.blocks.blockD, sizeof(native::nds_pokemon_blockD_t));
            }
            else
            {
                memcpy(&blocks.blockA, &pkmn.blocks.blocks[blockA_index][0], sizeof(native::nds_pokemon_blockA_t));
                memcpy(&blocks.blockB, &pkmn.blocks.blocks[blockB_index][1], sizeof(native::nds_pokemon_blockB_t));
                memcpy(&blocks.blockC, &pkmn.blocks.blocks[blockC_index][2], sizeof(native::nds_pokemon_blockC_t));
                memcpy(&blocks.blockD, &pkmn.blocks.blocks[blockD_index][3], sizeof(native::nds_pokemon_blockD_t));

                for(size_t i = 0; i < 64; i++)
                    pkmn.blocks.blocks16[i] ^= rng->lcrng();
            }

            pkmn.blocks = blocks;
        }

        static void nds_crypt(native::nds_party_pokemon_t &pkmn, bool encrypt)
        {
            nds_crypt(pkmn.pc, encrypt);

            prng::sptr rng = prng::make(4);
            rng->seed_lcrng(pkmn.pc.personality);

            uint16_t* pkmn_bytes = reinterpret_cast<uint16_t*>(&pkmn);
            for(size_t i = offsetof(native::nds_party_pokemon_t, status); i < sizeof(native::nds_party_pokemon_t); i++)
                pkmn_bytes[i] ^= rng->lcrng();
        }

        void nds_encrypt(native::nds_pc_pokemon_t &pkmn)
        {
            nds_crypt(pkmn, true);
        }

        void nds_decrypt(native::nds_pc_pokemon_t &pkmn)
        {
            nds_crypt(pkmn, false);
        }

        void nds_encrypt(native::nds_party_pokemon_t &pkmn)
        {
            nds_crypt(pkmn, true);
        }

        void nds_decrypt(native::nds_party_pokemon_t &pkmn)
        {
            nds_crypt(pkmn, false);
        }
    }
}
