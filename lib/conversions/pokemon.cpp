/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifdef _MSC_VER
#pragma warning(disable:4800) //Type-narrowing is inevitable
#endif

#include <cstring>
#include <vector>

#include <boost/assign.hpp>
#include <boost/format.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database.hpp>
#include <pkmn/conversions/pokemon.hpp>
#include <pkmn/conversions/text.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/prng.hpp>

#include "pokemon_gen1impl.hpp"
#include "pokemon_gen2impl.hpp"
#include "pokemon_gen3impl.hpp"
#include "pokemon_ndsimpl.hpp"

#include "utils.hpp"
#include "../SQLiteCpp/SQLiteC++.h"

namespace pkmn
{
    namespace conversions
    {
        pokemon::sptr import_gen1_pokemon(const native::gen1_pc_pokemon_t& native,
                                          const uint8_t* nickname_buffer,
                                          const uint8_t* otname_buffer,
                                          const pkmn::pkstring& version)
        {
            return pokemon::sptr(new pokemon_gen1impl(native,
                                                      import_gen1_text(nickname_buffer, 10),
                                                      import_gen1_text(otname_buffer, 7),
                                                      database::get_version_id(version))
                                );
        }

        pokemon::sptr import_gen1_pokemon(const native::gen1_party_pokemon_t& native,
                                          const uint8_t* nickname_buffer,
                                          const uint8_t* otname_buffer,
                                          const pkmn::pkstring& version)
        {
            return pokemon::sptr(new pokemon_gen1impl(native,
                                                      import_gen1_text(nickname_buffer, 10),
                                                      import_gen1_text(otname_buffer, 7),
                                                      database::get_version_id(version))
                                );
        }

        void export_gen1_pokemon(pokemon::sptr pkmn, native::gen1_pc_pokemon_t& native,
                                 uint8_t* nickname_buffer, uint8_t* otname_buffer)
        {
            memcpy(&native, pkmn->get_native(), sizeof(native::gen1_pc_pokemon_t));
            export_gen1_text(pkmn->get_nickname(), nickname_buffer, 10);
            export_gen1_text(pkmn->get_trainer_name(), otname_buffer, 7);
        }

        void export_gen1_pokemon(pokemon::sptr pkmn, native::gen1_party_pokemon_t& native,
                                 uint8_t* nickname_buffer, uint8_t* otname_buffer)
        {
            memcpy(&native, pkmn->get_native(), sizeof(native::gen1_party_pokemon_t));
            export_gen1_text(pkmn->get_nickname(), nickname_buffer, 10);
            export_gen1_text(pkmn->get_trainer_name(), otname_buffer, 7);
        }

        pokemon::sptr import_gen2_pokemon(const native::gen2_pc_pokemon_t& native,
                                          const uint8_t* nickname_buffer,
                                          const uint8_t* otname_buffer,
                                          const pkmn::pkstring& version)
        {
            return pokemon::sptr(new pokemon_gen2impl(native,
                                                      import_gen2_text(nickname_buffer, 10),
                                                      import_gen2_text(otname_buffer, 7),
                                                      database::get_version_id(version))
                                );
        }

        pokemon::sptr import_gen2_pokemon(const native::gen2_party_pokemon_t& native,
                                          const uint8_t* nickname_buffer,
                                          const uint8_t* otname_buffer,
                                          const pkmn::pkstring& version)
        {
            return pokemon::sptr(new pokemon_gen2impl(native,
                                                      import_gen2_text(nickname_buffer, 10),
                                                      import_gen2_text(otname_buffer, 7),
                                                      database::get_version_id(version))
                                );
        }

        void export_gen2_pokemon(pokemon::sptr pkmn, native::gen2_pc_pokemon_t& native,
                                 uint8_t* nickname_buffer, uint8_t* otname_buffer)
        {
            memcpy(&native, pkmn->get_native(), sizeof(native::gen2_pc_pokemon_t));
            export_gen2_text(pkmn->get_nickname(), nickname_buffer, 10);
            export_gen2_text(pkmn->get_trainer_name(), otname_buffer, 7);
        }

        void export_gen2_pokemon(pokemon::sptr pkmn, native::gen2_party_pokemon_t& native,
                                 uint8_t* nickname_buffer, uint8_t* otname_buffer)
        {
            memcpy(&native, pkmn->get_native(), sizeof(native::gen2_party_pokemon_t));
            export_gen2_text(pkmn->get_nickname(), nickname_buffer, 10);
            export_gen2_text(pkmn->get_trainer_name(), otname_buffer, 7);
        }

        pokemon::sptr import_gen3_pokemon(const native::gen3_pc_pokemon_t& native,
                                          const pkmn::pkstring& version,
                                          bool is_encrypted)
        {
            native::gen3_pc_pokemon_t _native = native;
            if(is_encrypted) gen3_crypt(_native);

            return pokemon::sptr(new pokemon_gen3impl(_native,
                                                      database::get_version_id(version)));
        }

        pokemon::sptr import_gen3_pokemon(const native::gen3_party_pokemon_t& native,
                                          const pkmn::pkstring& version,
                                          bool is_encrypted)
        {
            native::gen3_party_pokemon_t _native = native;
            if(is_encrypted) gen3_crypt(_native.pc);

            return pokemon::sptr(new pokemon_gen3impl(_native,
                                                      database::get_version_id(version)));
        }

        void export_gen3_pokemon(pokemon::sptr pkmn, native::gen3_pc_pokemon_t& native,
                                 bool encrypt)
        {
            memcpy(&native, pkmn->get_native(), sizeof(native::gen3_pc_pokemon_t));
            if(encrypt) native.checksum = gen3_crypt(native);
        }

        void export_gen3_pokemon(pokemon::sptr pkmn, native::gen3_party_pokemon_t& native,
                                 bool encrypt)
        {
            memcpy(&native, pkmn->get_native(), sizeof(native::gen3_party_pokemon_t));
            if(encrypt) native.pc.checksum = gen3_crypt(native.pc);
        }

        pokemon::sptr import_nds_pokemon(const native::nds_pc_pokemon_t& native,
                                         const pkmn::pkstring& version,
                                         bool is_encrypted)
        {
            // TODO: crypt
            native::nds_pc_pokemon_t _native = native;

            return pokemon::sptr(new pokemon_ndsimpl(_native,
                                                      database::get_version_id(version)));
        }

        pokemon::sptr import_nds_pokemon(const native::nds_party_pokemon_t& native,
                                         const pkmn::pkstring& version,
                                         bool is_encrypted)
        {
            // TODO: crypt
            native::nds_party_pokemon_t _native = native;

            return pokemon::sptr(new pokemon_ndsimpl(_native,
                                                      database::get_version_id(version)));
        }

        void export_nds_pokemon(pokemon::sptr pkmn, native::nds_pc_pokemon_t& native,
                                bool encrypt)
        {
            // TODO: crypt
            memcpy(&native, pkmn->get_native(), sizeof(native::nds_pc_pokemon_t));
        }

        void export_nds_pokemon(pokemon::sptr pkmn, native::nds_party_pokemon_t& native,
                                bool encrypt)
        {
            // TODO: crypt
            memcpy(&native, pkmn->get_native(), sizeof(native::nds_party_pokemon_t));
        }
    } /* namespace conversions */
} /* namespace pkmn */
