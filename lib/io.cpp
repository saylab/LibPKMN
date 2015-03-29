/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cstdint>
#include <fstream>
#include <cstring>
#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

#include <pkmn/database.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/io.hpp>
#include <pkmn/conversions/pokemon.hpp>

#include "SQLiteCpp/SQLiteC++.h"

namespace fs = boost::filesystem;

namespace pkmn
{
    namespace io
    {
        //For now, everything is Ruby until there's a better way
        pokemon::sptr import_from_3gpkm(const pkmn::pkstring &filename)
        {
            uint32_t filesize = uint32_t(fs::file_size(fs::path(filename)));
            std::ifstream ifile;

            if(filesize == sizeof(native::gen3_pc_pokemon_t))
            {
                native::gen3_pc_pokemon_t native_pkmn;
                ifile.open(filename.const_char(), std::ifstream::in | std::ifstream::binary);
                ifile.read((char*)&native_pkmn, sizeof(native::gen3_pc_pokemon_t));
                ifile.close();

                return conversions::import_gen3_pokemon(native_pkmn, "Ruby", false);
            }
            else if(filesize == sizeof(native::gen3_party_pokemon_t))
            {
                native::gen3_party_pokemon_t native_pkmn;
                ifile.open(filename.const_char(), std::ifstream::in | std::ifstream::binary);
                ifile.read((char*)&native_pkmn, sizeof(native::gen3_party_pokemon_t));
                ifile.close();

                return conversions::import_gen3_pokemon(native_pkmn, "Ruby", false);
            }
            else throw std::runtime_error("This is not a valid .3gpkm file.");
        }

        void export_to_3gpkm(pokemon::sptr pkmn, const pkmn::pkstring &filename)
        {
            if(pkmn->get_generation() != 3)
                throw std::runtime_error("The given Pokémon is not from Generation III.");

            native::gen3_party_pokemon_t native_pkmn;
            conversions::export_gen3_pokemon(pkmn, native_pkmn, false);

            std::ofstream ofile(filename.const_char(), std::ofstream::out | std::ofstream::binary);
            ofile.write((char*)&native_pkmn, sizeof(native::gen3_party_pokemon_t));
            ofile.close();
        }

        pokemon::sptr import_from_pkm(const pkmn::pkstring &filename)
        {
            uint32_t filesize = uint8_t(fs::file_size(fs::path(filename)));
            std::ifstream ifile;

            if(filesize == sizeof(native::nds_pc_pokemon_t))
            {
                native::nds_pc_pokemon_t native_pkmn;
                memset(&native_pkmn, 0xFF, sizeof(native::nds_pc_pokemon_t));
                ifile.open(filename.const_char(), std::ifstream::in | std::ifstream::binary);
                ifile.read((char*)&native_pkmn, sizeof(native::nds_pc_pokemon_t));
                ifile.close();

                // TODO: use fields to distinguish
                return conversions::import_nds_pokemon(native_pkmn, "Diamond", false);
            }
            else if(filesize == sizeof(native::nds_party_pokemon_t)) // Generation IV
            {
                native::nds_party_pokemon_t native_pkmn;
                memset(&native_pkmn, 0xFF, sizeof(native::nds_party_pokemon_t));
                ifile.open(filename.const_char(), std::ifstream::in | std::ifstream::binary);
                ifile.read((char*)&native_pkmn, sizeof(native::nds_party_pokemon_t));
                ifile.close();

                // TODO: distinguish between versions
                return conversions::import_nds_pokemon(native_pkmn, "Diamond", false);
            }
            else if(filesize == 220) // Generation V
            {
                native::nds_party_pokemon_t native_pkmn;
                memset(&native_pkmn, 0xFF, sizeof(native::nds_party_pokemon_t));
                ifile.open(filename.const_char(), std::ifstream::in | std::ifstream::binary);
                ifile.read((char*)&native_pkmn, sizeof(native::nds_party_pokemon_t));
                ifile.close();

                // TODO: distinguish between versions
                return conversions::import_nds_pokemon(native_pkmn, "Black", false);
            }
            else throw std::runtime_error("This is not a valid .pkm file.");
        }

        void export_to_pkm(pokemon::sptr pkmn, const pkmn::pkstring &filename)
        {
            if(pkmn->get_generation() != 4 and pkmn->get_generation() != 5)
                throw std::runtime_error("The given Pokemon is not from Generation IV-V.");

            // TODO: Gen IV vs Gen V
            native::nds_party_pokemon_t native_pkmn;
            conversions::export_nds_pokemon(pkmn, native_pkmn, false);

            std::ofstream ofile(filename.const_char(), std::ofstream::out | std::ofstream::binary);
            ofile.write((char*)&native_pkmn, sizeof(native::nds_party_pokemon_t));
            ofile.close();
        }

        pokemon::sptr import_from_pksql(const pkmn::pkstring &filename)
        {
            pkmn::contest_stats_t contest_stats;
            pkmn::ribbons_t ribbons;

            SQLite::Database pksql(filename);
            SQLite::Statement import_query(pksql, "SELECT * FROM pkmn");
            import_query.executeStep();

            pokemon::sptr pkmn = pokemon::make(
                                     database::get_species_id(int(import_query.getColumn(0))), // pokemon_id
                                     import_query.getColumn(1),  // game_id
                                     import_query.getColumn(11), // level
                                     import_query.getColumn(29), // move1_id
                                     import_query.getColumn(30), // move2_id
                                     import_query.getColumn(31), // move3_id
                                     import_query.getColumn(32)  // move4_id
                                 );

            pkmn->set_nickname(import_query.getColumn(2)); // nickname
            pkmn->set_trainer_name(import_query.getColumn(3)); // otname
            pkmn->set_trainer_gender((int(import_query.getColumn(4)) > 0) ? "Female" : "Male"); // female
            pkmn->set_trainer_id(import_query.getColumn(5)); // ot_id
            pkmn->set_ball(database::get_ball_name(import_query.getColumn(6))); // ball_id
            pkmn->set_original_game(database::get_version_name(import_query.getColumn(7))); // original_game_id
            pkmn->set_met_level(import_query.getColumn(8)); // met_level
            pkmn->set_personality(import_query.getColumn(9)); // personality
            pkmn->set_friendship(import_query.getColumn(10)); // friendship
            pkmn->set_level(import_query.getColumn(11)); // level
            pkmn->set_experience(import_query.getColumn(12)); // experience
            if(pkmn->get_pokedex_entry().chance_male + pkmn->get_pokedex_entry().chance_female > 0)
                pkmn->set_gender((int(import_query.getColumn(13)) > 0) ? "Female" : "Male"); // female
            pkmn->set_nature(database::get_nature_name(import_query.getColumn(14))); // nature_id
            pkmn->set_ability(database::get_ability_name(import_query.getColumn(15))); // ability_id
            pkmn->set_EV("HP", import_query.getColumn(16)); // ev_hp
            pkmn->set_EV("Attack", import_query.getColumn(17)); // ev_attack
            pkmn->set_EV("Defense", import_query.getColumn(18)); // ev_defense
            pkmn->set_EV("Speed", import_query.getColumn(19)); // ev_speed
            pkmn->set_EV((pkmn->get_generation() > 2) ? "Special Attack" : "Special", import_query.getColumn(20)); // ev_spatk
            if(pkmn->get_generation() > 2)
                pkmn->set_EV("Special Defense", import_query.getColumn(21)); // ev_spdef
            pkmn->set_IV("HP", import_query.getColumn(22)); // iv_hp
            pkmn->set_IV("Attack", import_query.getColumn(23)); // iv_attack
            pkmn->set_IV("Defense", import_query.getColumn(24)); // iv_defense
            pkmn->set_IV("Speed", import_query.getColumn(25)); // iv_speed
            pkmn->set_IV((pkmn->get_generation() > 2) ? "Special Attack" : "Special", import_query.getColumn(26)); // iv_spatk
            if(pkmn->get_generation() > 2)
                pkmn->set_IV("Special Defense", import_query.getColumn(27)); // iv_spdef
            pkmn->set_held_item(database::get_item_name(import_query.getColumn(28))); // held_item_id
            pkmn->set_move_PP(import_query.getColumn(29), 1); // move1_pp
            pkmn->set_move_PP(import_query.getColumn(30), 2); // move2_pp
            pkmn->set_move_PP(import_query.getColumn(31), 3); // move3_pp
            pkmn->set_move_PP(import_query.getColumn(32), 4); // move4_pp

            contest_stats.coolness  = import_query.getColumn(33); // coolness
            contest_stats.cuteness  = import_query.getColumn(34); // cuteness
            contest_stats.smartness = import_query.getColumn(35); // smartness
            contest_stats.toughness = import_query.getColumn(36); // toughness
            contest_stats.feel      = import_query.getColumn(37); // feel_sheen
            pkmn->set_contest_stats(contest_stats);

            pkmn->set_markings(uint8_t(import_query.getColumn(38))); // markings
            pkmn->set_super_training_medals(uint32_t(import_query.getColumn(39))); // super_training_medals
            pkmn->set_pokerus(uint8_t(import_query.getColumn(40))); // pokerus

            ribbons.hoenn           = uint32_t(import_query.getColumn(41)); // hoenn_ribbons
            ribbons.sinnoh.ribbons1 = uint16_t(import_query.getColumn(42)); // sinnoh_ribbons1
            ribbons.sinnoh.ribbons2 = uint16_t(import_query.getColumn(43)); // sinnoh_ribbons2
            ribbons.sinnoh.ribbons3 = uint32_t(import_query.getColumn(44)); // sinnoh_ribbons3
            ribbons.unova           = uint16_t(import_query.getColumn(45)); // unova_ribbons
            ribbons.kalos           = uint64_t(import_query.getColumn(46)); // kalos_ribbons
            pkmn->set_ribbons(ribbons);

            SQLite::Statement attribute_query(pksql, "SELECT * FROM pkmn_attributes");
            while(attribute_query.executeStep())
                pkmn->set_attribute(attribute_query.getColumn(0), attribute_query.getColumn(1));

            return pkmn;
        }

        void export_to_pksql(pokemon::sptr pkmn, const pkmn::pkstring &filename)
        {
            pkmn::dict<pkmn::pkstring, uint16_t> EVs = pkmn->get_EVs();
            pkmn::dict<pkmn::pkstring, uint16_t> IVs = pkmn->get_IVs();
            pkmn::dict<pkmn::pkstring, int> attributes = pkmn->get_attributes();
            pkmn::contest_stats_t contest_stats = pkmn->get_contest_stats();
            pkmn::ribbons_t ribbons = pkmn->get_ribbons();
            pkmn::moveset_t moves;
            std::vector<uint8_t> move_PPs;
            pkmn->get_moves(moves);
            pkmn->get_move_PPs(move_PPs);

            SQLite::Database pksql(filename, SQLITE_OPEN_CREATE);

            std::ostringstream export_querystream;
            export_querystream << "CREATE TABLE pkmn (\n"
                               << "    pokemon_id            INTEGER NOT NULL,\n"
                               << "    game_id               INTEGER NOT NULL,\n"
                               << "    nickname              VARCHAR(20) NOT NULL,\n"
                               << "    otname                VARCHAR(20) NOT NULL,\n"
                               << "    ot_female             BOOLEAN NOT NULL,\n"
                               << "    ot_id                 INTEGER NOT NULL,\n"
                               << "    ball_id               INTEGER NOT NULL,\n"
                               << "    original_game_id      INTEGER NOT NULL,\n"
                               << "    met_level             INTEGER NOT NULL,\n"
                               << "    personality           INTEGER NOT NULL,\n"
                               << "    friendship            INTEGER NOT NULL,\n"
                               << "    level                 INTEGER NOT NULL,\n"
                               << "    experience            INTEGER NOT NULL,\n"
                               << "    female                BOOLEAN NOT NULL,\n"
                               << "    nature_id             INTEGER NOT NULL,\n"
                               << "    ability_id            INTEGER NOT NULL,\n"
                               << "    ev_hp                 INTEGER NOT NULL,\n"
                               << "    ev_attack             INTEGER NOT NULL,\n"
                               << "    ev_defense            INTEGER NOT NULL,\n"
                               << "    ev_speed              INTEGER NOT NULL,\n"
                               << "    ev_spatk              INTEGER NOT NULL,\n" // Special in Gen I
                               << "    ev_spdef              INTEGER NOT NULL,\n"
                               << "    iv_hp                 INTEGER NOT NULL,\n"
                               << "    iv_attack             INTEGER NOT NULL,\n"
                               << "    iv_defense            INTEGER NOT NULL,\n"
                               << "    iv_speed              INTEGER NOT NULL,\n"
                               << "    iv_spatk              INTEGER NOT NULL,\n" // Special in Gen I
                               << "    iv_spdef              INTEGER NOT NULL,\n"
                               << "    held_item_id          INTEGER NOT NULL,\n"
                               << "    move1_id              INTEGER NOT NULL,\n"
                               << "    move2_id              INTEGER NOT NULL,\n"
                               << "    move3_id              INTEGER NOT NULL,\n"
                               << "    move4_id              INTEGER NOT NULL,\n"
                               << "    move1_pp              INTEGER NOT NULL,\n"
                               << "    move2_pp              INTEGER NOT NULL,\n"
                               << "    move3_pp              INTEGER NOT NULL,\n"
                               << "    move4_pp              INTEGER NOT NULL,\n"
                               << "    coolness              INTEGER NOT NULL,\n"
                               << "    beauty                INTEGER NOT NULL,\n"
                               << "    cuteness              INTEGER NOT NULL,\n"
                               << "    smartness             INTEGER NOT NULL,\n"
                               << "    toughness             INTEGER NOT NULL,\n"
                               << "    feel_sheen            INTEGER NOT NULL,\n"
                               << "    markings              INTEGER NOT NULL,\n"
                               << "    super_training_medals INTEGER NOT NULL,\n"
                               << "    pokerus               INTEGER NOT NULL,\n"
                               << "    hoenn_ribbons         INTEGER NOT NULL,\n"
                               << "    sinnoh_ribbons1       INTEGER NOT NULL,\n"
                               << "    sinnoh_ribbons2       INTEGER NOT NULL,\n"
                               << "    sinnoh_ribbons3       INTEGER NOT NULL,\n"
                               << "    unova_ribbons         INTEGER NOT NULL,\n"
                               << "    kalos_ribbons         INTEGER NOT NULL,\n"
                               << ");\n";

            export_querystream << "INSERT INTO \"pkmn\" VALUES("
                               << pkmn->get_pokemon_id() << ","
                               << pkmn->get_game_id() << ","
                               << pkmn->get_nickname() << ","
                               << pkmn->get_trainer_name() << ","
                               << (pkmn->get_trainer_gender().std_string() == "Female" ? 0 : 1) << ","
                               << pkmn->get_trainer_id() << ","
                               << database::get_ball_id(pkmn->get_ball()) << ","
                               << pkmn->get_original_game_id() << ","
                               << pkmn->get_met_level() << ","
                               << pkmn->get_personality() << ","
                               << pkmn->get_friendship() << ","
                               << pkmn->get_level() << ","
                               << pkmn->get_experience() << ","
                               << (pkmn->get_gender().std_string() == "Female" ? 0 : 1) << ","
                               << pkmn->get_nature_id() << ","
                               << pkmn->get_ability_id() << ","
                               << EVs["HP"] << ","
                               << EVs["Attack"] << ","
                               << EVs["Defense"] << ","
                               << EVs["Speed"] << ","
                               << EVs[(pkmn->get_generation() > 2) ? "Special Attack" : "Special"] << ","
                               << ((pkmn->get_generation() > 2) ? EVs["Special Defense"] : 0) << ","
                               << IVs["HP"] << ","
                               << IVs["Attack"] << ","
                               << IVs["Defense"] << ","
                               << IVs["Speed"] << ","
                               << IVs["Special Attack"] << ","
                               << IVs["Special Defense"] << ","
                               << pkmn->get_item_id() << ","
                               << database::get_move_id(moves[0].name) << ","
                               << database::get_move_id(moves[1].name) << ","
                               << database::get_move_id(moves[2].name) << ","
                               << database::get_move_id(moves[3].name) << ","
                               << move_PPs[0] << ","
                               << move_PPs[1] << ","
                               << move_PPs[2] << ","
                               << move_PPs[3] << ","
                               << contest_stats.coolness << ","
                               << contest_stats.beauty << ","
                               << contest_stats.cuteness << ","
                               << contest_stats.smartness << ","
                               << contest_stats.toughness << ","
                               << contest_stats.feel << "," // Covers sheen
                               << uint8_t(pkmn->get_markings()) << ","
                               << uint32_t(pkmn->get_super_training_medals()) << ","
                               << uint32_t(ribbons.hoenn) << ","
                               << uint16_t(ribbons.sinnoh.ribbons1) << ","
                               << uint16_t(ribbons.sinnoh.ribbons2) << ","
                               << uint32_t(ribbons.sinnoh.ribbons3) << ","
                               << uint16_t(ribbons.unova) << ","
                               << uint64_t(ribbons.kalos) << ");\n";

            export_querystream << "CREATE TABLE pkmn_attributes (\n"
                               << "    name  VARCHAR(20) NOT NULL,\n"
                               << "    value INTEGER NOT NULL\n"
                               << ");\n";

            BOOST_FOREACH(const pkmn::pkstring &key, attributes.keys())
            {
                export_querystream << "INSERT INTO \"pkmn_attributes\" VALUES("
                                   << key << "," << attributes.at(key) << ");\n";
            } 

            pksql.exec(export_querystream.str().c_str());
        }
    }
}
