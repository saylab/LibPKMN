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
#include <pkmn/conversions/misc.hpp>
#include <pkmn/conversions/pokemon.hpp>
#include <pkmn/conversions/text.hpp>
#include <pkmn/native/checksum.hpp>
#include <pkmn/native/crypt.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include "pokemon_gen1impl.hpp"
#include "pokemon_gen2impl.hpp"
#include "pokemon_gen3impl.hpp"
#include "pokemon_ndsimpl.hpp"

#include "internal.hpp"
#include "utils.hpp"
#include "../SQLiteCpp/SQLiteC++.h"

#define CONVERT_POKEMON_GAME_INDEX(src,src_id,dst_id) database::get_pokemon_game_index( \
                                                          database::get_pokemon_id(src, src_id), dst_id)

#define CONVERT_ITEM_GAME_INDEX(src,src_id,dst_id) database::get_item_game_index( \
                                                       database::get_item_id(src, src_id), dst_id)

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
            if(is_encrypted) native::gen3_decrypt(_native);

            return pokemon::sptr(new pokemon_gen3impl(_native,
                                                      database::get_version_id(version)));
        }

        pokemon::sptr import_gen3_pokemon(const native::gen3_party_pokemon_t &native,
                                          const pkmn::pkstring &version,
                                          bool is_encrypted)
        {
            native::gen3_party_pokemon_t _native = native;
            if(is_encrypted) native::gen3_decrypt(_native.pc);

            return pokemon::sptr(new pokemon_gen3impl(_native,
                                                      database::get_version_id(version)));
        }

        void export_gen3_pokemon(pokemon::sptr pkmn, native::gen3_pc_pokemon_t &native,
                                 bool encrypt)
        {
            set_gen3_pokemon_checksum(native);
            memcpy(&native, pkmn->get_native(), sizeof(native::gen3_pc_pokemon_t));
            if(encrypt) native::gen3_encrypt(native);
        }

        void export_gen3_pokemon(pokemon::sptr pkmn, native::gen3_party_pokemon_t &native,
                                 bool encrypt)
        {
            set_gen3_pokemon_checksum(native.pc);
            memcpy(&native, pkmn->get_native(), sizeof(native::gen3_party_pokemon_t));
            if(encrypt) native::gen3_encrypt(native.pc);
        }

        pokemon::sptr import_nds_pokemon(const native::nds_pc_pokemon_t &native,
                                         const pkmn::pkstring &version,
                                         bool is_encrypted)
        {
            native::nds_pc_pokemon_t _native = native;
            if(is_encrypted) native::nds_decrypt(_native);

            return pokemon::sptr(new pokemon_ndsimpl(_native,
                                                      database::get_version_id(version)));
        }

        pokemon::sptr import_nds_pokemon(const native::nds_party_pokemon_t &native,
                                         const pkmn::pkstring &version,
                                         bool is_encrypted)
        {
            native::nds_party_pokemon_t _native = native;
            if(is_encrypted) native::nds_decrypt(_native);

            return pokemon::sptr(new pokemon_ndsimpl(_native,
                                                      database::get_version_id(version)));
        }

        void export_nds_pokemon(pokemon::sptr pkmn, native::nds_pc_pokemon_t &native,
                                bool encrypt)
        {
            set_nds_pokemon_checksum(native);
            if(encrypt) native::nds_encrypt(native);
            memcpy(&native, pkmn->get_native(), sizeof(native::nds_pc_pokemon_t));
        }

        void export_nds_pokemon(pokemon::sptr pkmn, native::nds_party_pokemon_t &native,
                                bool encrypt)
        {
            set_nds_pokemon_checksum(native.pc);
            if(encrypt) native::nds_encrypt(native);
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
            pkmn::dict<pkmn::pkstring, int> IVs = conversions::import_gb_IVs(dst.pc.iv_data);

            dst.spatk = calculations::get_retro_stat("Special Attack",
                                                     get_num_from_query<uint16_t>(stats_query),
                                                     dst.pc.level,
                                                     dst.pc.ev_spcl,
                                                     IVs["Special"]
                                                    );
            dst.spdef = calculations::get_retro_stat("Special Defense",
                                                     get_num_from_query<uint16_t>(stats_query),
                                                     dst.pc.level,
                                                     dst.pc.ev_spcl,
                                                     IVs["Special"]
                                                    );
        }

        void gen2_to_gen1(const native::gen2_party_pokemon_t &src, native::gen1_party_pokemon_t &dst)
        {
            CONNECT_TO_DB(db);

            // Make sure this species and these moves are valid
            if(GET_SPECIES_INDEX(src.pc.species, Versions::GOLD) > 151)
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
            pkmn::dict<pkmn::pkstring, int> IVs = conversions::import_gb_IVs(dst.pc.iv_data);

            dst.spcl = calculations::get_retro_stat("Special",
                                                    get_num_from_query<uint16_t>(stats_query),
                                                    dst.pc.level,
                                                    dst.pc.ev_spcl,
                                                    IVs["Special"]
                                                   );
        }

        void gen3_to_gen4(const native::gen3_party_pokemon_t &src, native::nds_party_pokemon_t &dst)
        {
            CONNECT_TO_DB(db);

            const native::gen3_pokemon_growth_t* gen3_growth = &(src.pc.blocks.growth);
            const native::gen3_pokemon_attacks_t* gen3_attacks = &(src.pc.blocks.attacks);
            const native::gen3_pokemon_effort_t* gen3_effort = &(src.pc.blocks.effort);
            const native::gen3_pokemon_misc_t* gen3_misc = &(src.pc.blocks.misc);

            native::nds_pokemon_blockA_t* gen4_blockA = &(dst.pc.blocks.blockA);
            native::nds_pokemon_blockB_t* gen4_blockB = &(dst.pc.blocks.blockB);
            native::nds_pokemon_blockC_t* gen4_blockC = &(dst.pc.blocks.blockC);
            native::nds_pokemon_blockD_t* gen4_blockD = &(dst.pc.blocks.blockD);

            pokemon_entry_t entry(Versions::HEARTGOLD,
                                  GET_SPECIES_INDEX(gen4_blockA->species, Versions::HEARTGOLD),
                                  GET_SPECIES_INDEX(gen4_blockA->species, Versions::HEARTGOLD));

            dst.pc.personality = src.pc.personality;
            dst.pc.isdecrypted_isegg = 0; // TODO: set decrypted flag

            // Block A
            gen4_blockA->species = CONVERT_POKEMON_GAME_INDEX(gen3_growth->species, Versions::EMERALD, Versions::HEARTGOLD);
            gen4_blockA->held_item = CONVERT_ITEM_GAME_INDEX(gen3_growth->held_item, Versions::EMERALD, Versions::HEARTGOLD);
            gen4_blockA->ot_id = src.pc.ot_id;
            gen4_blockA->exp = gen3_growth->exp;
            gen4_blockA->friendship = gen3_growth->friendship;
            gen4_blockA->ability = (dst.pc.personality % 2) ? database::get_ability_id(entry.abilities.second)
                                                            : database::get_ability_id(entry.abilities.first);
            gen4_blockA->markings = src.pc.markings;
            gen4_blockA->country = uint8_t(src.pc.language - 0x200);
            memcpy(&(gen4_blockA->ev_hp), &(gen3_effort->ev_hp), (6+sizeof(contest_stats_t)));

            // Block B
            memcpy(gen4_blockB, gen3_attacks, sizeof(native::gen3_pokemon_attacks_t));
            for(int i = 0; i < 4; i++)
                gen4_blockB->move_pp_ups[i] = ((gen3_growth->pp_up & (0x3 << (i*2))) >> (i*2));
            gen4_blockB->iv_isegg_isnicknamed = gen3_misc->iv_egg_ability & ~1; // Ability elsewhere
            if(PKSTRING_UPPERCASE(conversions::import_gen3_text(src.pc.nickname, 10)) !=
               PKSTRING_UPPERCASE(entry.species_name))
            {
                gen4_blockB->iv_isegg_isnicknamed |= 1;
            }
            gen4_blockB->hoenn_ribbons = gen3_misc->ribbons_obedience & ~1; // Fateful encounter flag elsewhere
            gen4_blockB->form_encounterinfo = (gen3_misc->ribbons_obedience >> 31); // Fateful encounter
            gen4_blockB->form_encounterinfo |= ((gen3_misc->origin_info & 0x2) >> 1); // Gender
            if(entry.chance_male == 0.0 and entry.chance_female == 0.0)
                gen4_blockB->form_encounterinfo |= 0x4; // Genderless
            gen4_blockB->eggmet_plat = 55; // Pal Park
            gen4_blockB->met_plat = 55; // Pal Park

            // Block C
            conversions::export_gen4_text(conversions::import_gen3_text(src.pc.nickname, 10), gen4_blockC->nickname, 10);
            gen4_blockC->hometown = ((gen3_misc->origin_info & ~0xFC3F) >> 6);

            // Block D
            conversions::export_gen4_text(conversions::import_gen3_text(src.pc.otname, 7), gen4_blockD->otname, 7);
            // TODO: egg met, met
            gen4_blockD->eggmet_dp = 55; // Pal Park
            gen4_blockD->met_dp = 55; // Pal Park
            // TODO: Pokerus
            gen4_blockD->ball = uint8_t((gen3_misc->origin_info & 0x7800) >> 11);
            gen4_blockD->metlevel_otgender = uint8_t(gen3_misc->origin_info & 0x7F);
            if(gen3_misc->origin_info & (1<<15)) gen4_blockD->metlevel_otgender |= (1<<7);
            gen4_blockD->encounter_info = 0; // Pal Park
            gen4_blockD->ball_hgss = uint8_t((gen3_misc->origin_info & 0x7800) >> 11);

            set_nds_pokemon_checksum(dst.pc);

            // Party info
            dst.level = src.level;
            memcpy(&(dst.current_hp), &(src.current_hp), 14);
        }

        void gen4_to_gen5(const native::nds_party_pokemon_t &src, native::nds_party_pokemon_t &dst)
        {
            CONNECT_TO_DB(db);

            const native::nds_pokemon_blockA_t* src_blockA = &(src.pc.blocks.blockA);
            native::nds_pokemon_blockA_t* dst_blockA = &(dst.pc.blocks.blockA);
            native::nds_pokemon_blockB_t* dst_blockB = &(dst.pc.blocks.blockB);

            dst = src;

            dst_blockA->species = CONVERT_POKEMON_GAME_INDEX(src_blockA->species, Versions::HEARTGOLD, Versions::BLACK_2);
            dst_blockA->held_item = CONVERT_ITEM_GAME_INDEX(src_blockA->species, Versions::HEARTGOLD, Versions::BLACK_2);
            dst_blockB->nature = dst.pc.personality % 24;

            set_nds_pokemon_checksum(dst.pc);
        }
    } /* namespace conversions */
} /* namespace pkmn */
