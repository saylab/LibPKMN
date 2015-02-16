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

#include <boost/format.hpp>

#include <pkmn/calculations.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database.hpp>
#include <pkmn/pokedex/pokemon_entry.hpp>
#include <pkmn/conversions/pokemon.hpp>
#include <pkmn/conversions/text.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include "pokemon_gen1impl.hpp"
#include "pokemon_gen2impl.hpp"
#include "pokemon_gen3impl.hpp"
#include "pokemon_ndsimpl.hpp"

#include "internal.hpp"
#include "utils.hpp"
#include "../SQLiteCpp/SQLiteC++.h"

#define CONVERT_POKEMON_GAME_INDEX(src,src_id,dst_id) database::get_pokemon_game_index( \
                                                          database::get_pokemon_id(src_id, src), dst_id)

#define GET_SPECIES_INDEX(src,src_id) database::get_species_id(database::get_pokemon_id(src_id, src))

namespace pkmn
{
    namespace conversions
    {
        pkmn::shared_ptr<SQLite::Database> db;

        pokemon::sptr import_gen1_pokemon(const native::gen1_pc_pokemon_t &native,
                                          const uint8_t* nickname_buffer,
                                          const uint8_t* otname_buffer,
                                          const pkmn::pkstring &version)
        {
            return pokemon::sptr(new pokemon_gen1impl(native,
                                                      import_gen1_text(nickname_buffer, 10),
                                                      import_gen1_text(otname_buffer, 7),
                                                      database::get_version_id(version))
                                );
        }

        pokemon::sptr import_gen1_pokemon(const native::gen1_party_pokemon_t &native,
                                          const uint8_t* nickname_buffer,
                                          const uint8_t* otname_buffer,
                                          const pkmn::pkstring &version)
        {
            return pokemon::sptr(new pokemon_gen1impl(native,
                                                      import_gen1_text(nickname_buffer, 10),
                                                      import_gen1_text(otname_buffer, 7),
                                                      database::get_version_id(version))
                                );
        }

        void export_gen1_pokemon(pokemon::sptr pkmn, native::gen1_pc_pokemon_t &native,
                                 uint8_t* nickname_buffer, uint8_t* otname_buffer)
        {
            memcpy(&native, pkmn->get_native(), sizeof(native::gen1_pc_pokemon_t));
            export_gen1_text(pkmn->get_nickname(), nickname_buffer, 10);
            export_gen1_text(pkmn->get_trainer_name(), otname_buffer, 7);
        }

        void export_gen1_pokemon(pokemon::sptr pkmn, native::gen1_party_pokemon_t &native,
                                 uint8_t* nickname_buffer, uint8_t* otname_buffer)
        {
            memcpy(&native, pkmn->get_native(), sizeof(native::gen1_party_pokemon_t));
            export_gen1_text(pkmn->get_nickname(), nickname_buffer, 10);
            export_gen1_text(pkmn->get_trainer_name(), otname_buffer, 7);
        }

        pokemon::sptr import_gen2_pokemon(const native::gen2_pc_pokemon_t &native,
                                          const uint8_t* nickname_buffer,
                                          const uint8_t* otname_buffer,
                                          const pkmn::pkstring &version)
        {
            return pokemon::sptr(new pokemon_gen2impl(native,
                                                      import_gen2_text(nickname_buffer, 10),
                                                      import_gen2_text(otname_buffer, 7),
                                                      database::get_version_id(version))
                                );
        }

        pokemon::sptr import_gen2_pokemon(const native::gen2_party_pokemon_t &native,
                                          const uint8_t* nickname_buffer,
                                          const uint8_t* otname_buffer,
                                          const pkmn::pkstring &version)
        {
            return pokemon::sptr(new pokemon_gen2impl(native,
                                                      import_gen2_text(nickname_buffer, 10),
                                                      import_gen2_text(otname_buffer, 7),
                                                      database::get_version_id(version))
                                );
        }

        void export_gen2_pokemon(pokemon::sptr pkmn, native::gen2_pc_pokemon_t &native,
                                 uint8_t* nickname_buffer, uint8_t* otname_buffer)
        {
            memcpy(&native, pkmn->get_native(), sizeof(native::gen2_pc_pokemon_t));
            export_gen2_text(pkmn->get_nickname(), nickname_buffer, 10);
            export_gen2_text(pkmn->get_trainer_name(), otname_buffer, 7);
        }

        void export_gen2_pokemon(pokemon::sptr pkmn, native::gen2_party_pokemon_t &native,
                                 uint8_t* nickname_buffer, uint8_t* otname_buffer)
        {
            memcpy(&native, pkmn->get_native(), sizeof(native::gen2_party_pokemon_t));
            export_gen2_text(pkmn->get_nickname(), nickname_buffer, 10);
            export_gen2_text(pkmn->get_trainer_name(), otname_buffer, 7);
        }

        pokemon::sptr import_gen3_pokemon(const native::gen3_pc_pokemon_t &native,
                                          const pkmn::pkstring &version,
                                          bool is_encrypted)
        {
            native::gen3_pc_pokemon_t _native = native;
            if(is_encrypted) gen3_crypt(_native);

            return pokemon::sptr(new pokemon_gen3impl(_native,
                                                      database::get_version_id(version)));
        }

        pokemon::sptr import_gen3_pokemon(const native::gen3_party_pokemon_t &native,
                                          const pkmn::pkstring &version,
                                          bool is_encrypted)
        {
            native::gen3_party_pokemon_t _native = native;
            if(is_encrypted) gen3_crypt(_native.pc);

            return pokemon::sptr(new pokemon_gen3impl(_native,
                                                      database::get_version_id(version)));
        }

        void export_gen3_pokemon(pokemon::sptr pkmn, native::gen3_pc_pokemon_t &native,
                                 bool encrypt)
        {
            memcpy(&native, pkmn->get_native(), sizeof(native::gen3_pc_pokemon_t));
            if(encrypt) native.checksum = gen3_crypt(native);
        }

        void export_gen3_pokemon(pokemon::sptr pkmn, native::gen3_party_pokemon_t &native,
                                 bool encrypt)
        {
            memcpy(&native, pkmn->get_native(), sizeof(native::gen3_party_pokemon_t));
            if(encrypt) native.pc.checksum = gen3_crypt(native.pc);
        }

        pokemon::sptr import_nds_pokemon(const native::nds_pc_pokemon_t &native,
                                         const pkmn::pkstring &version,
                                         bool is_encrypted)
        {
            // TODO: crypt
            native::nds_pc_pokemon_t _native = native;

            return pokemon::sptr(new pokemon_ndsimpl(_native,
                                                      database::get_version_id(version)));
        }

        pokemon::sptr import_nds_pokemon(const native::nds_party_pokemon_t &native,
                                         const pkmn::pkstring &version,
                                         bool is_encrypted)
        {
            // TODO: crypt
            native::nds_party_pokemon_t _native = native;

            return pokemon::sptr(new pokemon_ndsimpl(_native,
                                                      database::get_version_id(version)));
        }

        void export_nds_pokemon(pokemon::sptr pkmn, native::nds_pc_pokemon_t &native,
                                bool encrypt)
        {
            // TODO: crypt
            memcpy(&native, pkmn->get_native(), sizeof(native::nds_pc_pokemon_t));
        }

        void export_nds_pokemon(pokemon::sptr pkmn, native::nds_party_pokemon_t &native,
                                bool encrypt)
        {
            // TODO: crypt
            memcpy(&native, pkmn->get_native(), sizeof(native::nds_party_pokemon_t));
        }

        void gen1_to_gen2(const native::gen1_party_pokemon_t &src, native::gen2_party_pokemon_t &dst)
        {
            CONNECT_TO_DB(db);

            dst.pc.species = CONVERT_POKEMON_GAME_INDEX(src.pc.species, Versions::RED, Versions::GOLD);
            dst.pc.held_item = src.pc.catch_rate;
            memcpy(&dst.pc.moves, &src.pc.moves, 25);

            std::ostringstream query_stream;
            query_stream << "SELECT base_happiness FROM pokemon_species WHERE id="
                         << GET_SPECIES_INDEX(src.pc.species, Versions::RED);
            SQLite::Statement friendship_query(*db, query_stream.str().c_str());
            dst.pc.friendship = get_num_from_query<uint8_t>(friendship_query);

            dst.pc.level = src.level;

            dst.status = src.pc.status;
            dst.current_hp = src.pc.current_hp;
            memcpy(&dst.max_hp, &src.max_hp, 8);

            query_stream.str("");
            query_stream << "SELECT base_stat FROM pokemon_stats WHERE pokemon_id="
                         << database::get_pokemon_id(dst.pc.species, Versions::GOLD)
                         << " AND base_stat IN (4,5)";
            SQLite::Statement stats_query(*db, query_stream.str().c_str());
            dst.spatk = calculations::get_retro_stat("Special Attack",
                                                     get_num_from_query<uint16_t>(stats_query),
                                                     dst.pc.level,
                                                     dst.pc.ev_spcl,
                                                     conversions::get_retro_IV(Stats::SPECIAL,
                                                                               dst.pc.iv_data)
                                                    );
            dst.spdef = calculations::get_retro_stat("Special Defense",
                                                     get_num_from_query<uint16_t>(stats_query),
                                                     dst.pc.level,
                                                     dst.pc.ev_spcl,
                                                     conversions::get_retro_IV(Stats::SPECIAL,
                                                                               dst.pc.iv_data)
                                                    );
        }

        void gen2_to_gen1(const native::gen2_party_pokemon_t &src, native::gen1_party_pokemon_t &dst)
        {
            CONNECT_TO_DB(db);

            // Make sure this species and these moves are valid
            if(GET_SPECIES_INDEX(src.pc.species, Versions::RED) > 151)
                throw std::runtime_error("This Pokémon did not exist in Generation I.");

            std::ostringstream query_stream;
            for(size_t i = 0; i < 4; i++)
            {
                query_stream.str("");
                query_stream << "SELECT generation_id FROM moves WHERE id=" << src.pc.moves[i];
                SQLite::Statement query(*db, query_stream.str().c_str());
                if(get_num_from_query<uint8_t>(query) > 1)
                    throw std::runtime_error("This Pokémon's moves did not exist in Generation I.");
            }

            pokemon_entry_t entry(Versions::RED,
                                  GET_SPECIES_INDEX(src.pc.species, Versions::RED),
                                  GET_SPECIES_INDEX(src.pc.species, Versions::RED));

            dst.pc.species = CONVERT_POKEMON_GAME_INDEX(src.pc.species, Versions::GOLD, Versions::RED);
            dst.pc.current_hp = src.current_hp;
            dst.pc.level = src.pc.level;
            dst.pc.status = src.status;
            dst.pc.types[0] = database::get_type_id(entry.types.first);
            dst.pc.types[1] = database::get_type_id(entry.types.second);
            dst.pc.catch_rate = src.pc.held_item; // Preserve held item
            memcpy(&dst.pc.moves, &src.pc.moves, 25);

            dst.level = src.pc.level;
            memcpy(&dst.max_hp, &src.max_hp, 8);

            query_stream.str("");
            query_stream << "SELECT base_stat FROM pokemon_stats WHERE pokemon_id="
                         << database::get_pokemon_id(dst.pc.species, Versions::RED)
                         << " AND base_stat=9";
            SQLite::Statement stats_query(*db, query_stream.str().c_str());
            dst.spcl = calculations::get_retro_stat("Special",
                                                    get_num_from_query<uint16_t>(stats_query),
                                                    dst.pc.level,
                                                    dst.pc.ev_spcl,
                                                    conversions::get_retro_IV(Stats::SPECIAL,
                                                                              dst.pc.iv_data)
                                                   );
        }
    } /* namespace conversions */
} /* namespace pkmn */