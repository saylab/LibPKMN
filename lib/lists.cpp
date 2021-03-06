/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <sstream>
#include <stdexcept>

#include <boost/algorithm/string.hpp>
#include <boost/locale/encoding_utf.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/lists.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include "internal.hpp"
#include "SQLiteCpp/SQLiteC++.h"

namespace pkmn
{
    pkmn::shared_ptr<SQLite::Database> db;

    void get_version_list(std::vector<pkmn::pkstring> &game_vec)
    {
        CONNECT_TO_DB(db);
        game_vec.clear();

        SQLite::Statement query(*db, "SELECT name FROM version_names WHERE local_language_id=9");

        while(query.executeStep()) game_vec.push_back((const char*)query.getColumn(0));
    }

    void get_version_group_list(std::vector<pkmn::pkstring> &game_group_vec)
    {
        // Must be done manually
        game_group_vec.clear();

        game_group_vec.push_back("Red/Blue/Green");
        game_group_vec.push_back("Yellow");
        game_group_vec.push_back("Gold/Silver");
        game_group_vec.push_back("Crystal");
        game_group_vec.push_back("Ruby/Sapphire/Emerald");
        game_group_vec.push_back("Fire Red/Leaf Green");
        game_group_vec.push_back("Diamond/Pearl");
        game_group_vec.push_back("Platinum");
        game_group_vec.push_back("Heart Gold/Soul Silver");
        game_group_vec.push_back("Black/White");
        game_group_vec.push_back("Black 2/White 2");
    }

    void get_item_list(std::vector<pkmn::pkstring> &item_vec, uint16_t game)
    {
        CONNECT_TO_DB(db);
        item_vec.clear();

        uint16_t gen = database::get_generation(game);
        std::ostringstream query_stream;
        query_stream << "SELECT name FROM item_names WHERE local_language_id=9 AND item_id IN "
                     << "(SELECT item_id FROM item_game_indices WHERE generation_id=" << gen;

        /*
         * The database only shows which generation items come from, but it doesn't take
         * into account differences between games within a generation.
         */
        switch(gen)
        {
            case 1:
            {
                query_stream << ")";
                break;
            }

            case 2:
            {
                std::string end = (game == Versions::CRYSTAL) ? ")"
                                                           : " AND game_index NOT IN (70,115,116,129))";
                query_stream << end;
                break;
            }

            case 3:
            {
                std::string end;
                switch(game)
                {
                    case Versions::EMERALD:
                        end = ")";
                        break;

                    case Versions::FIRERED:
                    case Versions::LEAFGREEN:
                        end = " AND game_index<=374)";
                        break;

                    default:
                        end = " AND game_index<=348)";
                        break;
                }
                query_stream << end;
                break;
            }

            case 4:
            {
                std::string end;
                switch(game)
                {
                    case Versions::HEARTGOLD:
                    case Versions::SOULSILVER:
                        end = ")";
                        break;

                    case Versions::PLATINUM:
                        end = " AND game_index<=467)";
                        break;

                    default:
                        end = " AND game_index<=464 AND game_index!=112)";
                        break;
                }
                query_stream << end;
                break;
            }

            case 5:
            {
                std::string end = (game < Versions::BLACK_2) ? " AND game_index<=626)"
                                                         : ")";
                query_stream << end;
                break;
            }

            default:
                query_stream << ")";
                break;
        }
        SQLite::Statement query(*db, query_stream.str().c_str());
        while(query.executeStep())
        {
            const uint16_t* entry = query.getColumn(0);
            item_vec.push_back(boost::locale::conv::utf_to_utf<wchar_t>(entry));
        }
    }

    void get_pokemon_list(std::vector<pkmn::pkstring> &pokemon_vec, uint16_t game)
    {
        CONNECT_TO_DB(db);
        pokemon_vec.clear();

        uint16_t bounds[] = {0,151,251,386,493,649,719};
        uint16_t bound = bounds[database::get_generation(game)];
        std::ostringstream query_stream;
        query_stream << "SELECT name FROM pokemon_species_names WHERE local_language_id=9 AND pokemon_species_id<=" << bound << std::endl;
        SQLite::Statement query(*db, query_stream.str().c_str());

        while(query.executeStep())
        {
            const uint16_t* entry = query.getColumn(0);
            pokemon_vec.push_back(boost::locale::conv::utf_to_utf<wchar_t>(entry));
        }
    }

    void get_type_list(std::vector<pkmn::pkstring> &type_vec, uint16_t gen)
    {
        CONNECT_TO_DB(db);
        type_vec.clear();

        SQLite::Statement type_names_query(*db, "SELECT name FROM type_names WHERE local_language_id=9");
        while(type_names_query.executeStep())
        {
            std::string type = std::string((const char*)type_names_query.getColumn(0));
            if(not (gen == 1 and (type == "Steel" or type == "Dark")) and type != "???" and type != "Shadow")
            {
                type_vec.push_back(type);
            }
        }
    }

    void get_ability_list(std::vector<pkmn::pkstring> &ability_vec, uint16_t gen)
    {
        CONNECT_TO_DB(db);
        ability_vec.clear();

        std::ostringstream query_stream;
        query_stream << "SELECT name FROM ability_names WHERE local_language_id=9 AND ability_id IN "
                     << "(SELECT id FROM abilities WHERE generation_id <= " << gen << ")";
        SQLite::Statement query(*db, query_stream.str().c_str());

        while(query.executeStep()) ability_vec.push_back((const char*)(query.getColumn(0)));
    }

    void get_nature_list(std::vector<pkmn::pkstring> &nature_vec)
    {
        CONNECT_TO_DB(db);
        nature_vec.clear();

        SQLite::Statement query(*db, "SELECT name FROM nature_names WHERE local_language_id=9");
        while(query.executeStep()) nature_vec.push_back((const char*)query.getColumn(0));
    }

    void get_pokemon_of_type(pokemon_entry_vector_t &pkmn_vector,
                             const pkmn::pkstring &type1,
                             const pkmn::pkstring &type2,
                             uint16_t generation,
                             bool lax)
    {
        CONNECT_TO_DB(db);
        pkmn_vector.clear();

        std::stringstream query_stream;
        std::vector<int> applicable_ids;
        int pkmn_id = 0;
        int type1_id = 0;
        int type2_id = 0;

        // Get type IDs
        query_stream << "SELECT type_id FROM type_names WHERE name='" << type1 << "'";
        type1_id = int(db->execAndGet(query_stream.str().c_str()));
        if(type2.std_string() != "None" and type2.std_string() != "Any")
        {
            query_stream.str("");
            query_stream << "SELECT type_id FROM type_names WHERE name='" << type2 << "'";
            type2_id = int(db->execAndGet(query_stream.str().c_str()));
        }

        if((type2 == "None" or type2 == "Any") and lax)
        {
            // Get IDs of Pokémon
            query_stream.str("");
            query_stream << "SELECT pokemon_id FROM pokemon_types WHERE type_id=" << type1_id;
            SQLite::Statement pokemon_types_query(*db, query_stream.str().c_str());

            // Get any Pokémon of specified type (by itself or paired with any other)
            while(pokemon_types_query.executeStep())
            {
                pkmn_id = pokemon_types_query.getColumn(0); // pokemon_id

                query_stream.str("");
                query_stream << "SELECT species_id FROM pokemon WHERE id=" << pkmn_id;
                int species_id = db->execAndGet(query_stream.str().c_str());

                // Get generation ID to restrict list
                query_stream.str("");
                query_stream << "SELECT generation_id FROM pokemon_species WHERE id=" << species_id;
                int generation_id = db->execAndGet(query_stream.str().c_str());
                if(generation_id <= generation)
                {
                    applicable_ids.push_back(pkmn_id);
                }
            }
        }
        else
        {
            // Get IDs of Pokémon matching first type
            std::vector<int> pkmn_ids;
            query_stream.str("");
            query_stream << "SELECT pokemon_id FROM pokemon_types WHERE type_id=" << type1_id;
            SQLite::Statement pokemon_types_id_query(*db, query_stream.str().c_str());

            while(pokemon_types_id_query.executeStep()) pkmn_ids.push_back(pokemon_types_id_query.getColumn(0));

            std::vector<int> to_erase;
            if(type2 == "None")
            {
                // If only one type is specified, find number of entries with that ID and remove duplicates
                for(size_t i = 0; i < pkmn_ids.size(); i++)
                {
                    int pkmn_count = 0; // Number of types Pokémon appears in pokemon_moves
                    query_stream.str("");
                    query_stream << "SELECT type_id FROM pokemon_types WHERE pokemon_id=" << pkmn_ids[i];
                    SQLite::Statement inner_query(*db, query_stream.str().c_str());
                    while(inner_query.executeStep()) pkmn_count++;

                    if(pkmn_count > 1) to_erase.push_back(i);
                }
            }
            else
            {
                // See if entry exists for other type, add to to_erase if not
                for(size_t i = 0; i < pkmn_ids.size(); i++)
                {
                    query_stream.str("");
                    query_stream << "SELECT type_id FROM pokemon_types WHERE pokemon_id=" << pkmn_ids[i]
                                 << " AND type_id=" << type2_id;
                    SQLite::Statement inner_query(*db, query_stream.str().c_str());
                    if(not inner_query.executeStep()) to_erase.push_back(i);
                }
            }

            // Erase invalid entries
            for(size_t i = to_erase.size()-1; i > 0; i--) pkmn_ids.erase(pkmn_ids.begin() + to_erase[i]);
            pkmn_ids.erase(pkmn_ids.begin() + to_erase[0]);

            // Get identifiers for remaining entries
            for(size_t i = 0; i < pkmn_ids.size(); i++)
            {
                query_stream.str("");
                query_stream << "SELECT generation_id FROM pokemon_species WHERE id="
                             << "(SELECT species_id FROM pokemon WHERE id=" << pkmn_ids[i] << ")";

                int generation_id = db->execAndGet(query_stream.str().c_str());
                if(generation_id <= generation)
                    applicable_ids.push_back(pkmn_ids[i]); // ID's that apply to final Pokemon
            }
        }

        int game_id_from_gen[] = {Versions::YELLOW, Versions::CRYSTAL,
                                  Versions::FIRERED, Versions::HEARTGOLD,
                                  Versions::BLACK_2, Versions::X};

        for(size_t i = 0; i < applicable_ids.size(); i++)
        {
            // Manually correct for Magnemite and Magneton in Gen 1
            int final_species_id = database::get_species_id(applicable_ids[i]);
            if(not ((database::get_species_name(final_species_id) == "Magnemite" or
                     database::get_species_name(final_species_id) == "Magneton") and generation == 1))
            {
                query_stream.str("");
                query_stream << "SELECT id FROM pokemon_forms WHERE pokemon_id=" << applicable_ids[i];
                pkmn::pokemon_entry_t entry(game_id_from_gen[generation],
                                            final_species_id,
                                            uint16_t(db->execAndGet(query_stream.str().c_str())));

                pkmn_vector.push_back(entry);
            }
        }
    }
} /* namespace pkmn */
