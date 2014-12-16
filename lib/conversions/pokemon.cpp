/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
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
#include <boost/algorithm/string.hpp>
#include <boost/locale/encoding_utf.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/item.hpp>
#include <pkmn/move.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/team_pokemon.hpp>
#include <pkmn/database.hpp>
#include <pkmn/types/pkstring.hpp>
#include <pkmn/types/prng.hpp>

#include <pkmds/pkmds_g5_sqlite.h>

#include "pokemon.hpp"
#include "utils.hpp"
#include "../library_bridge.hpp"
#include "../SQLiteCpp/SQLiteC++.h"

namespace pkmn
{
    namespace conversions
    {
        /*
         * Generation I
         */
        team_pokemon::sptr import_gen1_pokemon(const gen1_pc_pokemon_t &pkmn,
                                               const uint8_t* nickname_buffer,
                                               const uint8_t* otname_buffer)
        {
            unsigned int species_id, move1_id, move2_id, move3_id, move4_id;

            if(pkmn.species == 0 or pkmn.species > 190)
            {
                species_id = Species::INVALID;
                move1_id = Moves::NONE;
                move2_id = Moves::NONE;
                move3_id = Moves::NONE;
                move4_id = Moves::NONE;
            }
            else
            {
                species_id = database::get_pokemon_id(pkmn.species, Versions::RED);
                move1_id = pkmn.moves[0];
                move2_id = pkmn.moves[1];
                move3_id = pkmn.moves[2];
                move4_id = pkmn.moves[3];
            }

            team_pokemon::sptr t_pkmn = team_pokemon::make(species_id, Versions::RED,
                                                           pkmn.level, move1_id, move2_id,
                                                           move3_id, move4_id);

            //Nickname, trainer info
            t_pkmn->set_nickname(import_gen1_text(nickname_buffer, 10));
            t_pkmn->set_trainer_name(import_gen1_text(otname_buffer, 7));
            t_pkmn->set_trainer_id(pkmn.ot_id);
            t_pkmn->set_experience((65536*pkmn.exp[0]) + (256*pkmn.exp[1]) * pkmn.exp[2]);

            //Effort values
            t_pkmn->set_EV("HP", pkmn.ev_hp);
            t_pkmn->set_EV("Attack", pkmn.ev_atk);
            t_pkmn->set_EV("Defense", pkmn.ev_def);
            t_pkmn->set_EV("Speed", pkmn.ev_spd);
            t_pkmn->set_EV("Special", pkmn.ev_spcl);

            //Individual values
            t_pkmn->set_IV("HP", get_retro_IV(Stats::HP, pkmn.iv_data));
            t_pkmn->set_IV("Attack", get_retro_IV(Stats::ATTACK, pkmn.iv_data));
            t_pkmn->set_IV("Defense", get_retro_IV(Stats::DEFENSE, pkmn.iv_data));
            t_pkmn->set_IV("Speed", get_retro_IV(Stats::SPEED, pkmn.iv_data));
            t_pkmn->set_IV("Special", get_retro_IV(Stats::SPECIAL, pkmn.iv_data));

            //Move PP's
            for(size_t i = 0; i < 4; i++) t_pkmn->set_move_PP(pkmn.move_pps[i], (i+1));

            return t_pkmn;
        }

        void export_gen1_pokemon(team_pokemon::sptr t_pkmn, gen1_pc_pokemon_t &pkmn,
                                 uint8_t* nickname_buffer, uint8_t* otname_buffer)
        {
            //Necessary values
            pkmn::dict<pkmn::pkstring, unsigned int> stats = t_pkmn->get_stats();
            pkmn::dict<pkmn::pkstring, unsigned int> EVs = t_pkmn->get_EVs();
            pkmn::dict<pkmn::pkstring, unsigned int> IVs = t_pkmn->get_IVs();
            pkmn::pkstring_pair_t types = t_pkmn->get_types();
            pkmn::moveset_t moves;
            t_pkmn->get_moves(moves);
            std::vector<unsigned int> move_PPs;
            t_pkmn->get_move_PPs(move_PPs);

            export_gen1_text(t_pkmn->get_nickname(), nickname_buffer, 10);
            export_gen1_text(t_pkmn->get_trainer_name(), otname_buffer, 10);

            pkmn.species = database::get_pokemon_game_index(t_pkmn->get_species_id(), Versions::RED);
            pkmn.current_hp = t_pkmn->get_stats()["HP"];
            pkmn.level = t_pkmn->get_level();
            pkmn.status = 0;

            pkmn.types[0] = database::get_type_id(types.first);
            pkmn.types[1] = database::get_type_id(types.second);

            pkmn.catch_rate = gen1_catch_rates[t_pkmn->get_species_id()];
            for(size_t i = 0; i < 4; i++) pkmn.moves[i] = moves[i]->get_move_id();
            pkmn.ot_id = t_pkmn->get_trainer_id();

            unsigned int experience = t_pkmn->get_experience();
            pkmn.exp[0] = (experience /= 65536);
            pkmn.exp[1] = (experience /= 256);
            pkmn.exp[2] = experience;

            //Effort values
            pkmn.ev_hp = EVs["HP"];
            pkmn.ev_atk = EVs["Attack"];
            pkmn.ev_def = EVs["Defense"];
            pkmn.ev_spd = EVs["Speed"];
            pkmn.ev_spcl = EVs["Special"];

            //Individual values
            set_retro_IV(Stats::HP, pkmn.iv_data, IVs["HP"]);
            set_retro_IV(Stats::ATTACK, pkmn.iv_data, IVs["Attack"]);
            set_retro_IV(Stats::DEFENSE, pkmn.iv_data, IVs["Defense"]);
            set_retro_IV(Stats::SPEED, pkmn.iv_data, IVs["Speed"]);
            set_retro_IV(Stats::SPECIAL, pkmn.iv_data, IVs["Special"]);

            for(size_t i = 0; i < 4; i++) pkmn.move_pps[i] = move_PPs[i];
        }

        team_pokemon::sptr import_gen1_pokemon(const gen1_party_pokemon_t &pkmn,
                                               const uint8_t* nickname_buffer,
                                               const uint8_t* otname_buffer)
        {
            return import_gen1_pokemon(pkmn.pc, nickname_buffer, otname_buffer);
        }

        void export_gen1_pokemon(team_pokemon::sptr t_pkmn, gen1_party_pokemon_t &pkmn,
                                 uint8_t* nickname_buffer, uint8_t* otname_buffer)
        {
            pkmn::dict<pkmn::pkstring, unsigned int> stats = t_pkmn->get_stats();

            export_gen1_pokemon(t_pkmn, pkmn.pc, nickname_buffer, otname_buffer);
            pkmn.level = t_pkmn->get_level();
            pkmn.max_hp = stats["HP"];
            pkmn.atk = stats["Attack"];
            pkmn.def = stats["Defense"];
            pkmn.spd = stats["Speed"];
            pkmn.spcl = stats["Special"];
        }

        /*
         * Generation II
         */
        team_pokemon::sptr import_gen2_pokemon(const gen2_pc_pokemon_t &pkmn,
                                               const uint8_t* nickname_buffer,
                                               const uint8_t* otname_buffer)
        {
            unsigned int species_id, move1_id, move2_id, move3_id, move4_id;

            if(pkmn.species == 0 or pkmn.species > 251)
            {
                species_id = Species::INVALID;
                move1_id = Moves::NONE;
                move2_id = Moves::NONE;
                move3_id = Moves::NONE;
                move4_id = Moves::NONE;
            }
            else
            {
                species_id = database::get_pokemon_id(pkmn.species, Versions::RED);
                move1_id = pkmn.moves[0];
                move2_id = pkmn.moves[1];
                move3_id = pkmn.moves[2];
                move4_id = pkmn.moves[3];
            }

            team_pokemon::sptr t_pkmn = team_pokemon::make(species_id, Versions::CRYSTAL,
                                                           pkmn.level, move1_id, move2_id,
                                                           move3_id, move4_id);

            //Nickname, trainer info
            t_pkmn->set_nickname(import_gen2_text(nickname_buffer, 10));
            t_pkmn->set_trainer_name(import_gen2_text(otname_buffer, 7));
            t_pkmn->set_trainer_id(pkmn.ot_id);
            t_pkmn->set_trainer_gender(crystal_get_otgender(pkmn.caught_data));
            t_pkmn->set_met_level(crystal_get_metlevel(pkmn.caught_data));
            t_pkmn->set_held_item(database::get_item_name(pkmn.held_item, Versions::CRYSTAL));
            t_pkmn->set_experience((65536*pkmn.exp[0]) + (256*pkmn.exp[1]) * pkmn.exp[2]);
            //TODO: friendship

            //Effort values
            t_pkmn->set_EV("HP", pkmn.ev_hp);
            t_pkmn->set_EV("Attack", pkmn.ev_atk);
            t_pkmn->set_EV("Defense", pkmn.ev_def);
            t_pkmn->set_EV("Speed", pkmn.ev_spd);
            t_pkmn->set_EV("Special", pkmn.ev_spcl);

            //Individual values
            t_pkmn->set_IV("HP", get_retro_IV(Stats::HP, pkmn.iv_data));
            t_pkmn->set_IV("Attack", get_retro_IV(Stats::ATTACK, pkmn.iv_data));
            t_pkmn->set_IV("Defense", get_retro_IV(Stats::DEFENSE, pkmn.iv_data));
            t_pkmn->set_IV("Speed", get_retro_IV(Stats::SPEED, pkmn.iv_data));
            t_pkmn->set_IV("Special", get_retro_IV(Stats::SPECIAL, pkmn.iv_data));

            //Move PP's
            for(size_t i = 0; i < 4; i++) t_pkmn->set_move_PP(pkmn.move_pps[i], (i+1));

            return t_pkmn;
        }

        void export_gen2_pokemon(team_pokemon::sptr t_pkmn, gen2_pc_pokemon_t &pkmn,
                                 uint8_t* nickname_buffer, uint8_t* otname_buffer)
        {
            SQLite::Database db(get_database_path().c_str());
            pkmn::dict<pkmn::pkstring, unsigned int> EVs = t_pkmn->get_EVs();
            pkmn::dict<pkmn::pkstring, unsigned int> IVs = t_pkmn->get_IVs();
            moveset_t moves;
            t_pkmn->get_moves(moves);
            std::vector<unsigned int> move_PPs;
            t_pkmn->get_move_PPs(move_PPs);

            export_gen2_text(t_pkmn->get_nickname(), nickname_buffer, 10);
            export_gen2_text(t_pkmn->get_trainer_name(), otname_buffer, 10);

            pkmn.species = database::get_pokemon_game_index(t_pkmn->get_species_id(), Versions::CRYSTAL);
            pkmn.held_item = database::get_item_game_index(t_pkmn->get_item_id(), Versions::CRYSTAL);
            for(size_t i = 0; i < 4; i++) pkmn.moves[i] = moves[0]->get_move_id();
            pkmn.ot_id = t_pkmn->get_trainer_id();

            unsigned int experience = t_pkmn->get_experience();
            pkmn.exp[0] = (experience /= 65536);
            pkmn.exp[1] = (experience /= 256);
            pkmn.exp[2] = experience;

            //Effort values
            pkmn.ev_hp = EVs["HP"];
            pkmn.ev_atk = EVs["Attack"];
            pkmn.ev_def = EVs["Defense"];
            pkmn.ev_spd = EVs["Speed"];
            pkmn.ev_spcl = EVs["Special"];

            //Individual values
            set_retro_IV(Stats::HP, pkmn.iv_data, IVs["HP"]);
            set_retro_IV(Stats::ATTACK, pkmn.iv_data, IVs["Attack"]);
            set_retro_IV(Stats::DEFENSE, pkmn.iv_data, IVs["Defense"]);
            set_retro_IV(Stats::SPEED, pkmn.iv_data, IVs["Speed"]);
            set_retro_IV(Stats::SPECIAL, pkmn.iv_data, IVs["Special"]);

            for(size_t i = 0; i < 4; i++) pkmn.move_pps[i] = move_PPs[i];

            pkmn.pokerus = 0;
            crystal_set_caughtdata(pkmn.caught_data, (t_pkmn->get_trainer_gender() == "Female"),
                                                     (t_pkmn->get_met_level()));
            pkmn.level = t_pkmn->get_level();
        }

        team_pokemon::sptr import_gen2_pokemon(const gen2_party_pokemon_t &pkmn,
                                               const uint8_t* nickname_buffer,
                                               const uint8_t* otname_buffer)
        {
            return import_gen2_pokemon(pkmn.pc, nickname_buffer, otname_buffer);
        }

        void export_gen2_pokemon(team_pokemon::sptr t_pkmn, gen2_party_pokemon_t &pkmn,
                                 uint8_t* nickname_buffer, uint8_t* otname_buffer)
        {
            pkmn::dict<pkmn::pkstring, unsigned int> stats = t_pkmn->get_stats();

            export_gen2_pokemon(t_pkmn, pkmn.pc, nickname_buffer, otname_buffer);
            pkmn.status = 0;
            pkmn.current_hp = stats["HP"];
            pkmn.max_hp = stats["HP"];
            pkmn.atk = stats["Attack"];
            pkmn.def = stats["Defense"];
            pkmn.spd = stats["Speed"];
            pkmn.spatk = stats["Special Attack"];
            pkmn.spdef = stats["Special Defense"];
        }

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
            {G,A,E,M},{G,A,M,E},{G,E,A,M},{G,M,A,E},{G,M,A,E},{G,M,E,A},
            {A,G,E,M},{A,G,M,E},{E,G,A,M},{M,G,A,E},{A,M,G,E},{A,M,E,G},
            {A,E,G,M},{E,G,M,A},{E,A,G,M},{E,A,M,G},{E,M,G,A},{E,M,A,G},
            {M,G,A,E},{M,G,E,A},{E,A,G,M},{M,A,M,G},{M,E,G,A},{M,E,A,G}
        };

        //Never directly encrypt/decrypt the binary file itself
        static void gen3_get_decrypted_blocks(const gen3_pc_pokemon_t &pkmn, gen3_pokemon_blocks_t &blocks)
        {
            uint8_t index = pkmn.personality % 24;

            memcpy(&blocks.growth,  pkmn.blocks.blocks[gen3_block_orders[index][G]], 12);
            memcpy(&blocks.attacks, pkmn.blocks.blocks[gen3_block_orders[index][A]], 12);
            memcpy(&blocks.effort,  pkmn.blocks.blocks[gen3_block_orders[index][E]], 12);
            memcpy(&blocks.misc,    pkmn.blocks.blocks[gen3_block_orders[index][M]], 12);

            uint32_t security_key = pkmn.ot_id ^ pkmn.personality;
            for(uint8_t i = 0; i < 12; i++) blocks.blocks32[i] ^= security_key;
        }

        //Returns checksum
        static uint16_t gen3_set_encrypted_blocks(gen3_pokemon_blocks_t &blocks, gen3_pc_pokemon_t pkmn)
        {
            uint16_t checksum = 0;
            uint8_t index = pkmn.personality % 24;

            memcpy(pkmn.blocks.blocks[gen3_block_orders[index][G]],  &blocks.growth, 12);
            memcpy(pkmn.blocks.blocks[gen3_block_orders[index][A]], &blocks.attacks, 12);
            memcpy(pkmn.blocks.blocks[gen3_block_orders[index][E]],  &blocks.effort, 12);
            memcpy(pkmn.blocks.blocks[gen3_block_orders[index][M]],    &blocks.misc, 12);

            uint32_t security_key = pkmn.ot_id ^ pkmn.personality;
            for(uint8_t i = 0; i < 48; i++) checksum += blocks.blocks8[i];
            for(uint8_t i = 0; i < 12; i++) blocks.blocks32[i] ^= security_key;

            return checksum;
        }

        team_pokemon::sptr import_gen3_pokemon(const gen3_pc_pokemon_t &pkmn,
                                               unsigned int version_id, bool is_encrypted)
        {
            //Since version_id is given, make sure it's legit
            if(database::get_generation(version_id) != 3){
                throw std::runtime_error("import_gen3_pokemon: version_id must be from Generation III.");
            }

            gen3_pokemon_blocks_t blocks;
            if(is_encrypted) gen3_get_decrypted_blocks(pkmn, blocks);
            else memcpy(&blocks, &pkmn.blocks, sizeof(gen3_pokemon_blocks_t));

            unsigned int species_id, move1_id, move2_id, move3_id, move4_id;

            if(blocks.growth.species == 0 or (blocks.growth.species > 251 and blocks.growth.species < 277) or
               blocks.growth.species == 412)
            {
                species_id = Species::INVALID;
                move1_id = Moves::NONE;
                move2_id = Moves::NONE;
                move3_id = Moves::NONE;
                move4_id = Moves::NONE;
            }
            else
            {
                species_id = database::get_pokemon_id(blocks.growth.species, version_id);
                move1_id = blocks.attacks.moves[0];
                move2_id = blocks.attacks.moves[1];
                move3_id = blocks.attacks.moves[2];
                move4_id = blocks.attacks.moves[3];
            }

            SQLite::Database db(get_database_path().c_str());
            std::ostringstream query_stream;
            unsigned int level = database::get_level(species_id, blocks.growth.exp);

            team_pokemon::sptr t_pkmn = team_pokemon::make(species_id, version_id,
                                                           level, move1_id, move2_id,
                                                           move3_id, move4_id);

            t_pkmn->set_personality(pkmn.personality);
            t_pkmn->set_trainer_id(pkmn.ot_id);
            t_pkmn->set_nickname(import_gen3_text(pkmn.nickname, 10));
            t_pkmn->set_trainer_name(import_gen3_text(pkmn.otname, 7));
            t_pkmn->set_markings(pkmn.markings);

            //Growth
            t_pkmn->set_held_item(database::get_item_name(blocks.growth.held_item, version_id));
            t_pkmn->set_experience(blocks.growth.exp);
            //TODO: PP Up, friendship

            //Attacks
            for(size_t i = 0; i < 4; i++) t_pkmn->set_move_PP(blocks.attacks.move_pps[i], (i+1));

            //Effort
            //TODO: contest stats
            t_pkmn->set_EV("HP", blocks.effort.ev_hp);
            t_pkmn->set_EV("Attack", blocks.effort.ev_atk);
            t_pkmn->set_EV("Defense", blocks.effort.ev_def);
            t_pkmn->set_EV("Speed", blocks.effort.ev_spd);
            t_pkmn->set_EV("Special Attack", blocks.effort.ev_spatk);
            t_pkmn->set_EV("Special Defense", blocks.effort.ev_spdef);

            //Misc
            //TODO: Pokerus, met location, Origin info, ribbons, obedience
            t_pkmn->set_IV("HP", get_modern_IV(Stats::HP, blocks.misc.iv_egg_ability));
            t_pkmn->set_IV("Attack", get_modern_IV(Stats::ATTACK, blocks.misc.iv_egg_ability));
            t_pkmn->set_IV("Defense", get_modern_IV(Stats::DEFENSE, blocks.misc.iv_egg_ability));
            t_pkmn->set_IV("Speed", get_modern_IV(Stats::SPEED, blocks.misc.iv_egg_ability));
            t_pkmn->set_IV("Special Attack", get_modern_IV(Stats::SPECIAL_ATTACK, blocks.misc.iv_egg_ability));
            t_pkmn->set_IV("Special Defense", get_modern_IV(Stats::SPECIAL_DEFENSE, blocks.misc.iv_egg_ability));

            int ability_slot = get_gen3_ability_slot(blocks.misc.iv_egg_ability) + 1;
            query_stream.str("");
            query_stream << "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=" << t_pkmn->get_pokemon_id()
                         << " AND is_hidden=0 AND slot=" << ability_slot;
            SQLite::Statement query(db, query_stream.str().c_str());

            unsigned int ability_id;
            if((not query.executeStep()) and ability_slot == 2)
            {
                //Account for odd situation where slot=1 but there is no second ability
                query_stream.str("");
                query_stream << "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=" << t_pkmn->get_pokemon_id()
                             << " AND is_hidden=0 AND slot=1";
                ability_id = db.execAndGet(query_stream.str().c_str());
            }
            else ability_id = query.getColumn(0);
            t_pkmn->set_ability(database::get_ability_name(ability_id));

            pkmn::ribbons rib;
            rib.hoenn = blocks.misc.ribbons_obedience;
            t_pkmn->set_ribbons(rib);

            return t_pkmn;
        }

        void export_gen3_pokemon(team_pokemon::sptr t_pkmn, gen3_pc_pokemon_t &pkmn, bool encrypt)
        {
            SQLite::Database db(get_database_path().c_str());
            pkmn::dict<pkmn::pkstring, unsigned int> EVs = t_pkmn->get_EVs();
            pkmn::dict<pkmn::pkstring, unsigned int> IVs = t_pkmn->get_IVs();
            moveset_t moves;
            t_pkmn->get_moves(moves);
            std::vector<unsigned int> move_PPs;
            t_pkmn->get_move_PPs(move_PPs);

            gen3_pokemon_blocks_t blocks;

            //Growth
            blocks.growth.species = database::get_pokemon_game_index(t_pkmn->get_species_id(), t_pkmn->get_game_id());
            blocks.growth.held_item = database::get_item_game_index(t_pkmn->get_held_item()->get_item_id(),
                                                                  Versions::FIRERED);
            blocks.growth.exp = t_pkmn->get_experience();

            std::ostringstream query_stream;
            blocks.growth.exp = t_pkmn->get_experience();
            blocks.growth.pp_up = 0; //TODO
            blocks.growth.friendship = 70; //Base

            //Attacks
            for(size_t i = 0; i < 4; i++)
            {
                blocks.attacks.moves[i] = moves[i]->get_move_id();
                blocks.attacks.move_pps[i] = move_PPs[i];
            }

            //Effort
            //TODO: contest stats
            blocks.effort.ev_hp = EVs["HP"];
            blocks.effort.ev_atk = EVs["Attack"];
            blocks.effort.ev_def = EVs["Defense"];
            blocks.effort.ev_spd = EVs["Speed"];
            blocks.effort.ev_spatk = EVs["Special Attack"];
            blocks.effort.ev_spdef = EVs["Special Defense"];

            //Misc
            blocks.misc.pokerus = 0; //TODO
            blocks.misc.met_location = 0xFF; //TODO, "fateful encounter" for now
            //TODO: origin info
            set_modern_IV(Stats::HP, blocks.misc.iv_egg_ability, IVs["HP"]);
            set_modern_IV(Stats::ATTACK, blocks.misc.iv_egg_ability, IVs["Attack"]);
            set_modern_IV(Stats::DEFENSE, blocks.misc.iv_egg_ability, IVs["Defense"]);
            set_modern_IV(Stats::SPEED, blocks.misc.iv_egg_ability, IVs["Speed"]);
            set_modern_IV(Stats::SPECIAL_ATTACK, blocks.misc.iv_egg_ability, IVs["Special Attack"]);
            set_modern_IV(Stats::SPECIAL_DEFENSE, blocks.misc.iv_egg_ability, IVs["Special Defense"]);

            query_stream.str("");
            query_stream << "SELECT slot FROM pokemon_abilities WHERE pokemon_id=" << t_pkmn->get_pokemon_id()
                         << " AND is_hidden=0 AND ability_id=" << t_pkmn->get_ability_id();
            unsigned int ability_slot = db.execAndGet(query_stream.str().c_str());
            set_gen3_ability_slot(blocks.misc.iv_egg_ability, ability_slot);
            blocks.misc.ribbons_obedience = t_pkmn->get_ribbons().hoenn;
            blocks.misc.ribbons_obedience |= (1<<31); // Mew/Deoxys obedience

            //Outside of blocks
            pkmn.personality = t_pkmn->get_personality();
            pkmn.ot_id = t_pkmn->get_trainer_id();
            export_gen3_text(t_pkmn->get_nickname(), pkmn.nickname, 10);
            pkmn.language = 0x202; //English
            export_gen3_text(t_pkmn->get_trainer_name(), pkmn.otname, 7);
            pkmn.markings = t_pkmn->get_markings();

            if(encrypt) pkmn.checksum = gen3_set_encrypted_blocks(blocks, pkmn);
            else
            {
                memcpy(&(pkmn.blocks), &blocks, sizeof(gen3_pokemon_blocks_t));
                uint32_t security_key = pkmn.ot_id ^ pkmn.personality;
                for(uint8_t i = 0; i < 48; i++) pkmn.checksum += pkmn.blocks.blocks8[i];
                for(uint8_t i = 0; i < 12; i++) pkmn.blocks.blocks32[i] ^= security_key;
            }
        }

        team_pokemon::sptr import_gen3_pokemon(const gen3_party_pokemon_t &pkmn, unsigned int version_id,
                                               bool is_encrypted)
        {
            return import_gen3_pokemon(pkmn.pc, version_id, is_encrypted);
        }

        void export_gen3_pokemon(team_pokemon::sptr t_pkmn, gen3_party_pokemon_t &pkmn, bool encrypt)
        {
            pkmn::dict<pkmn::pkstring, unsigned int> stats = t_pkmn->get_stats();

            export_gen3_pokemon(t_pkmn, pkmn.pc, encrypt);
            pkmn.condition = 0;
            pkmn.level = t_pkmn->get_level();
            pkmn.pokerus_time = 0; //TODO
            pkmn.current_hp = stats["HP"]; //TODO
            pkmn.max_hp = stats["HP"];
            pkmn.atk = stats["Attack"];
            pkmn.def = stats["Defense"];
            pkmn.spd = stats["Speed"];
            pkmn.spatk = stats["Special Attack"];
            pkmn.spdef = stats["Special Defense"];
        }

        /*
         * Nintendo DS (Generation IV-V)
         */
        #define NDS_SPECIES_IS(x) (blocks.blockA.species == x)
        #define NDS_SPECIES_GT(x) (blocks.blockA.species > x)
        #define NDS_SPECIES_LT(x) (blocks.blockA.species < x)

        team_pokemon::sptr import_nds_pokemon(const nds_pc_pokemon_t &pkmn, unsigned int version_id,
                                              bool is_encrypted)
        {
            // Since version_id is given, make sure it's legit
            unsigned int gen = database::get_generation(version_id);
            if(gen < 4 or gen > 5)
                throw std::runtime_error("import_nds_pokemon: version_id must be from Generation IV-V.");

            nds_pokemon_blocks_t blocks;
            // TODO: implement crypt functions
            memcpy(&blocks, &pkmn.blocks, sizeof(nds_pokemon_blocks_t));

            unsigned int species_id, move1_id, move2_id, move3_id, move4_id;

            unsigned int version_group_id = database::get_version_group_id(version_id);
            bool is_valid;
            if(version_group_id == Version_Groups::DIAMOND_PEARL)
                is_valid = !(NDS_SPECIES_IS(494)) and !(NDS_SPECIES_IS(495)) and NDS_SPECIES_LT(501);
            else if(version_group_id == Version_Groups::PLATINUM)
                is_valid = !(NDS_SPECIES_IS(494)) and !(NDS_SPECIES_IS(495)) and NDS_SPECIES_LT(508);
            else
                is_valid = NDS_SPECIES_LT(650);

            if(!is_valid)
            {
                species_id = Species::INVALID;
                move1_id = Moves::NONE;
                move2_id = Moves::NONE;
                move3_id = Moves::NONE;
                move4_id = Moves::NONE;
            }
            else
            {
                species_id = database::get_pokemon_id(blocks.blockA.species, version_id);
                move1_id = blocks.blockB.moves[0];
                move2_id = blocks.blockB.moves[1];
                move3_id = blocks.blockB.moves[2];
                move4_id = blocks.blockB.moves[3];
            }

            SQLite::Database db(get_database_path().c_str());
            std::ostringstream query_stream;
            unsigned int level = database::get_level(species_id, blocks.blockA.exp);

            team_pokemon::sptr t_pkmn = team_pokemon::make(species_id, version_id,
                                                           level, move1_id, move2_id,
                                                           move3_id, move4_id);

            // Block A
            t_pkmn->set_held_item(database::get_item_name(blocks.blockA.held_item, version_id));
            t_pkmn->set_trainer_id(blocks.blockA.ot_id);
            t_pkmn->set_experience(blocks.blockA.exp);
            // TODO: friendship
            t_pkmn->set_ability(database::get_ability_name(blocks.blockA.ability));
            t_pkmn->set_markings(blocks.blockA.markings);
            t_pkmn->set_EV("HP", blocks.blockA.ev_hp);
            t_pkmn->set_EV("Attack", blocks.blockA.ev_atk);
            t_pkmn->set_EV("Defense", blocks.blockA.ev_def);
            t_pkmn->set_EV("Speed", blocks.blockA.ev_spd);
            t_pkmn->set_EV("Special Attack", blocks.blockA.ev_spatk);
            t_pkmn->set_EV("Special Defense", blocks.blockA.ev_spdef);
            pkmn::ribbons rib;
            rib.sinnoh.ribbons1 = blocks.blockA.sinnoh_ribbons1;
            if(gen == 4) rib.sinnoh.ribbons2 = blocks.blockA.sinnoh_ribbons2;
            else rib.unova = blocks.blockA.unova_ribbons;

            // Block B
            for(size_t i = 0; i < 4; i++) t_pkmn->set_move((i+1), blocks.blockB.moves[i]);
            for(size_t i = 0; i < 4; i++) t_pkmn->set_move_PP((i+1), blocks.blockB.move_pps[i]);
            t_pkmn->set_IV("HP", modern_get_IV(&blocks.blockB.iv_isegg_isnicknamed, Stats::HP));
            t_pkmn->set_IV("Attack", modern_get_IV(&blocks.blockB.iv_isegg_isnicknamed, Stats::ATTACK));
            t_pkmn->set_IV("Defense", modern_get_IV(&blocks.blockB.iv_isegg_isnicknamed, Stats::DEFENSE));
            t_pkmn->set_IV("Speed", modern_get_IV(&blocks.blockB.iv_isegg_isnicknamed, Stats::SPEED));
            t_pkmn->set_IV("Special Attack", modern_get_IV(&blocks.blockB.iv_isegg_isnicknamed, Stats::SPECIAL_ATTACK));
            t_pkmn->set_IV("Special Defense", modern_get_IV(&blocks.blockB.iv_isegg_isnicknamed, Stats::SPECIAL_DEFENSE));
            rib.hoenn = blocks.blockB.hoenn_ribbons;
            // TODO: Form, encounter info
            if(gen == 4) t_pkmn->set_attribute("shiny-leaf", blocks.blockB.shiny_leaf);
            t_pkmn->set_nature(database::get_nature_name((gen == 4) ? (pkmn.personality % 24)
                                                                    :  blocks.blockB.nature));
            // TODO: Gen V info

            // Block C
            t_pkmn->set_nickname((gen == 4) ? import_gen4_text(blocks.blockC.nickname, 11)
                                            : import_modern_text(blocks.blockC.nickname, 11));
            t_pkmn->set_original_game(database::get_version_name(hometown_to_libpkmn_game(blocks.blockC.hometown)));
            rib.sinnoh.ribbons3 = blocks.blockC.sinnoh_ribbons3;
            t_pkmn->set_ribbons(rib);

            // Block D
            t_pkmn->set_trainer_name((gen == 4) ? import_gen4_text(blocks.blockD.otname, 8)
                                                : import_modern_text(blocks.blockD.otname, 8));
            // TODO: met dates
            // TODO: Pokerus
            if(version_group_id == Version_Groups::HEARTGOLD_SOULSILVER)
                t_pkmn->set_ball(ball_dict.at(game_ball_to_libpkmn_ball(blocks.blockD.ball_hgss), "Poke Ball"));
            else
                t_pkmn->set_ball(ball_dict.at(game_ball_to_libpkmn_ball(blocks.blockD.ball), "Poke Ball"));
            t_pkmn->set_met_level(get_gen_456_met_level(&blocks.blockD.metlevel_otgender));
            t_pkmn->set_trainer_gender((get_gen_456_otgender(&blocks.blockD.metlevel_otgender)) ? "Female" : "Male");
            // TODO: encounter info
        }

        team_pokemon::sptr import_nds_pokemon(const nds_party_pokemon_t &pkmn, unsigned int version_id,
                                              bool is_encrypted)
        {
            return import_nds_pokemon(pkmn.pc, version_id, is_encrypted);
        }

        void export_nds_pokemon(team_pokemon::sptr t_pkmn, nds_pc_pokemon_t &pkmn,
                                bool encrypt)
        {
            pkmn::dict<pkmn::pkstring, unsigned int> EVs = t_pkmn->get_EVs();
            pkmn::dict<pkmn::pkstring, unsigned int> IVs = t_pkmn->get_IVs();
            pkmn::dict<pkmn::pkstring, int> attributes = t_pkmn->get_attributes();
            pkmn::ribbons rib = t_pkmn->get_ribbons();
            pkmn::moveset_t moves;
            t_pkmn->get_moves(moves);
            std::vector<unsigned int> move_PPs;
            t_pkmn->get_move_PPs(move_PPs);
            unsigned int generation = t_pkmn->get_generation();

            // Pointers to blocks (for code cleanliness)
            nds_pokemon_blockA_t* blockA = &pkmn.blocks.blockA;
            nds_pokemon_blockB_t* blockB = &pkmn.blocks.blockB;
            nds_pokemon_blockC_t* blockC = &pkmn.blocks.blockC;
            nds_pokemon_blockD_t* blockD = &pkmn.blocks.blockD;

            // Block A
            blockA->species = database::get_pokemon_game_index(t_pkmn->get_species_id(), t_pkmn->get_game_id());
            blockA->held_item = database::get_item_game_index(t_pkmn->get_item_id(), t_pkmn->get_game_id());
            blockA->ot_id = t_pkmn->get_trainer_id();
            blockA->exp = t_pkmn->get_experience();
            blockA->friendship = 70; // TODO
            blockA->ability = t_pkmn->get_ability_id();
            blockA->markings = t_pkmn->get_markings();
            blockA->country = 2; // English
            blockA->ev_hp = EVs["HP"];
            blockA->ev_atk = EVs["Attack"];
            blockA->ev_def = EVs["Defense"];
            blockA->ev_spd = EVs["Speed"];
            blockA->ev_spatk = EVs["Special Attack"];
            blockA->ev_spdef = EVs["Special Defense"];
            // TODO: contest stats
            blockA->coolness = 0;
            blockA->beauty = 0;
            blockA->cuteness = 0;
            blockA->smartness = 0;
            blockA->toughness = 0;
            blockA->sheen = 0;
            blockA->sinnoh_ribbons1 = rib.sinnoh.ribbons1;
            if(generation == 4) blockA->sinnoh_ribbons2 = rib.sinnoh.ribbons2;
            else blockA->unova_ribbons = rib.unova;

            // Block B
            for(size_t i = 0; i < 4; i++) blockB->moves[i] = moves[i]->get_move_id();
            for(size_t i = 0; i < 4; i++) blockB->move_pps[i] = move_PPs[i];
            for(size_t i = 0; i < 4; i++) blockB->move_pp_ups[i] = 0;
            blockB->iv_isegg_isnicknamed = 0;
            modern_set_IV(&blockB->iv_isegg_isnicknamed, Stats::HP, IVs["HP"]);
            modern_set_IV(&blockB->iv_isegg_isnicknamed, Stats::ATTACK, IVs["Attack"]);
            modern_set_IV(&blockB->iv_isegg_isnicknamed, Stats::DEFENSE, IVs["Defense"]);
            modern_set_IV(&blockB->iv_isegg_isnicknamed, Stats::SPEED, IVs["Speed"]);
            modern_set_IV(&blockB->iv_isegg_isnicknamed, Stats::SPECIAL_ATTACK, IVs["Special Attack"]);
            modern_set_IV(&blockB->iv_isegg_isnicknamed, Stats::SPECIAL_DEFENSE, IVs["Special Defense"]);
            if(t_pkmn->get_nickname() == boost::algorithm::to_upper_copy(t_pkmn->get_species_name().std_wstring()))
                blockB->iv_isegg_isnicknamed |= (1<<31);
            blockB->hoenn_ribbons = rib.hoenn;

            if(t_pkmn->get_gender() == "Female") blockB->form_encounterinfo |= (1<<1);
            else if(t_pkmn->get_gender() == "Genderless") blockB->form_encounterinfo |= (1<<2);
            // TODO: forms

            if(generation == 5) blockB->nature = t_pkmn->get_nature_id();
            else blockB->shiny_leaf = attributes.at("shiny-leaf", 0);

            blockB->gen5_info = 0;
            if(generation == 5)
            {
                SQLite::Database db(get_database_path().c_str());
                std::ostringstream query_stream;
                query_stream << "SELECT ability_id FROM pokemon_abilities WHERE is_hidden=1 AND pokemon_id="
                             << t_pkmn->get_pokemon_id();
                unsigned int dw_ability_id = db.execAndGet(query_stream.str().c_str());
                if(t_pkmn->get_ability_id() == dw_ability_id) blockB->gen5_info |= (1<<31);
            }
            // TODO: met

            // Block C
            if(generation == 4) export_gen4_text(t_pkmn->get_nickname(), blockC->nickname, 11);
            else export_modern_text(t_pkmn->get_nickname(), blockC->nickname, 11);
            blockC->hometown = libpkmn_game_to_hometown(t_pkmn->get_original_game_id());
            blockC->sinnoh_ribbons3 = rib.sinnoh.ribbons3;

            // Block D
            if(generation == 4) export_gen4_text(t_pkmn->get_trainer_name(), blockD->otname, 8);
            else export_modern_text(t_pkmn->get_trainer_name(), blockD->otname, 8);
            // TODO: met, Pokerus
            blockD->ball = libpkmn_ball_to_game_ball(reverse_ball_dict[t_pkmn->get_ball()]);
            set_gen_456_met_level(&blockD->metlevel_otgender, t_pkmn->get_met_level());
            set_gen_456_otgender(&blockD->metlevel_otgender, (t_pkmn->get_gender() == "Female"));

            pkmn.personality = t_pkmn->get_personality();
            // TODO: isdecrypted, checksum
        }

        void export_nds_pokemon(team_pokemon::sptr t_pkmn, nds_party_pokemon_t &pkmn,
                                bool encrypt)
        {
            export_nds_pokemon(t_pkmn, pkmn.pc, encrypt);
        }

        /*
         * Generation V
         */
        team_pokemon::sptr import_gen5_pokemon(pkmds::party_pkm* p_pkm)
        {
            pkmds::opendb(get_database_path().c_str());

            unsigned int species_id = database::get_species_id(database::get_pokemon_id(p_pkm->species_int,
                                                                                        Versions::BLACK));

            team_pokemon::sptr t_pkmn = team_pokemon::make(species_id, Versions::BLACK, p_pkm->party_data.level,
                                        p_pkm->moves[0], p_pkm->moves[1],
                                        p_pkm->moves[2], p_pkm->moves[3]);

            t_pkmn->set_original_game(hometown_to_libpkmn_game(p_pkm->hometown));
            t_pkmn->set_nickname(import_modern_text(p_pkm->nickname, 11));
            t_pkmn->set_trainer_name(import_modern_text(p_pkm->otname, 8));

            t_pkmn->set_move_PP(p_pkm->pp[0], 1);
            t_pkmn->set_move_PP(p_pkm->pp[1], 2);
            t_pkmn->set_move_PP(p_pkm->pp[2], 3);
            t_pkmn->set_move_PP(p_pkm->pp[3], 4);

            uint8_t* metlevel_int = reinterpret_cast<uint8_t*>(&(p_pkm->ball)+1);
            t_pkmn->set_met_level(get_gen_456_met_level(metlevel_int));
            t_pkmn->set_ball(ball_dict.at(game_ball_to_libpkmn_ball(p_pkm->ball), "Poke Ball"));
            if(get_gen_456_otgender(metlevel_int)) t_pkmn->set_trainer_gender("Female");
            else t_pkmn->set_trainer_gender("Male");

            t_pkmn->set_held_item(item::make(database::get_item_id(p_pkm->item, Versions::BLACK), Versions::BLACK));
            t_pkmn->set_personality(p_pkm->pid);
            t_pkmn->set_trainer_public_id(p_pkm->tid);
            t_pkmn->set_trainer_secret_id(p_pkm->sid);
            t_pkmn->set_experience(p_pkm->exp);

            t_pkmn->set_EV("HP", p_pkm->evs.hp);
            t_pkmn->set_EV("Attack", p_pkm->evs.attack);
            t_pkmn->set_EV("Defense", p_pkm->evs.defense);
            t_pkmn->set_EV("Special Attack", p_pkm->evs.spatk);
            t_pkmn->set_EV("Special Defense", p_pkm->evs.spdef);
            t_pkmn->set_EV("Speed", p_pkm->evs.speed);

            uint32_t* ivs = reinterpret_cast<uint32_t*>(&(p_pkm->ivs));
            t_pkmn->set_IV("HP", modern_get_IV(ivs, Stats::HP));
            t_pkmn->set_IV("Attack", modern_get_IV(ivs, Stats::ATTACK));
            t_pkmn->set_IV("Defense", modern_get_IV(ivs, Stats::DEFENSE));
            t_pkmn->set_IV("Special Attack", modern_get_IV(ivs, Stats::SPECIAL_ATTACK));
            t_pkmn->set_IV("Special Defense", modern_get_IV(ivs, Stats::SPECIAL_DEFENSE));
            t_pkmn->set_IV("Speed", modern_get_IV(ivs, Stats::SPEED));

            t_pkmn->set_markings(p_pkm->markings_int);

            //Attributes
            t_pkmn->set_attribute("country", p_pkm->country);
            t_pkmn->set_attribute("cool", p_pkm->contest.cool);
            t_pkmn->set_attribute("beauty", p_pkm->contest.beauty);
            t_pkmn->set_attribute("cute", p_pkm->contest.cute);
            t_pkmn->set_attribute("smart", p_pkm->contest.smart);
            t_pkmn->set_attribute("tough", p_pkm->contest.tough);
            t_pkmn->set_attribute("sheen", p_pkm->contest.sheen);

            uint32_t* hribbon1 = reinterpret_cast<uint32_t*>(&(p_pkm->hribbon1));
            t_pkmn->set_attribute("hoenn_cool_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::COOL));
            t_pkmn->set_attribute("hoenn_cool_super_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::COOL_SUPER));
            t_pkmn->set_attribute("hoenn_cool_hyper_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::COOL_HYPER));
            t_pkmn->set_attribute("hoenn_cool_master_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::COOL_MASTER));
            t_pkmn->set_attribute("hoenn_beauty_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY));
            t_pkmn->set_attribute("hoenn_beauty_super_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY_SUPER));
            t_pkmn->set_attribute("hoenn_beauty_hyper_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY_HYPER));
            t_pkmn->set_attribute("hoenn_beauty_master_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY_MASTER));
            t_pkmn->set_attribute("hoenn_cute_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::CUTE));
            t_pkmn->set_attribute("hoenn_cute_super_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::CUTE_SUPER));
            t_pkmn->set_attribute("hoenn_cute_hyper_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::CUTE_HYPER));
            t_pkmn->set_attribute("hoenn_cute_master_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::CUTE_MASTER));
            t_pkmn->set_attribute("hoenn_smart_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::SMART));
            t_pkmn->set_attribute("hoenn_smart_super_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::SMART_SUPER));
            t_pkmn->set_attribute("hoenn_smart_hyper_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::SMART_HYPER));
            t_pkmn->set_attribute("hoenn_smart_master_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::SMART_MASTER));
            t_pkmn->set_attribute("hoenn_tough_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::TOUGH));
            t_pkmn->set_attribute("hoenn_tough_super_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::TOUGH_SUPER));
            t_pkmn->set_attribute("hoenn_tough_hyper_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::TOUGH_HYPER));
            t_pkmn->set_attribute("hoenn_tough_master_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::TOUGH_MASTER));
            t_pkmn->set_attribute("hoenn_champion_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::CHAMPION));
            t_pkmn->set_attribute("hoenn_winning_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::WINNING));
            t_pkmn->set_attribute("hoenn_victory_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::VICTORY));
            t_pkmn->set_attribute("hoenn_artist_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::ARTIST));
            t_pkmn->set_attribute("hoenn_effort_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::EFFORT));
            t_pkmn->set_attribute("hoenn_marine_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::MARINE));
            t_pkmn->set_attribute("hoenn_land_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::LAND));
            t_pkmn->set_attribute("hoenn_sky_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::SKY));
            t_pkmn->set_attribute("hoenn_country_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::COUNTRY));
            t_pkmn->set_attribute("hoenn_national_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::NATIONAL));
            t_pkmn->set_attribute("hoenn_earth_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::EARTH));
            t_pkmn->set_attribute("hoenn_world_ribbon", get_ribbon(hribbon1, Ribbons::Hoenn::WORLD));

            uint32_t* sribbon1 = reinterpret_cast<uint32_t*>(&(p_pkm->sribbon1));
            t_pkmn->set_attribute("sinnoh_champion_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::CHAMPION));
            t_pkmn->set_attribute("sinnoh_ability_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::ABILITY));
            t_pkmn->set_attribute("sinnoh_great_ability_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::GREAT_ABILITY));
            t_pkmn->set_attribute("sinnoh_double_ability_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::DOUBLE_ABILITY));
            t_pkmn->set_attribute("sinnoh_multi_ability_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::MULTI_ABILITY));
            t_pkmn->set_attribute("sinnoh_pair_ability_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::PAIR_ABILITY));
            t_pkmn->set_attribute("sinnoh_world_ability_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::WORLD_ABILITY));
            t_pkmn->set_attribute("sinnoh_alert_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::ALERT));
            t_pkmn->set_attribute("sinnoh_shock_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::SHOCK));
            t_pkmn->set_attribute("sinnoh_downcast_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::DOWNCAST));
            t_pkmn->set_attribute("sinnoh_careless_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::CARELESS));
            t_pkmn->set_attribute("sinnoh_relax_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::RELAX));
            t_pkmn->set_attribute("sinnoh_snooze_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::SNOOZE));
            t_pkmn->set_attribute("sinnoh_smile_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::SMILE));
            t_pkmn->set_attribute("sinnoh_gorgeous_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::GORGEOUS));
            t_pkmn->set_attribute("sinnoh_royal_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::ROYAL));
            t_pkmn->set_attribute("sinnoh_gorgeous_royal_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::GORGEOUS_ROYAL));
            t_pkmn->set_attribute("sinnoh_footprint_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::FOOTPRINT));
            t_pkmn->set_attribute("sinnoh_record_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::RECORD));
            t_pkmn->set_attribute("sinnoh_history_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::HISTORY));
            t_pkmn->set_attribute("sinnoh_legend_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::LEGEND));
            t_pkmn->set_attribute("sinnoh_red_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::RED));
            t_pkmn->set_attribute("sinnoh_green_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::GREEN));
            t_pkmn->set_attribute("sinnoh_blue_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::BLUE));
            t_pkmn->set_attribute("sinnoh_festival_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::FESTIVAL));
            t_pkmn->set_attribute("sinnoh_carnival_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::FESTIVAL));
            t_pkmn->set_attribute("sinnoh_classic_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::CLASSIC));
            t_pkmn->set_attribute("sinnoh_premier_ribbon", get_ribbon(sribbon1, Ribbons::Sinnoh::PREMIER));

            uint32_t* sribbon3 = reinterpret_cast<uint32_t*>(&(p_pkm->sribbon3));
            t_pkmn->set_attribute("sinnoh_cool_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::COOL-23));
            t_pkmn->set_attribute("sinnoh_cool_super_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::COOL_SUPER-23));
            t_pkmn->set_attribute("sinnoh_cool_hyper_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::COOL_SUPER-23));
            t_pkmn->set_attribute("sinnoh_cool_master_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::COOL_MASTER-23));
            t_pkmn->set_attribute("sinnoh_beauty_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::BEAUTY-23));
            t_pkmn->set_attribute("sinnoh_beauty_super_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::BEAUTY_SUPER-23));
            t_pkmn->set_attribute("sinnoh_beauty_hyper_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::BEAUTY_SUPER-23));
            t_pkmn->set_attribute("sinnoh_beauty_master_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::BEAUTY_MASTER-23));
            t_pkmn->set_attribute("sinnoh_cute_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::CUTE-23));
            t_pkmn->set_attribute("sinnoh_cute_super_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::CUTE_SUPER-23));
            t_pkmn->set_attribute("sinnoh_cute_hyper_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::CUTE_SUPER-23));
            t_pkmn->set_attribute("sinnoh_cute_master_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::CUTE_MASTER-23));
            t_pkmn->set_attribute("sinnoh_smart_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::SMART-23));
            t_pkmn->set_attribute("sinnoh_smart_super_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::SMART_SUPER-23));
            t_pkmn->set_attribute("sinnoh_smart_hyper_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::SMART_SUPER-23));
            t_pkmn->set_attribute("sinnoh_smart_master_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::SMART_MASTER-23));
            t_pkmn->set_attribute("sinnoh_tough_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::TOUGH-23));
            t_pkmn->set_attribute("sinnoh_tough_super_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::TOUGH_SUPER-23));
            t_pkmn->set_attribute("sinnoh_tough_hyper_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::TOUGH_SUPER-23));
            t_pkmn->set_attribute("sinnoh_tough_master_ribbon", get_ribbon(sribbon3, Ribbons::Sinnoh::TOUGH_MASTER-23));

            t_pkmn->set_attribute("eggmet_year", p_pkm->metdate.year);
            t_pkmn->set_attribute("eggmet_month", p_pkm->metdate.month);
            t_pkmn->set_attribute("eggmet_day", p_pkm->metdate.day);
            t_pkmn->set_attribute("met_year", p_pkm->metdate.year);
            t_pkmn->set_attribute("met_month", p_pkm->metdate.month);
            t_pkmn->set_attribute("met_day", p_pkm->metdate.day);

            pkmds::closedb();

            return t_pkmn;
        }

        void export_gen5_pokemon(team_pokemon::sptr t_pkmn, pkmds::party_pkm* p_pkm)
        {
            p_pkm->species = pkmds::Species::species(
                             database::get_pokemon_game_index(t_pkmn->get_pokemon_id(),
                             t_pkmn->get_game_id()));

            moveset_t moves;
            t_pkmn->get_moves(moves);
            p_pkm->moves[0] = pkmds::Moves::moves(moves[0]->get_move_id());
            p_pkm->moves[1] = pkmds::Moves::moves(moves[1]->get_move_id());
            p_pkm->moves[2] = pkmds::Moves::moves(moves[2]->get_move_id());
            p_pkm->moves[3] = pkmds::Moves::moves(moves[3]->get_move_id());

            std::vector<unsigned int> move_PPs;
            t_pkmn->get_move_PPs(move_PPs);
            p_pkm->pp[0] = move_PPs[0];
            p_pkm->pp[1] = move_PPs[1];
            p_pkm->pp[2] = move_PPs[2];
            p_pkm->pp[3] = move_PPs[3];

            pkmds::setlevel(p_pkm, t_pkmn->get_level());

            export_modern_text(t_pkmn->get_nickname(), p_pkm->nickname, 11);
            export_modern_text(t_pkmn->get_nickname(), p_pkm->otname, 8);

            unsigned int raw_held = t_pkmn->get_held_item()->get_item_id();
            p_pkm->item = pkmds::Items::items(database::get_item_game_index(raw_held, t_pkmn->get_game_id()));

            uint8_t* metlevel_int = reinterpret_cast<uint8_t*>(&(p_pkm->ball)+1);
            set_gen_456_met_level(metlevel_int, t_pkmn->get_met_level());
            p_pkm->ball = pkmds::Balls::balls(libpkmn_ball_to_game_ball(reverse_ball_dict.at(t_pkmn->get_ball(), PokeBalls::POKE_BALL)));
            set_gen_456_otgender(metlevel_int, (t_pkmn->get_trainer_gender().std_string() == "Female"));
            p_pkm->pid = t_pkmn->get_personality();
            p_pkm->tid = t_pkmn->get_trainer_public_id();
            p_pkm->sid = t_pkmn->get_trainer_secret_id();
            p_pkm->exp = t_pkmn->get_experience();

            pkmn::dict<pkmn::pkstring, unsigned int> stats = t_pkmn->get_stats();
            p_pkm->party_data.maxhp = stats["HP"];
            p_pkm->party_data.attack = stats["Attack"];
            p_pkm->party_data.defense = stats["Defense"];
            p_pkm->party_data.spatk = stats["Special Attack"];
            p_pkm->party_data.spdef = stats["Special Defense"];
            p_pkm->party_data.speed = stats["Speed"];

            pkmn::dict<pkmn::pkstring, unsigned int> IVs = t_pkmn->get_IVs();
            uint32_t* IVint = reinterpret_cast<uint32_t*>(&(p_pkm->ppup[3])+1);
            modern_set_IV(IVint, Stats::HP, IVs["HP"]);
            modern_set_IV(IVint, Stats::ATTACK, IVs["Attack"]);
            modern_set_IV(IVint, Stats::DEFENSE, IVs["Defense"]);
            modern_set_IV(IVint, Stats::SPECIAL_ATTACK, IVs["Special Attack"]);
            modern_set_IV(IVint, Stats::SPECIAL_DEFENSE, IVs["Special Defense"]);
            modern_set_IV(IVint, Stats::SPEED, IVs["Speed"]);

            pkmn::dict<pkmn::pkstring, unsigned int> EVs = t_pkmn->get_EVs();
            p_pkm->evs.hp = EVs["HP"];
            p_pkm->evs.attack = EVs["Attack"];
            p_pkm->evs.defense = EVs["Defense"];
            p_pkm->evs.spatk = EVs["Special Attack"];
            p_pkm->evs.spdef = EVs["Special Defense"];
            p_pkm->evs.speed = EVs["Speed"];

            p_pkm->hometown = pkmds::Hometowns::hometowns(libpkmn_game_to_hometown(t_pkmn->get_original_game_id()));
            p_pkm->markings_int = t_pkmn->get_markings();

            //Attributes
            pkmn::dict<pkmn::pkstring, int> attributes = t_pkmn->get_attributes();
            p_pkm->tameness = attributes.at("friendship",0);
            p_pkm->country = pkmds::Countries::countries(attributes.at("country",2)); //Default to English
            p_pkm->contest.cool = attributes.at("cool",false);
            p_pkm->contest.beauty = attributes.at("beauty",false);
            p_pkm->contest.cute = attributes.at("cute",false);
            p_pkm->contest.smart = attributes.at("smart",false);
            p_pkm->contest.tough = attributes.at("tough",false);
            p_pkm->contest.sheen = attributes.at("sheen",false);

            uint32_t* hribbon1 = reinterpret_cast<uint32_t*>(&(p_pkm->hribbon1));
            set_ribbon(hribbon1, Ribbons::Hoenn::COOL, attributes.at("hoenn_cool_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::COOL_SUPER, attributes.at("hoenn_cool_super_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::COOL_HYPER, attributes.at("hoenn_cool_hyper_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::COOL_MASTER, attributes.at("hoenn_cool_master_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY, attributes.at("hoenn_beauty_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY_SUPER, attributes.at("hoenn_beauty_super_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY_HYPER, attributes.at("hoenn_beauty_hyper_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::BEAUTY_MASTER, attributes.at("hoenn_beauty_master_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::CUTE, attributes.at("hoenn_cute_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::CUTE_SUPER, attributes.at("hoenn_cute_super_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::CUTE_HYPER, attributes.at("hoenn_cute_hyper_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::CUTE_MASTER, attributes.at("hoenn_cute_master_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::SMART, attributes.at("hoenn_smart_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::SMART_SUPER, attributes.at("hoenn_smart_super_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::SMART_HYPER, attributes.at("hoenn_smart_hyper_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::SMART_MASTER, attributes.at("hoenn_smart_master_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::TOUGH, attributes.at("hoenn_tough_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::TOUGH_SUPER, attributes.at("hoenn_tough_super_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::TOUGH_HYPER, attributes.at("hoenn_tough_hyper_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::TOUGH_MASTER, attributes.at("hoenn_tough_master_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::CHAMPION, attributes.at("hoenn_champion_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::WINNING, attributes.at("hoenn_winning_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::VICTORY, attributes.at("hoenn_victory_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::ARTIST, attributes.at("hoenn_artist_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::EFFORT, attributes.at("hoenn_effort_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::MARINE, attributes.at("hoenn_marine_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::LAND, attributes.at("hoenn_land_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::SKY, attributes.at("hoenn_sky_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::COUNTRY, attributes.at("hoenn_country_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::NATIONAL, attributes.at("hoenn_national_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::EARTH, attributes.at("hoenn_earth_ribbon",false));
            set_ribbon(hribbon1, Ribbons::Hoenn::WORLD, attributes.at("hoenn_world_ribbon",false));

            uint32_t* sribbon1 = reinterpret_cast<uint32_t*>(&(p_pkm->sribbon1));
            set_ribbon(sribbon1, Ribbons::Sinnoh::CHAMPION, attributes.at("sinnoh_champion_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::ABILITY, attributes.at("sinnoh_ability_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::GREAT_ABILITY, attributes.at("sinnoh_great_ability_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::DOUBLE_ABILITY, attributes.at("sinnoh_double_ability_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::MULTI_ABILITY, attributes.at("sinnoh_multi_ability_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::PAIR_ABILITY, attributes.at("sinnoh_pair_ability_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::WORLD_ABILITY, attributes.at("sinnoh_ability_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::ALERT, attributes.at("sinnoh_alert_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::SHOCK, attributes.at("sinnoh_shock_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::DOWNCAST, attributes.at("sinnoh_downcast_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::CARELESS, attributes.at("sinnoh_careless_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::RELAX, attributes.at("sinnoh_relax_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::SNOOZE, attributes.at("sinnoh_snooze_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::SMILE, attributes.at("sinnoh_smile_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::GORGEOUS, attributes.at("sinnoh_gorgeous_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::ROYAL, attributes.at("sinnoh_royal_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::GORGEOUS_ROYAL, attributes.at("sinnoh_gorgeous_royal_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::RECORD, attributes.at("sinnoh_record_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::HISTORY, attributes.at("sinnoh_history_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::LEGEND, attributes.at("sinnoh_legend_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::RED, attributes.at("sinnoh_red_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::GREEN, attributes.at("sinnoh_green_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::BLUE, attributes.at("sinnoh_blue_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::FESTIVAL, attributes.at("sinnoh_festival_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::CARNIVAL, attributes.at("sinnoh_carnival_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::CLASSIC, attributes.at("sinnoh_classic_ribbon",false));
            set_ribbon(sribbon1, Ribbons::Sinnoh::PREMIER, attributes.at("sinnoh_premier_ribbon",false));

            uint32_t* sribbon3 = reinterpret_cast<uint32_t*>(&(p_pkm->sribbon3));
            set_ribbon(sribbon3, Ribbons::Sinnoh::COOL-23, attributes.at("sinnoh_cool_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::COOL_SUPER-23, attributes.at("sinnoh_cool_super_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::COOL_HYPER-23, attributes.at("sinnoh_cool_hyper_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::COOL_MASTER-23, attributes.at("sinnoh_cool_master_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::BEAUTY-23, attributes.at("sinnoh_beauty_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::BEAUTY_SUPER-23, attributes.at("sinnoh_beauty_super_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::BEAUTY_HYPER-23, attributes.at("sinnoh_beauty_hyper_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::BEAUTY_MASTER-23, attributes.at("sinnoh_beauty_master_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::CUTE-23, attributes.at("sinnoh_cute_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::CUTE_SUPER-23, attributes.at("sinnoh_cute_super_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::CUTE_HYPER-23, attributes.at("sinnoh_cute_hyper_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::CUTE_MASTER-23, attributes.at("sinnoh_cute_master_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::SMART-23, attributes.at("sinnoh_smart_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::SMART_SUPER-23, attributes.at("sinnoh_smart_super_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::SMART_HYPER-23, attributes.at("sinnoh_smart_hyper_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::SMART_MASTER-23, attributes.at("sinnoh_smart_master_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::TOUGH-23, attributes.at("sinnoh_tough_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::TOUGH_SUPER-23, attributes.at("sinnoh_tough_super_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::TOUGH_HYPER-23, attributes.at("sinnoh_tough_hyper_ribbon",false));
            set_ribbon(sribbon3, Ribbons::Sinnoh::TOUGH_MASTER-23, attributes.at("sinnoh_tough_master_ribbon",false));

            p_pkm->eggdate.year = attributes.at("eggmet_year",0);
            p_pkm->eggdate.month = attributes.at("eggmet_month",0);
            p_pkm->eggdate.day = attributes.at("eggmet_day",0);
            p_pkm->metdate.year = attributes.at("met_year",0);
            p_pkm->metdate.month = attributes.at("met_month",0);
            p_pkm->metdate.day = attributes.at("met_day",0);
        }
    } /* namespace conversions */
} /* namespace pkmn */
