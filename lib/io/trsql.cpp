/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>

#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include <pkmn/database.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/conversions/pokemon.hpp>
#include <pkmn/conversions/text.hpp>
#include <pkmn/native/pokemon.hpp>

#include "pksql.hpp"
#include "trsql.hpp"

namespace pkmn
{
    namespace io
    {
        uint64_t trsql::latest_id = 1;

        void trsql::create_tables(pkmn::database_sptr db)
        {
            pksql::create_tables(db);

            std::ostringstream table_stream;
            table_stream << "CREATE TABLE trainer (\n"
                         << "    id               INTEGER NOT NULL,\n"
                         << "    trsql_compat_num INTEGER NOT NULL,\n"
                         << "    game_id          INTEGER NOT NULL,\n"
                         << "    trainer_id       INTEGER NOT NULL,\n"
                         << "    name             VARCHAR(20) NOT NULL,\n"
                         << "    female           BOOLEAN NOT NULL,\n"
                         << "    money            INTEGER NOT NULL\n"
                         << ");\n";

            table_stream << "CREATE TABLE trainer_pokemon (\n"
                         << "    trainer_id       INTEGER NOT NULL,\n"
                         << "    pokemon_id       INTEGER NOT NULL,\n"
                         << "    pokemon_position INETGER NOT NULL\n"
                         << ");\n";

            db->exec(table_stream.str().c_str());
        }

        bool trsql::valid(pkmn::database_sptr db)
        {
            // Check for correct tables
            if(not pksql::valid(db) or not db->tableExists("trainer") or not db->tableExists("trainer_pokemon"))
                return false;

            // Check for valid TRSQL compatibility number
            // Serious enough problem to throw error
            int id = db->execAndGet("SELECT trsql_compat_num FROM trainer");
            if(id != TRSQL_COMPAT_NUM)
                throw std::runtime_error("Invalid TRSQL compatibility number.");

            // Check for at least one entry in "trainer"
            SQLite::Statement trainer_query(*db, "SELECT * FROM trainer");
            if(not trainer_query.executeStep())
                return false;

            // Check for at least one entry in "trainer_pokemon"
            SQLite::Statement trainer_pokemon_query(*db, "SELECT * FROM trainer_pokemon");
            if(not trainer_pokemon_query.executeStep())
                return false;

            // Check for correct number of columns
            if(trainer_query.getColumnCount() != 7)
                return false;

            if(trainer_pokemon_query.getColumnCount() != 3)
                return false;

            return true;
        }

        bool trsql::valid(const pkmn::pkstring &filename)
        {
            pkmn::database_sptr trsql;

            try {trsql = pkmn::database_sptr(new SQLite::Database(filename));}
            catch(...) {return false;}

            if(not trsql::valid(trsql)) return false;

            return true;
        }

        trainer::sptr trsql::from(pkmn::database_sptr db, uint64_t id)
        {
            SQLite::Statement trainer_query(*db, str(boost::format("SELECT * FROM trainer WHERE id=%d")
                                                     % id).c_str());
            trainer_query.executeStep();
            int game_id = trainer_query.getColumn("game_id");

            trainer::sptr tr = trainer::make(
                                   game_id,
                                   trainer_query.getColumn("name"),
                                   ((int(trainer_query.getColumn("female")) == 1) ? Genders::FEMALE : Genders::MALE)
                               );

            tr->set_id(trainer_query.getColumn("trainer_id"));
            tr->set_money(trainer_query.getColumn("money"));

            // Get valid Pokemon
            SQLite::Statement trainer_pokemon_query(*db, str(boost::format("SELECT * FROM trainer_pokemon WHERE trainer_id=%d")
                                                             % id).c_str());

            while(trainer_pokemon_query.executeStep())
            {
                int trainer_id = trainer_pokemon_query.getColumn("trainer_id");
                int pokemon_id = trainer_pokemon_query.getColumn("pokemon_id");
                int pos        = trainer_pokemon_query.getColumn("pokemon_position");

                tr->set_pokemon(pos, pksql::from(db, pokemon_id));
            }

            return tr;
        }

        trainer::sptr trsql::from(const pkmn::pkstring &filename)
        {
            // Validate input file
            if(not trsql::valid(filename))
                throw std::runtime_error("Invalid TRSQL file.");

            pkmn::database_sptr db(new SQLite::Database(filename));
            return trsql::from(db, uint64_t(db->execAndGet("SELECT id FROM trainer")));
        }

        uint64_t trsql::to(trainer::sptr tr, pkmn::database_sptr db)
        {
            static int native_sizes[7] = {0,44,48,100,380,380,500};

            if(not db->tableExists("trainer") and not db->tableExists("trainer_pokemon"))
                trsql::create_tables(db);

            std::ostringstream query_stream;
            query_stream << "INSERT INTO \"trainer\" VALUES("
                         << trsql::latest_id << ","
                         << TRSQL_COMPAT_NUM << ","
                         << tr->get_game_id() << ","
                         << tr->get_id() << ","
                         << "'" << tr->get_name() << "',"
                         << ((tr->get_gender() == "Female") ? 1 : 0) << ","
                         << tr->get_money() << ");\n";

            db->exec(query_stream.str().c_str());

            // Add any invalid Pokemon
            pokemon_team_t party = tr->get_party();
            for(size_t i = 0; i < 6; i++)
            {
                int pokemon_id = pksql::to(party[i], db);

                query_stream.str("");
                query_stream << "INSERT INTO \"trainer_pokemon\" VALUES("
                             << trsql::latest_id << ","
                             << pokemon_id << ","
                             << (i+1)
                             << ");";

                db->exec(query_stream.str().c_str());
            }

            if(not trsql::valid(db))
                throw std::runtime_error("Failed to create valid TRSQL database.");

            return trsql::latest_id++;
        }

        pkmn::database_sptr trsql::to(trainer::sptr tr, const pkmn::pkstring &filename)
        {
            pkmn::database_sptr db(new SQLite::Database(filename, (SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE)));
            db->exec("BEGIN TRANSACTION");
            trsql::to(tr, db);
            db->exec("END TRANSACTION");

            if(not trsql::valid(filename))
                throw std::runtime_error("Failed to create valid TRSQL file.");

            return db;
        }
    }
}
