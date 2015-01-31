/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/locale/encoding_utf.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include "internal.hpp"
#include "SQLiteCpp/SQLiteC++.h"

namespace pkmn
{
    namespace database
    {
        static pkmn::shared_ptr<SQLite::Database> db;

        uint16_t get_ability_id(const pkmn::pkstring& ability_name)
        {
            CONNECT_TO_DB(db);
            if(ability_name == "None") return Abilities::NONE;

            std::ostringstream query_stream;
            query_stream << "SELECT ability_id FROM ability_names WHERE name='" << ability_name << "'";
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        pkmn::pkstring get_ability_name(uint16_t ability_id)
        {
            CONNECT_TO_DB(db);
            if(ability_id == Abilities::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM ability_names WHERE local_language_id=9 AND ability_id=" << ability_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_pkstring_from_query(query);
        }

        pkmn::pkstring get_ability_description(uint16_t ability_id, uint16_t version_id)
        {
            CONNECT_TO_DB(db);
            if(ability_id == Abilities::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT flavor_text FROM ability_flavor_text WHERE language_id=9 AND ability_id=" << ability_id
                         << " AND version_group_id=" << get_version_group_id(version_id);
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_pkstring_from_query(query);
        }

        pkmn::pkstring get_ability_description(const pkmn::pkstring& ability_name, const pkmn::pkstring& version_name)
        {
            return get_ability_description(get_ability_id(ability_name), get_version_id(version_name));
        }

        uint16_t get_ball_id(const pkmn::pkstring& ball_name)
        {
            CONNECT_TO_DB(db);
            if(ball_name == "None" or ball_name == "Unknown") return Balls::UNKNOWN;

            std::ostringstream query_stream;
            query_stream << "SELECT id FROM balls WHERE name='" << ball_name << "'";
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        pkmn::pkstring get_ball_name(uint16_t ball_id)
        {
            CONNECT_TO_DB(db);
            if(ball_id == Balls::UNKNOWN) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM balls WHERE id=" << ball_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_pkstring_from_query(query);
        }

        uint16_t get_egg_group_id(const pkmn::pkstring& egg_group_name)
        {
            CONNECT_TO_DB(db);
            if(egg_group_name == "None") return Egg_Groups::NONE;

            std::ostringstream query_stream;
            query_stream << "SELECT egg_group_id FROM egg_group_prose WHERE name='" << egg_group_name << "'";
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        pkmn::pkstring get_egg_group_name(uint16_t egg_group_id)
        {
            CONNECT_TO_DB(db);
            if(egg_group_id == Egg_Groups::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM egg_group_prose WHERE local_language_id=9 AND egg_group_id=" << egg_group_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_pkstring_from_query(query);
        }

        uint32_t get_experience(uint16_t species_id, uint16_t level)
        {
            CONNECT_TO_DB(db);
            if(species_id == 0 or level == 0) return 0;

            std::ostringstream query_stream;
            query_stream << "SELECT experience FROM experience WHERE level=" << level
                         << " AND growth_rate_id=(SELECT growth_rate_id FROM pokemon_species"
                         << " WHERE id=" << species_id << ")";
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        uint32_t get_experience(const pkmn::pkstring& species_name, uint16_t level)
        {
            return get_experience(get_species_id(species_name), level);
        }

        uint16_t PKMN_API get_form_id(const pkmn::pkstring& species_name, const pkmn::pkstring& form_name)
        {
            CONNECT_TO_DB(db);

            std::ostringstream query_stream;
            query_stream << "SELECT form_id FROM libpkmn_pokemon_form_names WHERE name='"
                         << form_name << "'";
            SQLite::Statement query(*db, query_stream.str().c_str());
            std::vector<uint16_t> form_ids;
            while(query.executeStep()) form_ids.push_back(query.getColumn(0));
            BOOST_FOREACH(uint16_t id, form_ids)
            {
                query_stream.str("");
                query_stream << "SELECT species_id FROM pokemon WHERE id="
                             << "(SELECT pokemon_id FROM pokemon_forms WHERE id="
                             << id << ")";
                uint16_t species_id = db->execAndGet(query_stream.str().c_str());

                if(database::get_species_id(species_name) == species_id) return id;
            }

            THROW_QUERY_ERROR(query_stream);
        }

        pkmn::pkstring PKMN_API get_form_name(uint16_t form_id)
        {
            CONNECT_TO_DB(db);

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM libpkmn_pokemon_form_names WHERE form_id="
                         << form_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_pkstring_from_query(query);
        }

        uint8_t get_form_game_index(uint16_t form_id)
        {
            CONNECT_TO_DB(db);
            std::ostringstream query_stream;
            query_stream << "SELECT form_index FROM form_game_indices WHERE form_id="
                         << form_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        uint16_t get_form_id(uint16_t species_id, uint8_t form_index)
        {
            CONNECT_TO_DB(db);
            std::ostringstream query_stream;
            query_stream << "SELECT form_id FROM form_game_indices WHERE form_index="
                         << form_index;
            SQLite::Statement query(*db, query_stream.str().c_str());
            bool match = false;
            uint16_t form_id = 0;
            while(query.executeStep())
            {
                form_id = uint16_t(query.getColumn(0));
                if(database::get_species_id(database::get_pokemon_id(form_id)) == species_id)
                {
                    match = true;
                    break;
                }
            }

            if(match) return form_id;
            else throw std::runtime_error("Invalid query.");
        }

        uint16_t get_level(uint16_t species_id, uint32_t experience)
        {
            CONNECT_TO_DB(db);
            if(species_id == 0) return 0;

            std::ostringstream query_stream;
            query_stream << "SELECT experience.level "
                         << "FROM   experience "
                         << "       INNER JOIN pokemon_species "
                         << "               ON experience.growth_rate_id = pokemon_species.growth_rate_id "
                         << "                  AND experience.growth_rate_id = pokemon_species.growth_rate_id "
                         << "WHERE  ( experience.experience <= " << experience << " ) "
                         << "       AND ( pokemon_species.id = " << species_id << " ) "
                         << "ORDER  BY experience.level DESC";
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        uint16_t get_level(const pkmn::pkstring& species_name, uint32_t experience)
        {
            return get_level(get_species_id(species_name), experience);
        }

        uint16_t get_generation(uint16_t version_id)
        {
            CONNECT_TO_DB(db);
            if(version_id == Versions::NONE) return 0;

            std::ostringstream query_stream;
            query_stream << "SELECT generation_id FROM version_groups WHERE id="
                         << get_version_group_id(version_id);
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        uint16_t get_generation(const pkmn::pkstring& version_name)
        {
            return get_generation(get_version_id(version_name));
        }

        uint16_t get_item_id(const pkmn::pkstring& item_name)
        {
            CONNECT_TO_DB(db);
            if(item_name == "None") return Items::NONE;

            std::ostringstream query_stream;
            query_stream << "SELECT item_id FROM item_names WHERE name='" << item_name << "'";
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        uint16_t get_item_id(uint16_t game_index, uint16_t version_id)
        {
            CONNECT_TO_DB(db);
            if(game_index == 0 or version_id == Versions::NONE) return Items::NONE;

            std::ostringstream query_stream;
            query_stream << "SELECT item_id FROM item_game_indices WHERE game_index=" << game_index
                         << " AND generation_id=" << get_generation(version_id);
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        uint16_t get_item_id(uint16_t game_index, const pkmn::pkstring& version_name)
        {
            return get_item_id(game_index, get_version_id(version_name));
        }

        pkmn::pkstring get_item_name(uint16_t item_id)
        {
            CONNECT_TO_DB(db);
            if(item_id == Items::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM item_names WHERE local_language_id=9 AND item_id=" << item_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_pkstring_from_query(query);
        }

        pkmn::pkstring get_item_name(uint16_t game_index, uint16_t version_id)
        {
            return get_item_name(get_item_id(game_index, version_id));
        }

        pkmn::pkstring get_item_name(uint16_t game_index, const pkmn::pkstring& version_name)
        {
            return get_item_name(game_index, get_version_id(version_name));
        }

        uint16_t get_item_category_id(uint16_t item_id)
        {
            CONNECT_TO_DB(db);
            if(item_id == Items::NONE) return 0;

            std::ostringstream query_stream;
            query_stream << "SELECT category_id FROM items WHERE id=" << item_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        uint16_t get_item_category_id(const pkmn::pkstring& item_name)
        {
            return get_item_category_id(get_item_id(item_name));
        }

        pkmn::pkstring get_item_category_name(uint16_t item_id)
        {
            CONNECT_TO_DB(db);
            if(item_id == Items::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM item_category_prose WHERE local_language_id=9 AND item_category_id="
                         << get_item_category_id(item_id);
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_pkstring_from_query(query);
        }

        pkmn::pkstring get_item_category_name(const pkmn::pkstring& item_name)
        {
            return get_item_category_name(get_item_id(item_name));
        }

        pkmn::pkstring get_item_description(uint16_t item_id, uint16_t version_id)
        {
            CONNECT_TO_DB(db);
            if(item_id == Items::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT flavor_text FROM item_flavor_text WHERE language_id=9 AND item_id=" << item_id
                         << " AND version_group_id=" << get_version_group_id(version_id);
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_pkstring_from_query(query);
        }

        pkmn::pkstring get_item_description(const pkmn::pkstring& item_name, const pkmn::pkstring& version_name)
        {
            return get_item_description(get_item_id(item_name), get_version_id(version_name));
        }

        uint16_t get_item_game_index(uint16_t item_id, uint16_t version_id)
        {
            CONNECT_TO_DB(db);
            if(item_id == Items::NONE or version_id == Versions::NONE) return 0;

            std::ostringstream query_stream;
            query_stream << "SELECT game_index FROM item_game_indices WHERE item_id=" << item_id
                         << " AND generation_id=" << get_generation(version_id);
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        uint16_t get_item_game_index(const pkmn::pkstring& item_name, const pkmn::pkstring& version_name)
        {
            return get_item_game_index(get_item_id(item_name), get_version_id(version_name));
        }

        uint16_t get_move_id(const pkmn::pkstring& move_name)
        {
            CONNECT_TO_DB(db);
            if(move_name == "None") return Moves::NONE;

            std::ostringstream query_stream;
            query_stream << "SELECT move_id FROM move_names WHERE name='" << move_name << "'";
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        pkmn::pkstring get_move_name(uint16_t move_id)
        {
            CONNECT_TO_DB(db);
            if(move_id == Moves::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM move_names WHERE local_language_id=9 AND move_id=" << move_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_pkstring_from_query(query);
        }

        uint16_t get_move_damage_class_id(uint16_t move_id)
        {
            CONNECT_TO_DB(db);
            if(move_id == Moves::NONE) return 0;

            std::ostringstream query_stream;
            query_stream << "SELECT damage_class_id FROM moves WHERE id=" << move_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        uint16_t get_move_damage_class_id(const pkmn::pkstring& move_name)
        {
            return get_move_damage_class_id(get_move_id(move_name));
        }

        pkmn::pkstring get_move_damage_class_name(uint16_t move_id)
        {
            CONNECT_TO_DB(db);
            if(move_id == Moves::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM move_damage_class_prose WHERE local_language_id=9 AND move_damage_class_id="
                         << get_move_damage_class_id(move_id);
            SQLite::Statement query(*db, query_stream.str().c_str());

            if(not query.executeStep())
            {
                THROW_QUERY_ERROR(query_stream);
            }
            else
            {
                std::stringstream full_name((const char*)(query.getColumn(0)));
                std::string name;

                full_name >> name; //Should only get first word
                name[0] = ::toupper(name[0]);
                return name;
            }
        }

        pkmn::pkstring get_move_damage_class_name(const pkmn::pkstring& move_name)
        {
            return get_move_damage_class_name(get_move_id(move_name));
        }

        pkmn::pkstring get_move_description(uint16_t move_id, uint16_t version_id)
        {
            CONNECT_TO_DB(db);
            if(move_id == Moves::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT flavor_text FROM move_flavor_text WHERE language_id=9 AND move_id=" << move_id
                         << " AND version_group_id=" << get_version_group_id(version_id);
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_pkstring_from_query(query);
        }

        pkmn::pkstring get_move_description(const pkmn::pkstring& move_name, const pkmn::pkstring& version_name)
        {
            return get_move_description(get_move_id(move_name), get_version_id(version_name));
        }

        uint16_t get_move_pp(uint16_t move_id)
        {
            CONNECT_TO_DB(db);
            if(move_id == Moves::NONE) return 0;

            std::ostringstream query_stream;
            query_stream << "SELECT pp FROM moves WHERE id=" << move_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        uint16_t get_move_pp(const pkmn::pkstring& move_name)
        {
            return get_move_pp(get_move_id(move_name));
        }

        uint16_t get_nature_id(const pkmn::pkstring& nature_name)
        {
            CONNECT_TO_DB(db);
            if(nature_name == "None") return Moves::NONE;

            std::ostringstream query_stream;
            query_stream << "SELECT nature_id FROM nature_names WHERE name='" << nature_name << "'";
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        pkmn::pkstring get_nature_name(uint16_t nature_id)
        {
            CONNECT_TO_DB(db);
            if(nature_id == Natures::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM nature_names WHERE local_language_id=9 AND nature_id=" << nature_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_pkstring_from_query(query);
        }

        double get_nature_stat_mod(uint16_t nature_id, uint16_t stat_id)
        {
            CONNECT_TO_DB(db);
            if(nature_id == Natures::NONE or stat_id == Stats::NONE) return 1.0;

            std::ostringstream query_stream;
            query_stream << "SELECT decreased_stat_id,increased_stat_id FROM natures WHERE id=" << nature_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            if(not query.executeStep()) THROW_QUERY_ERROR(query_stream);

            if(double(query.getColumn(0)) == stat_id) return 0.9;
            else if(double(query.getColumn(1)) == stat_id) return 1.1;
            else return 1.0;
        }

        double get_nature_stat_mod(const pkmn::pkstring& nature_name, const pkmn::pkstring& stat_name)
        {
            return get_nature_stat_mod(get_nature_id(nature_name), get_stat_id(stat_name));
        }

        pkstring get_pokedex_entry(uint16_t species_id, uint16_t version_id)
        {
            CONNECT_TO_DB(db);
            if(species_id == Species::NONE or version_id == Versions::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT flavor_text FROM pokemon_species_flavor_text WHERE language_id=9 AND species_id=" << species_id
                         << " AND version_id=" << version_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_pkstring_from_query(query);
        }

        pkstring get_pokedex_entry(const pkmn::pkstring& species_name, const pkmn::pkstring& version_name)
        {
            return get_pokedex_entry(get_species_id(species_name), get_version_id(version_name));
        }

        uint16_t get_pokemon_id(uint16_t game_index, uint16_t version_id)
        {
            CONNECT_TO_DB(db);
            if(game_index == 0 or version_id == Versions::NONE) return 0;

            std::ostringstream query_stream;
            query_stream << "SELECT pokemon_id FROM pokemon_game_indices WHERE version_id=" << version_id
                         << " AND game_index=" << game_index;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        uint16_t get_pokemon_id(uint16_t game_index, const pkmn::pkstring& version_name)
        {
            return get_pokemon_id(game_index, get_version_id(version_name));
        }

        uint16_t get_pokemon_id(uint16_t form_id)
        {
            CONNECT_TO_DB(db);
            if(form_id == 0) return 0;

            std::ostringstream query_stream;
            query_stream << "SELECT pokemon_id FROM pokemon_forms WHERE id=" << form_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        uint16_t get_pokemon_game_index(uint16_t pokemon_id, uint16_t version_id)
        {
            CONNECT_TO_DB(db);
            if(version_id == Versions::NONE) return 0;

            std::ostringstream query_stream;
            query_stream << "SELECT game_index FROM pokemon_game_indices WHERE pokemon_id=" << pokemon_id
                         << " AND version_id=" << version_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        uint16_t get_pokemon_game_index(uint16_t pokemon_id, const pkmn::pkstring& version_name)
        {
            return get_pokemon_game_index(pokemon_id, get_version_id(version_name));
        }

        uint16_t get_species_id(const pkmn::pkstring& species_name)
        {
            CONNECT_TO_DB(db);
            if(species_name == "None") return Species::NONE;

            std::ostringstream query_stream;
            query_stream << "SELECT pokemon_species_id FROM pokemon_species_names WHERE name='" << species_name << "'";
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        uint16_t get_species_id(uint16_t pokemon_id)
        {
            CONNECT_TO_DB(db);
            if(pokemon_id == 0) return Species::NONE;

            std::ostringstream query_stream;
            query_stream << "SELECT species_id FROM pokemon WHERE id=" << pokemon_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        pkmn::pkstring get_species_name(uint16_t species_id)
        {
            CONNECT_TO_DB(db);
            if(species_id == Versions::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM pokemon_species_names WHERE local_language_id=9 AND pokemon_species_id=" << species_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_pkstring_from_query(query);
        }

        uint16_t get_stat_id(const pkmn::pkstring& stat_name)
        {
            CONNECT_TO_DB(db);
            if(stat_name == "None") return Stats::NONE;

            std::ostringstream query_stream;
            query_stream << "SELECT stat_id FROM stat_names WHERE name='" << stat_name << "'";
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        pkmn::pkstring get_stat_name(uint16_t stat_id)
        {
            CONNECT_TO_DB(db);
            if(stat_id == Stats::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM stat_names WHERE local_language_id=9 AND stat_id=" << stat_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_pkstring_from_query(query);
        }

        uint16_t get_type_id(const pkmn::pkstring& type_name)
        {
            CONNECT_TO_DB(db);
            if(type_name == "None") return Types::NONE;

            std::ostringstream query_stream;
            query_stream << "SELECT type_id FROM type_names WHERE local_language_id=9 AND name='" << type_name << "'";
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        pkmn::pkstring get_type_name(uint16_t type_id)
        {
            CONNECT_TO_DB(db);
            if(type_id == Versions::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM type_names WHERE local_language_id=9 AND type_id=" << type_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_pkstring_from_query(query);
        }

        uint16_t get_version_id(const pkmn::pkstring& version_name)
        {
            CONNECT_TO_DB(db);
            if(version_name == "None") return Versions::NONE;

            std::ostringstream query_stream;
            query_stream << "SELECT version_id FROM version_names WHERE name='" << version_name << "'";
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        pkmn::pkstring get_version_name(uint16_t version_id)
        {
            CONNECT_TO_DB(db);
            if(version_id == Versions::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM version_names WHERE local_language_id=9 AND version_id=" << version_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_pkstring_from_query(query);
        }

        uint16_t get_version_game_index(const pkmn::pkstring& version_name)
        {
            return get_version_game_index(get_version_id(version_name));
        }

        uint16_t get_version_game_index(uint16_t version_id)
        {
            CONNECT_TO_DB(db);
            if(version_id == Versions::NONE) return 0;

            std::ostringstream query_stream;
            query_stream << "SELECT game_index FROM version_game_indices WHERE version_id="
                         << version_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        uint16_t get_version_id(uint16_t version_game_index)
        {
            CONNECT_TO_DB(db);

            std::ostringstream query_stream;
            query_stream << "SELECT version_id FROM version_game_indices WHERE game_index="
                         << version_game_index;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        uint16_t get_version_group_id(uint16_t version_id)
        {
            CONNECT_TO_DB(db);
            if(version_id == Versions::NONE) return Version_Groups::NONE;

            std::ostringstream query_stream;
            query_stream << "SELECT version_group_id FROM versions WHERE id=" << version_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            return get_num_from_query<uint16_t>(query);
        }

        uint16_t get_version_group_id(const pkmn::pkstring& version_name)
        {
            return get_version_group_id(get_version_id(version_name));
        }

        pkmn::pkstring get_version_group_name(uint16_t version_group_id)
        {
            CONNECT_TO_DB(db);
            if(version_group_id == Version_Groups::NONE) return "None";

            std::ostringstream query_stream;
            query_stream << "SELECT name FROM version_names WHERE local_language_id=9 AND version_id IN "
                         << "(SELECT id FROM versions WHERE version_group_id=" << version_group_id << ")";
            SQLite::Statement query(*db, query_stream.str().c_str());
            std::string output;

            if(query.executeStep()) output = (const char*)(query.executeStep());
            else THROW_QUERY_ERROR(query_stream);

            while(query.executeStep())
            {
                output += "/";
                output += (const char*)(query.executeStep());
            }

            return output;
        }
    }
}
