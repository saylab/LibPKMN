/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include <pkmn/database.hpp>
#include <pkmn/conversions/pokemon.hpp>

#include "pksql.hpp"

namespace pkmn
{
    namespace io
    {
        int pksql::native_sizes[7] = {0,44,48,100,380,380,500};

        uint64_t pksql::latest_id = 1;

        void pksql::create_tables(pkmn::database_sptr db)
        {
            std::ostringstream table_stream;
            table_stream << "CREATE TABLE pokemon (\n"
                         << "    id                    INTEGER NOT NULL,\n"
                         << "    pksql_compat_num      INTEGER NOT NULL,\n"
                         << "    species_id            INTEGER NOT NULL,\n"
                         << "    game_id               INTEGER NOT NULL,\n"
                         << "    nickname              VARCHAR(20) NOT NULL,\n"
                         << "    otname                VARCHAR(20) NOT NULL,\n"
                         << "    otfemale              BOOLEAN NOT NULL,\n"
                         << "    raw                   BLOB NOT NULL\n"
                         << ");\n";

            table_stream << "CREATE TABLE pokemon_attributes (\n"
                         << "    pokemon_id INTEGER NOT NULL,\n"
                         << "    name       VARCHAR(20) NOT NULL,\n"
                         << "    value      INTEGER NOT NULL\n"
                         << ");\n";

            db->exec(table_stream.str().c_str());
        }

        bool pksql::valid(pkmn::database_sptr db)
        {
            // Check for correct tables
            if(not db->tableExists("pokemon") or not db->tableExists("pokemon_attributes"))
                return false;

            // Check for valid PKSQL compatibility number
            // Serious enough problem to throw error
            int id = db->execAndGet("SELECT pksql_compat_num FROM pokemon");
            if(id != PKSQL_COMPAT_NUM)
                throw std::runtime_error("Invalid PKSQL compatibility number.");

            // Check for at least one entry in "pokemon"
            SQLite::Statement pokemon_query(*db, "SELECT * FROM pokemon");
            if(not pokemon_query.executeStep())
                return false;

            // Check for correct number of columns
            if(pokemon_query.getColumnCount() != 8)
                return false;

            return true;
        }

        bool pksql::valid(const pkmn::pkstring &filename)
        {
            pkmn::database_sptr pksql;

            try {pksql = pkmn::database_sptr(new SQLite::Database(filename));}
            catch(...) {return false;}

            if(not pksql::valid(pksql)) return false;

            return true;
        }

        pokemon::sptr pksql::from(pkmn::database_sptr db, uint64_t id)
        {
            SQLite::Statement import_query(*db, str(boost::format("SELECT * FROM pokemon WHERE id=%d")
                                                    % id).c_str());
            import_query.executeStep();

            int version_id = import_query.getColumn("game_id");
            pkmn::pkstring version_name = database::get_version_name(version_id);
            int generation = database::get_generation(version_id);
            pkmn::pkstring nickname = import_query.getColumn("nickname");
            pkmn::pkstring trainer_name = import_query.getColumn("otname");
            pkmn::pkstring trainer_gender = bool(import_query.getColumn("otfemale")) ? "Female" : "Male";
            const void* raw = import_query.getColumn("raw");

            pokemon::sptr pkmn;
            switch(generation)
            {
                case 1:
                {
                    const native::gen1_party_pokemon_t* native1 = reinterpret_cast<const native::gen1_party_pokemon_t*>(raw);
                    uint8_t dummy1[10];

                    pkmn = conversions::import_gen1_pokemon(*native1, dummy1, dummy1,
                                                            version_name);
                    pkmn->set_nickname(nickname);
                    pkmn->set_trainer_name(trainer_name);
                    break;
                }

                case 2:
                {
                    const native::gen2_party_pokemon_t* native2 = reinterpret_cast<const native::gen2_party_pokemon_t*>(raw);
                    uint8_t dummy2[20];

                    pkmn = conversions::import_gen2_pokemon(*native2, dummy2, dummy2,
                                                            version_name);
                    pkmn->set_nickname(nickname);
                    pkmn->set_trainer_name(trainer_name);
                    pkmn->set_trainer_gender(trainer_gender);
                    break;
                }

                case 3:
                {
                    const native::gen3_party_pokemon_t* native3 = reinterpret_cast<const native::gen3_party_pokemon_t*>(raw);
                    pkmn = conversions::import_gen3_pokemon(*native3, version_name, false);
                    break;
                }

                default:
                {
                    const native::nds_party_pokemon_t* nativen = reinterpret_cast<const native::nds_party_pokemon_t*>(raw);
                    pkmn = conversions::import_nds_pokemon(*nativen, version_name, false);
                    break;
                }
            }

            SQLite::Statement attribute_query(*db, str(boost::format("SELECT * FROM pokemon_attributes WHERE pokemon_id=%d")
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
            return pksql::from(db, uint64_t(db->execAndGet("SELECT id FROM pokemon")));
        }

        uint64_t pksql::to(pokemon::sptr pkmn, pkmn::database_sptr db)
        {
            if(not db->tableExists("pokemon") and not db->tableExists("pokemon_attributes"))
                pksql::create_tables(db);

            std::ostringstream query_stream;
            query_stream << "INSERT INTO \"pokemon\" VALUES("
                         << pksql::latest_id << ","
                         << PKSQL_COMPAT_NUM << ","
                         << pkmn->get_species_id() << ","
                         << pkmn->get_game_id() << ","
                         << "'" << pkmn->get_nickname() << "',"
                         << "'" << pkmn->get_trainer_name() << "',"
                         << (pkmn->get_trainer_gender() == std::string("Female") ? 1 : 0)
                         << ",?);";

            SQLite::Statement pksql_query(*db, query_stream.str().c_str());
            pksql_query.bind(1, pkmn->get_native(), pksql::native_sizes[pkmn->get_generation()]);
            pksql_query.executeStep();

            if(not pksql::valid(db))
                throw std::runtime_error("Failed to create valid PKSQL database.");

            return pksql::latest_id++;
        }

        pkmn::database_sptr pksql::to(pokemon::sptr pkmn, const pkmn::pkstring &filename)
        {
            pkmn::database_sptr db(new SQLite::Database(filename, (SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE)));
            db->exec("BEGIN TRANSACTION");
            pksql::to(pkmn, db);
            db->exec("END TRANSACTION");

            if(not pksql::valid(filename))
                throw std::runtime_error("Failed to create valid PKSQL file.");

            return db;
        }
    }
}
