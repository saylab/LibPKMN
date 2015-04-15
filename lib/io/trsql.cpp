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

        std::string trsql::create_tables()
        {
            std::ostringstream table_stream;
            table_stream << pksql::create_tables() << std::endl;
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
                         << "    id               INTEGER NOT NULL,\n"
                         << "    trainer_id       INTEGER NOT NULL,\n"
                         << "    pokemon_id       INTEGER NOT NULL\n"
                         << "    pokemon_position INTEGER NOT NULL\n"
                         << ");\n";

            table_stream << "CREATE TABLE trainer_invalid_pokemon (\n"
                         << "    id               INTEGER NOT NULL,\n"
                         << "    trainer_id       INTEGER NOT NULL,\n"
                         << "    raw_pokemon      BLOB NOT NULL,\n"
                         << "    pokemon_position INETGER NOT NULL,\n"
                         << "    nickname         VARCHAR(20) NOT NULL,\n"
                         << "    otname           VARCHAR(20) NOT NULL\n"
                         << ");\n";

            return table_stream.str();
        }

        std::string trsql::query(trainer::sptr tr, uint64_t id)
        {
            std::vector<std::pair<int, int> > pokemon_ids;
            pkmn::pokemon_team_t party = tr->get_party();

            std::ostringstream query_stream;

            query_stream << "INSERT INTO \"trainer\" VALUES("
                         << id << ","
                         << TRSQL_COMPAT_NUM << ","
                         << tr->get_game_id() << ","
                         << tr->get_id() << ","
                         << tr->get_name() << ","
                         << ((tr->get_gender() == "Female") ? 1 : 0) << ","
                         << tr->get_money() << ");\n";

            // Trainer's valid Pokemon
            for(size_t i = 0; i < party.size(); i++)
            {
                if(party[i]->get_species_id() == Species::NONE or
                   party[i]->get_species_id() == Species::INVALID)
                {
                    continue;
                }
                else
                {
                    query_stream << pksql::query(party[i], trsql::latest_id);
                    pokemon_ids.push_back(std::make_pair(i+1, trsql::latest_id++));
                }
            }
            if(id == trsql::latest_id) trsql::latest_id++;

            return query_stream.str();
        }

        bool trsql::valid(pkmn::database_sptr db)
        {
            // Check for correct tables
            if(not pksql::valid(db) or not db->tableExists("trainer") or not db->tableExists("trainer_pokemon")
               or not db->tableExists("trainer_invalid_pokemon"))
            {
                return false;
            }

            // Check for at least one entry in "trainer"
            SQLite::Statement trainer_query(*db, "SELECT * FROM trainer");
            if(not trainer_query.executeStep())
                return false;

            // Check for at least one entry in "pkmn"
            SQLite::Statement pkmn_query(*db, "SELECT * FROM pkmn");
            if(not pkmn_query.executeStep())
                return false;

            // Check for at least one entry in "trainer_"
            SQLite::Statement trainer_pokemon_query(*db, "SELECT * FROM trainer_pokemon");
            if(not trainer_pokemon_query.executeStep())
                return false;

            // Check for valid TRSQL compatibility number
            // Serious enough problem to throw error
            int id = db->execAndGet("SELECT trsql_compat_num FROM pkmn");
            if(id != TRSQL_COMPAT_NUM)
                throw std::runtime_error("Invalid TRSQL compatibility number.");

            // Check for correct number of columns
            if(trainer_query.getColumnCount() != 7)
                return false;
            if(trainer_pokemon_query.getColumnCount() != 4)
                return false;

            return true;
        }

        bool trsql::valid(const pkmn::pkstring &filename)
        {
            pkmn::database_sptr trsql;

            try {trsql = pkmn::database_sptr(new SQLite::Database(filename));}
            catch(...) {return false;}

            return trsql::valid(trsql);

            return true;
        }

        trainer::sptr trsql::from(pkmn::database_sptr db, uint64_t id)
        {
            SQLite::Statement trainer_query(*db, str(boost::format("SELECT * FROM trainer WHERE id=%d")
                                                     % id).c_str());
            trainer_query.executeStep();

            trainer::sptr tr = trainer::make(
                                   trainer_query.getColumn(2), // game_id,
                                   trainer_query.getColumn(4), // name,
                                   ((int(trainer_query.getColumn(5)) == 1) ? Genders::FEMALE : Genders::MALE) // gender
                               );

            tr->set_id(trainer_query.getColumn(3));
            tr->set_money(trainer_query.getColumn(6));

            // Get valid Pokemon
            SQLite::Statement trainer_pokemon_query(*db, str(boost::format("SELECT * FROM trainer_pokemon WHERE trainer_id=%d")
                                                             % tr->get_id()).c_str());
            while(trainer_pokemon_query.executeStep())
            {
                SQLite::Statement pkmn_query(*db, str(boost::format("SELECT id FROM pkmn WHERE id=%d")
                                                      % trainer_pokemon_query.getColumn(2)).c_str());

                tr->set_pokemon(trainer_pokemon_query.getColumn(3),
                                pksql::from(db,
                                            db->execAndGet(str(boost::format("SELECT id FROM pkmn WHERE id=%d")
                                                               % trainer_pokemon_query.getColumn(2)).c_str())
                                )
                               );
            }

            // Get invalid Pokemon
            SQLite::Statement trainer_invalid_pokemon_query(*db,
                                                            str(boost::format("SELECT * FROM trainer_invalid_pokemon WHERE trainer_id=")
                                                                % tr->get_id()).c_str()
                                                           );
            while(trainer_invalid_pokemon_query.executeStep())
            {
                const void* raw = trainer_invalid_pokemon_query.getColumn(2);
                pokemon::sptr pkmn;

                int game_id = db->exec(str(boost::format("SELECT game_id FROM trainer WHERE trainer_id=")
                                           % trainer_invalid_pokemon_query.getColumn(1)).c_str());

                switch(database::get_generation(game_id))
                {
                    case 1:
                    {
                        native::gen1_party_pokemon_t native1;
                        memcpy(&native1, raw, sizeof(native::gen1_party_pokemon_t));

                        uint8_t* nickname_buffer = new uint8_t[15];
                        conversions::export_gen1_text(trainer_invalid_pokemon_query.getColumn(4),
                                                      nickname_buffer,
                                                      10);

                        uint8_t* otname_buffer = new uint8_t[10];
                        conversions::export_gen1_text(trainer_invalid_pokemon_query.getColumn(5),
                                                      otname_buffer,
                                                      7);

                        pkmn = conversions::import_gen1_pokemon(native1,
                                                                nickname_buffer,
                                                                otname_buffer,
                                                                database::get_version_name(game_id)
                                                               );
                        delete[] nickname_buffer;
                        delete[] otname_buffer;

                        break;
                    }

                    case 2:
                    {
                        native::gen2_party_pokemon_t native2;
                        memcpy(&native2, raw, sizeof(native::gen2_party_pokemon_t));

                        uint8_t* nickname_buffer = new uint8_t[15];
                        conversions::export_gen2_text(trainer_invalid_pokemon_query.getColumn(4),
                                                      nickname_buffer,
                                                      10);

                        uint8_t* otname_buffer = new uint8_t[10];
                        conversions::export_gen2_text(trainer_invalid_pokemon_query.getColumn(5),
                                                      otname_buffer,
                                                      7);

                        pkmn = conversions::import_gen2_pokemon(native2,
                                                                nickname_buffer,
                                                                otname_buffer,
                                                                database::get_version_name(game_id)
                                                               );
                        delete[] nickname_buffer;
                        delete[] otname_buffer;

                        break;
                    }

                    case 3:
                    {
                        native::gen3_party_pokemon_t native3;
                        memcpy(&native3, raw, sizeof(native::gen3_party_pokemon_t));

                        pkmn = conversions::import_gen3_pokemon(native3,
                                                                database::get_version_name(game_id),
                                                                false
                                                               );
                        break;
                    }

                    default:
                    {
                        native::nds_party_pokemon_t nativen;
                        memcpy(&nativen, raw, sizeof(native::nds_party_pokemon_t));

                        pkmn = conversions::import_nds_pokemon(nativen,
                                                               database::get_version_name(game_id),
                                                               false
                                                              );
                        break;
                    }
                }

                tr->set_pokemon(trainer_invalid_pokemon_query.getColumn(3), pkmn);
            }

            if(id > trsql::latest_id)
                trsql::latest_id = id;

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

        void trsql::to(trainer::sptr tr, pkmn::database_sptr db)
        {
            static int native_sizes[7] = {0,44,48,100,380,380,500};

            if(not db->tableExists("trainer"))
                db->exec(trsql::create_tables().c_str());

            db->exec(trsql::query(tr).c_str());

            // Add any invalid Pokemon
            pokemon_team_t party = tr->get_party();
            for(size_t i = 0; i < party.size(); i++)
            {
                if(party[i]->get_species_id() == Species::INVALID)
                {
                    std::ostringstream query_stream;
                    query_stream << "INSERT INTO trainer_invalid_pokemon VALUES("
                                 << trsql::latest_id++ << ","
                                 << db->execAndGet("SELECT id FROM trainer") << ", ?,"
                                 << (i+1) << ","
                                 << party[i]->get_nickname() << ","
                                 << party[i]->get_trainer_name() << ");";

                    SQLite::Statement export_query(*db, query_stream.str().c_str());
                    export_query.bind(1, party[i]->get_native(), native_sizes[party[i]->get_generation()]);
                }
            }

            if(not trsql::valid(db))
                throw std::runtime_error("Failed to create valid TRSQL database.");
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
