/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_CONVERSIONS_POKEMON_HPP
#define INCLUDED_PKMN_CONVERSIONS_POKEMON_HPP

#include <pkmn/config.hpp>
#include <pkmn/pokemon.hpp>
#include <pkmn/native/pokemon.hpp>

namespace pkmn
{
    namespace conversions
    {
#ifndef SWIG
        pokemon::sptr PKMN_API import_gen1_pokemon(const native::gen1_pc_pokemon_t &native,
                                                   const uint8_t* nickname_buffer,
                                                   const uint8_t* otname_buffer,
                                                   const pkmn::pkstring &version);

        pokemon::sptr PKMN_API import_gen1_pokemon(const native::gen1_party_pokemon_t &native,
                                                   const uint8_t* nickname_buffer,
                                                   const uint8_t* otname_buffer,
                                                   const pkmn::pkstring &version);

        void PKMN_API export_gen1_pokemon(pokemon::sptr pkmn, native::gen1_pc_pokemon_t &native,
                                          uint8_t* nickname_buffer, uint8_t* otname_buffer);

        void PKMN_API export_gen1_pokemon(pokemon::sptr pkmn, native::gen1_party_pokemon_t &native,
                                          uint8_t* nickname_buffer, uint8_t* otname_buffer);

        pokemon::sptr PKMN_API import_gen2_pokemon(const native::gen2_pc_pokemon_t &native,
                                                   const uint8_t* nickname_buffer,
                                                   const uint8_t* otname_buffer,
                                                   const pkmn::pkstring &version);

        pokemon::sptr PKMN_API import_gen2_pokemon(const native::gen2_party_pokemon_t &native,
                                                   const uint8_t* nickname_buffer,
                                                   const uint8_t* otname_buffer,
                                                   const pkmn::pkstring &version);

        void PKMN_API export_gen2_pokemon(pokemon::sptr pkmn, native::gen2_pc_pokemon_t &native,
                                          uint8_t* nickname_buffer, uint8_t* otname_buffer);

        void PKMN_API export_gen2_pokemon(pokemon::sptr pkmn, native::gen2_party_pokemon_t &native,
                                          uint8_t* nickname_buffer, uint8_t* otname_buffer);

        pokemon::sptr PKMN_API import_gen3_pokemon(const native::gen3_pc_pokemon_t &native,
                                                   const pkmn::pkstring &version,
                                                   bool is_encrypted = true);

        pokemon::sptr PKMN_API import_gen3_pokemon(const native::gen3_party_pokemon_t &native,
                                                   const pkmn::pkstring &version,
                                                   bool is_encrypted = true);

        void PKMN_API export_gen3_pokemon(pokemon::sptr pkmn, native::gen3_pc_pokemon_t &native,
                                          bool encrypt = true);

        void PKMN_API export_gen3_pokemon(pokemon::sptr pkmn, native::gen3_party_pokemon_t &native,
                                          bool encrypt = true);

        pokemon::sptr PKMN_API import_nds_pokemon(const native::nds_pc_pokemon_t &native,
                                                  const pkmn::pkstring &version,
                                                  bool is_encrypted = true);

        pokemon::sptr PKMN_API import_nds_pokemon(const native::nds_party_pokemon_t &native,
                                                  const pkmn::pkstring &version,
                                                  bool is_encrypted = true);

        void PKMN_API export_nds_pokemon(pokemon::sptr pkmn, native::nds_pc_pokemon_t &native,
                                         bool encrypt = true);

        void PKMN_API export_nds_pokemon(pokemon::sptr pkmn, native::nds_party_pokemon_t &native,
                                         bool encrypt = true);

        void PKMN_API gen1_to_gen2(const native::gen1_party_pokemon_t &src, native::gen2_party_pokemon_t &dst);

        void PKMN_API gen2_to_gen1(const native::gen2_party_pokemon_t &src, native::gen1_party_pokemon_t &dst);

        void PKMN_API gen3_to_gen4(const native::gen3_party_pokemon_t &src, native::nds_party_pokemon_t &dst);

        void PKMN_API gen4_to_gen5(const native::nds_party_pokemon_t &src, native::nds_party_pokemon_t &dst);
#endif /* SWIG */

        pokemon::sptr PKMN_API gen1_to_gen2(pokemon::sptr src);

        pokemon::sptr PKMN_API gen2_to_gen1(pokemon::sptr src);

        pokemon::sptr PKMN_API gen3_to_gen4(pokemon::sptr src);

        pokemon::sptr PKMN_API gen4_to_gen5(pokemon::sptr src);
    }
}

#endif /* INCLUDED_PKMN_CONVERSIONS_POKEMON_HPP */
