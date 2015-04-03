/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include <pkmn/database.hpp>

#include "pksql.hpp"

namespace pkmn
{
    namespace io
    {
        uint64_t pksql::latest_id = 1;

        std::string pksql::create_tables()
        {
            std::ostringstream table_stream;
            table_stream << "CREATE TABLE pkmn (\n"
                         << "    id                    INTEGER NOT NULL,\n"
                         << "    pksql_compat_num      INTEGER NOT NULL,\n"
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
                         << "    kalos_ribbons         INTEGER NOT NULL\n"
                         << ");\n";

            table_stream << "CREATE TABLE pkmn_attributes (\n"
                         << "    pkmn_id INTEGER NOT NULL,\n"
                         << "    name  VARCHAR(20) NOT NULL,\n"
                         << "    value INTEGER NOT NULL\n"
                         << ");\n";

            return table_stream.str();
        }

        std::string pksql::query(pokemon::sptr pkmn, uint64_t id)
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

            std::ostringstream query_stream;

            query_stream << "INSERT INTO \"pkmn\" VALUES("
                         << id << ","
                         << PKSQL_COMPAT_NUM << ","
                         << pkmn->get_pokemon_id() << ","
                         << pkmn->get_game_id() << ","
                         << "'" << pkmn->get_nickname() << "',"
                         << "'" << pkmn->get_trainer_name() << "',"
                         << (pkmn->get_trainer_gender().std_string() == "Female" ? 0 : 1) << ","
                         << pkmn->get_trainer_id() << ","
                         << database::get_ball_id(pkmn->get_ball()) << ","
                         << pkmn->get_original_game_id() << ","
                         << pkmn->get_met_level() << ","
                         << pkmn->get_personality() << ","
                         << int(pkmn->get_friendship()) << ","
                         << int(pkmn->get_level()) << ","
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
                         << IVs[(pkmn->get_generation() > 2) ? "Special Attack" : "Special"] << ","
                         << ((pkmn->get_generation() > 2) ? IVs["Special Defense"] : 0) << ","
                         << pkmn->get_item_id() << ","
                         << database::get_move_id(moves[0].name) << ","
                         << database::get_move_id(moves[1].name) << ","
                         << database::get_move_id(moves[2].name) << ","
                         << database::get_move_id(moves[3].name) << ","
                         << int(move_PPs[0]) << ","
                         << int(move_PPs[1]) << ","
                         << int(move_PPs[2]) << ","
                         << int(move_PPs[3]) << ","
                         << int(contest_stats.coolness) << ","
                         << int(contest_stats.beauty) << ","
                         << int(contest_stats.cuteness) << ","
                         << int(contest_stats.smartness) << ","
                         << int(contest_stats.toughness) << ","
                         << int(contest_stats.feel) << "," // Covers sheen
                         << int(pkmn->get_markings()) << ","
                         << uint32_t(pkmn->get_super_training_medals()) << ","
                         << int(pkmn->get_pokerus()) << ","
                         << uint32_t(ribbons.hoenn) << ","
                         << uint16_t(ribbons.sinnoh.ribbons1) << ","
                         << uint16_t(ribbons.sinnoh.ribbons2) << ","
                         << uint32_t(ribbons.sinnoh.ribbons3) << ","
                         << uint16_t(ribbons.unova) << ","
                         << uint64_t(ribbons.kalos) << ");\n";

            BOOST_FOREACH(const pkmn::pkstring &key, attributes.keys())
            {
                query_stream << "INSERT INTO \"pkmn_attributes\" VALUES("
                             << key << "," << attributes.at(key) << ");\n";
            }

            if(id == pksql::latest_id) pksql::latest_id++;

            return query_stream.str();
        }

        bool pksql::valid(pkmn::database_sptr db)
        {
            // Check for correct tables
            if(not db->tableExists("pkmn") or not db->tableExists("pkmn_attributes"))
                return false;

            // Check for at least one entry in "pkmn"
            SQLite::Statement pkmn_query(*db, "SELECT * FROM pkmn");
            if(not pkmn_query.executeStep())
                return false;

            // Check for valid PKSQL compatibility number
            // Serious enough problem to throw error
            int id = db->execAndGet("SELECT pksql_compat_num FROM pkmn");
            if(id != PKSQL_COMPAT_NUM)
                throw std::runtime_error("Invalid PKSQL compatibility number.");

            // Check for correct number of columns
            if(pkmn_query.getColumnCount() != 54)
                return false;

            return true;
        }

        bool pksql::valid(const pkmn::pkstring &filename)
        {
            pkmn::database_sptr pksql;

            try {pksql = pkmn::database_sptr(new SQLite::Database(filename));}
            catch(...) {return false;}

            return pksql::valid(pksql);

            return true;
        }

        pokemon::sptr pksql::from(pkmn::database_sptr db, uint64_t id)
        {
            SQLite::Statement import_query(*db, str(boost::format("SELECT * FROM pkmn WHERE id=%d")
                                                    % id).c_str());
            import_query.executeStep();

            pokemon::sptr pkmn = pokemon::make(
                                     database::get_species_id(int(import_query.getColumn(2))), // pokemon_id
                                     import_query.getColumn(3),  // game_id
                                     import_query.getColumn(13), // level
                                     import_query.getColumn(31), // move1_id
                                     import_query.getColumn(32), // move2_id
                                     import_query.getColumn(33), // move3_id
                                     import_query.getColumn(34)  // move4_id
                                 );

            pkmn->set_nickname(import_query.getColumn(4)); // nickname
            pkmn->set_trainer_name(import_query.getColumn(5)); // otname
            pkmn->set_trainer_gender((int(import_query.getColumn(6)) > 0) ? "Female" : "Male"); // female
            pkmn->set_trainer_id(import_query.getColumn(7)); // ot_id
            pkmn->set_ball(database::get_ball_name(import_query.getColumn(8))); // ball_id
            pkmn->set_original_game(database::get_version_name(import_query.getColumn(9))); // original_game_id
            pkmn->set_met_level(import_query.getColumn(10)); // met_level
            pkmn->set_personality(import_query.getColumn(11)); // personality
            pkmn->set_friendship(import_query.getColumn(12)); // friendship
            pkmn->set_level(import_query.getColumn(13)); // level
            pkmn->set_experience(import_query.getColumn(14)); // experience
            if(pkmn->get_pokedex_entry().chance_male + pkmn->get_pokedex_entry().chance_female > 0)
                pkmn->set_gender((int(import_query.getColumn(15)) > 0) ? "Female" : "Male"); // female
            pkmn->set_nature(database::get_nature_name(import_query.getColumn(16))); // nature_id
            pkmn->set_ability(database::get_ability_name(import_query.getColumn(17))); // ability_id
            pkmn->set_EV("HP", import_query.getColumn(18)); // ev_hp
            pkmn->set_EV("Attack", import_query.getColumn(19)); // ev_attack
            pkmn->set_EV("Defense", import_query.getColumn(20)); // ev_defense
            pkmn->set_EV("Speed", import_query.getColumn(21)); // ev_speed
            pkmn->set_EV((pkmn->get_generation() > 2) ? "Special Attack" : "Special", import_query.getColumn(22)); // ev_spatk
            if(pkmn->get_generation() > 2)
                pkmn->set_EV("Special Defense", import_query.getColumn(23)); // ev_spdef
            pkmn->set_IV("HP", import_query.getColumn(24)); // iv_hp
            pkmn->set_IV("Attack", import_query.getColumn(25)); // iv_attack
            pkmn->set_IV("Defense", import_query.getColumn(26)); // iv_defense
            pkmn->set_IV("Speed", import_query.getColumn(27)); // iv_speed
            pkmn->set_IV((pkmn->get_generation() > 2) ? "Special Attack" : "Special", import_query.getColumn(28)); // iv_spatk
            if(pkmn->get_generation() > 2)
                pkmn->set_IV("Special Defense", import_query.getColumn(29)); // iv_spdef
            pkmn->set_held_item(database::get_item_name(import_query.getColumn(30))); // held_item_id
            pkmn->set_move_PP(import_query.getColumn(35), 1); // move1_pp
            pkmn->set_move_PP(import_query.getColumn(36), 2); // move2_pp
            pkmn->set_move_PP(import_query.getColumn(37), 3); // move3_pp
            pkmn->set_move_PP(import_query.getColumn(38), 4); // move4_pp

            pkmn::contest_stats_t contest_stats;
            contest_stats.coolness  = import_query.getColumn(39); // coolness
            contest_stats.beauty    = import_query.getColumn(40); // coolness
            contest_stats.cuteness  = import_query.getColumn(41); // cuteness
            contest_stats.smartness = import_query.getColumn(42); // smartness
            contest_stats.toughness = import_query.getColumn(43); // toughness
            contest_stats.feel      = import_query.getColumn(44); // feel_sheen
            pkmn->set_contest_stats(contest_stats);

            pkmn->set_markings(uint8_t(import_query.getColumn(45))); // markings
            pkmn->set_super_training_medals(uint32_t(import_query.getColumn(46))); // super_training_medals
            pkmn->set_pokerus(uint8_t(import_query.getColumn(47))); // pokerus

            pkmn::ribbons_t ribbons;
            ribbons.hoenn           = uint32_t(import_query.getColumn(48)); // hoenn_ribbons
            ribbons.sinnoh.ribbons1 = uint16_t(import_query.getColumn(49)); // sinnoh_ribbons1
            ribbons.sinnoh.ribbons2 = uint16_t(import_query.getColumn(50)); // sinnoh_ribbons2
            ribbons.sinnoh.ribbons3 = uint32_t(import_query.getColumn(51)); // sinnoh_ribbons3
            ribbons.unova           = uint16_t(import_query.getColumn(52)); // unova_ribbons
            ribbons.kalos           = uint64_t(import_query.getColumn(53)); // kalos_ribbons
            pkmn->set_ribbons(ribbons);

            SQLite::Statement attribute_query(*db, str(boost::format("SELECT * FROM pkmn_attributes WHERE pkmn_id=%d")
                                                       % id).c_str());
            while(attribute_query.executeStep())
                pkmn->set_attribute(attribute_query.getColumn(0), attribute_query.getColumn(1));

            if(id > pksql::latest_id)
                pksql::latest_id = id;

            return pkmn;
        }

        pokemon::sptr pksql::from(const pkmn::pkstring &filename)
        {
            // Validate input file
            if(not pksql::valid(filename))
                throw std::runtime_error("Invalid PKSQL file.");

            pkmn::database_sptr db(new SQLite::Database(filename));
            return pksql::from(db, uint64_t(db->execAndGet("SELECT id FROM pkmn")));
        }

        void pksql::to(pokemon::sptr pkmn, pkmn::database_sptr db)
        {
            if(not db->tableExists("pkmn") and not db->tableExists("pkmn_attributes"))
                db->exec(pksql::create_tables().c_str());

            db->exec(pksql::query(pkmn).c_str());
        }

        pkmn::database_sptr pksql::to(pokemon::sptr pkmn, const pkmn::pkstring &filename)
        {
            pkmn::database_sptr db(new SQLite::Database(filename, (SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE)));
            pksql::to(pkmn, db);

            return db;
        }
    }
}
