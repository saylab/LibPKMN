/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_NATIVE_CRYPT_HPP
#define INCLUDED_PKMN_NATIVE_CRYPT_HPP

#include <pkmn/config.hpp>
#include <pkmn/native/pokemon.hpp>

namespace pkmn
{
    namespace native
    {
        void PKMN_API gen3_encrypt(native::gen3_pc_pokemon_t &pkmn);

        void PKMN_API gen3_decrypt(native::gen3_pc_pokemon_t &pkmn);

        void PKMN_API nds_encrypt(native::nds_pc_pokemon_t &pkmn);

        void PKMN_API nds_decrypt(native::nds_pc_pokemon_t &pkmn);

        void PKMN_API nds_encrypt(native::nds_party_pokemon_t &pkmn);

        void PKMN_API nds_decrypt(native::nds_party_pokemon_t &pkmn);

        void PKMN_API gen6_encrypt(native::gen6_pc_pokemon_t &pkmn);

        void PKMN_API gen6_decrypt(native::gen6_pc_pokemon_t &pkmn);

        void PKMN_API gen6_encrypt(native::gen6_party_pokemon_t &pkmn);

        void PKMN_API gen6_decrypt(native::gen6_party_pokemon_t &pkmn);
    }
}
#endif /* INCLUDED_PKMN_NATIVE_CRYPT_HPP */
