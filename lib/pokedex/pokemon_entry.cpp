/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <sstream>

#include <boost/assign.hpp>

#include <pkmn/database.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/pokedex/pokemon_entry.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include "internal.hpp"
#include "SQLiteCpp/SQLiteC++.h"

namespace pkmn
{
    static pkmn::shared_ptr<SQLite::Database> db;
    static pkmn::pokemon_entry_t none_entry;
    static pkmn::pokemon_entry_t invalid_entry;
    static bool entries_created = false;

    static void create_none_invalid_entries()
    {
        // None entry
        none_entry.species_name = "None";
        none_entry.pokedex_num = 0;
        none_entry.form = "None";
        none_entry.pokedex_entry = "None";
        none_entry.types = std::make_pair("None", "None");
        none_entry.abilities = std::make_pair("None", "None");
        none_entry.hidden_ability = "None";
        none_entry.egg_groups = std::make_pair("None", "None");
        none_entry.catch_rate = 0;
        none_entry.exp_yield = 0;
        none_entry.base_friendship = 0;
        none_entry.height = 0.0;
        none_entry.weight = 0.0;
        none_entry.chance_male = 0.0;
        none_entry.chance_female = 0.0;
        none_entry.has_gender_differences = false;
        none_entry.base_stats = boost::assign::map_list_of
            ("HP", 0)
            ("Attack", 0)
            ("Defense", 0)
            ("Speed", 0)
            ("Special", 0)
            ("Special Attack", 0)
            ("Special Defense", 0)
        ;
        none_entry.ev_yields = boost::assign::map_list_of
            ("HP", 0)
            ("Attack", 0)
            ("Defense", 0)
            ("Speed", 0)
            ("Special", 0)
            ("Special Attack", 0)
            ("Special Defense", 0)
        ;

        // Invalid entry
        invalid_entry = none_entry;
        invalid_entry.species_name = "Invalid";

        entries_created = true;
    }

    pokemon_entry_t::pokemon_entry_t(uint16_t version_id,
                                     uint16_t species_id,
                                     uint16_t form_id)
    {
        CONNECT_TO_DB(db);
        if(not entries_created)
            create_none_invalid_entries();

        if(species_id == Species::NONE)
        {
            *this = none_entry;
            return;
        }
        else if(species_id == Species::INVALID)
        {
            *this = invalid_entry;
            return;
        }

        uint16_t pokemon_id;
        std::ostringstream query_stream;

        // Use default form if nothing given
        if(form_id == 0 or form_id == species_id) pokemon_id = species_id;
        else
        {
            query_stream << "SELECT pokemon_id,introduced_in_version_group_id FROM pokemon_forms WHERE id="
                         << form_id;
            SQLite::Statement query(*db, query_stream.str().c_str());
            if(query.executeStep())
            {
                if(database::get_version_group_id(version_id) < int(query.getColumn(1)))
                    throw std::runtime_error("This form did not exist in this version.");

                pokemon_id = query.getColumn(0);
            }
            else throw std::runtime_error("Invalid form.");
        }

        form = (form_id == species_id) ? "Standard"
                                       : database::get_form_name(form_id);

        /*
         * Everything from "pokemon" table
         */
        query_stream.str("");
        query_stream << "SELECT * FROM pokemon WHERE id=" << pokemon_id;
        SQLite::Statement pokemon_query(*db, query_stream.str().c_str());
        pokemon_query.executeStep();
        pokedex_num = pokemon_query.getColumn(2);           // species_id
        species_name = database::get_species_name(pokedex_num);
        height = float(pokemon_query.getColumn(3)) / float(10.0); // height
        weight = float(pokemon_query.getColumn(4)) / float(10.0); // weight
        exp_yield = pokemon_query.getColumn(5);             // base_experience

        /*
         * Everything from "pokemon_species" table
         */
        query_stream.str("");
        query_stream << "SELECT * FROM pokemon_species WHERE id="
                     << database::get_species_id(pokemon_id);
        SQLite::Statement pokemon_species_query(*db, query_stream.str().c_str());
        pokemon_species_query.executeStep();

        /*
         * Gender
         *
         * gender_val_dict's keys correspond to how the different
         * gender rates are represented in the database. The values
         * are the actual decimal representations of the percentages.
         */
        pkmn::dict<int8_t, float> gender_val_dict; // Double is percentage male
        gender_val_dict[-1] = 0.0;
        gender_val_dict[0]  = 1.0;
        gender_val_dict[1]  = 0.875;
        gender_val_dict[2]  = 0.75;
        gender_val_dict[4]  = 0.5;
        gender_val_dict[6]  = 0.25;
        gender_val_dict[8]  = 0.0;

        int8_t gender_val = pokemon_species_query.getColumn(8); // gender_rate

        if(gender_val == -1)
        {
            chance_male = 0.0;
            chance_female = 0.0;
        }
        else
        {
            chance_male = gender_val_dict[gender_val];
            chance_female = 1.0f - chance_male;
        }

        catch_rate = pokemon_species_query.getColumn(9);              // capture_rate
        base_friendship = pokemon_species_query.getColumn(10);        // base_happiness
        has_gender_differences = pokemon_species_query.getColumn(13); // has_gender_differences

        int generation = database::get_generation(version_id);
        bool old_games     = (generation < 3);

        /*
         * Abilities
         */
        query_stream.str("");
        query_stream << "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id="
                     << pokemon_id << " AND is_hidden=0";
        SQLite::Statement pokemon_abilities_query(*db, query_stream.str().c_str());
        pokemon_abilities_query.executeStep();
        abilities.first = database::get_ability_name(pokemon_abilities_query.getColumn(0)); // ability_id
        if(pokemon_abilities_query.executeStep())
            abilities.second = database::get_ability_name(pokemon_abilities_query.getColumn(0)); // ability_id
        else
            abilities.second = "None";
        if(generation < 5)
            hidden_ability = "None";
        else
        {
            query_stream.str("");
            query_stream << "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id="
                         << pokemon_id << " AND is_hidden=1";
            SQLite::Statement pokemon_abilities_query2(*db, query_stream.str().c_str());
            if(pokemon_abilities_query2.executeStep())
                hidden_ability = database::get_ability_name(pokemon_abilities_query2.getColumn(0));
            else
                hidden_ability = "None";
        }

        /*
         * Egg groups
         */
        query_stream.str("");
        query_stream << "SELECT egg_group_id FROM pokemon_egg_groups WHERE species_id="
                     << pokedex_num;
        SQLite::Statement pokemon_egg_groups_query(*db, query_stream.str().c_str());
        pokemon_egg_groups_query.executeStep();
        egg_groups.first = database::get_egg_group_name(pokemon_egg_groups_query.getColumn(0));
        if(pokemon_egg_groups_query.executeStep())
            egg_groups.second = database::get_egg_group_name(pokemon_egg_groups_query.getColumn(0));
        else
            egg_groups.second = "None";

        /*
         * Pokédex entry
         */
        query_stream.str("");
        query_stream << "SELECT flavor_text FROM pokemon_species_flavor_text WHERE species_id="
                     << database::get_species_id(pokemon_id) << " AND version_id="
                     << version_id << " AND language_id=9";
        SQLite::Statement pokedex_entry_query(*db, query_stream.str().c_str());
        pokedex_entry = get_pkstring_from_query(pokedex_entry_query);

        /*
         * Base stats, effort yields
         */
        query_stream.str("");
        query_stream << "SELECT base_stat,effort FROM pokemon_stats WHERE pokemon_id="
                     << pokemon_id << " AND stat_id IN (1,2,3,6)";
        SQLite::Statement pokemon_stats_query(*db, query_stream.str().c_str());
        pokemon_stats_query.executeStep();
        base_stats["HP"] = pokemon_stats_query.getColumn(0);                      // base_stat
        ev_yields["HP"]  = pokemon_stats_query.getColumn(old_games ? 0 : 1);      // base_stat, effort
        pokemon_stats_query.executeStep();
        base_stats["Attack"] = pokemon_stats_query.getColumn(0);                  // base_stat
        ev_yields["Attack"]  = pokemon_stats_query.getColumn(old_games ? 0 : 1);  // base_stat, effort
        pokemon_stats_query.executeStep();
        base_stats["Defense"] = pokemon_stats_query.getColumn(0);                 // base_stat
        ev_yields["Defense"]  = pokemon_stats_query.getColumn(old_games ? 0 : 1); // base_stat, effort
        pokemon_stats_query.executeStep();
        base_stats["Speed"] = pokemon_stats_query.getColumn(0);                   // base_stat
        ev_yields["Speed"]  = pokemon_stats_query.getColumn(old_games ? 0 : 1);   // base_stat, effort
        query_stream.str("");
        if(old_games)
        {
            query_stream << "SELECT base_stat FROM pokemon_stats WHERE pokemon_id="
                         << pokemon_id << " AND stat_id=9";
            base_stats["Special"] = db->execAndGet(query_stream.str().c_str());  // base_stat
            ev_yields["Special"]  = db->execAndGet(query_stream.str().c_str());  // base_stat
        }
        else
        {
            query_stream << "SELECT base_stat,effort FROM pokemon_stats WHERE pokemon_id="
                         << pokemon_id << " AND stat_id IN (4,5)";
            SQLite::Statement pokemon_stats_query2(*db, query_stream.str().c_str());
            pokemon_stats_query2.executeStep();
            base_stats["Special Attack"] = pokemon_stats_query2.getColumn(0);     // base_stat
            ev_yields["Special Attack"]  = pokemon_stats_query2.getColumn(0);     // effort
        }

        /*
         * Types
         */
        query_stream.str("");
        query_stream << "SELECT type_id FROM pokemon_types WHERE pokemon_id="
                     << pokemon_id;
        SQLite::Statement pokemon_types_query(*db, query_stream.str().c_str());
        pokemon_types_query.executeStep();
        types.first = database::get_type_name(pokemon_types_query.getColumn(0)); // type_id
        if(pokemon_types_query.executeStep())
            types.second = database::get_type_name(pokemon_types_query.getColumn(0));
        else
            types.second = "None";

        if(generation < 5)
        {
            switch(pokedex_num)
            {
                case Species::BUTTERFREE:
                    if(generation > 1)
                        base_stats["Special Attack"] = 80;
                    break;

                case Species::BEEDRILL:
                    base_stats["Attack"] = 80;
                    break;

                case Species::PIDGEOT:
                    base_stats["Speed"] = 91;
                    break;

                case Species::PIKACHU:
                    base_stats["Defense"] = 30;
                    if(generation > 1)
                        base_stats["Special Defense"] = 40;
                    break;

                case Species::RAICHU:
                    base_stats["Speed"] = 100;
                    break;

                case Species::NIDOQUEEN:
                    base_stats["Attack"] = 82;
                    break;

                case Species::NIDOKING:
                    base_stats["Attack"] = 92;
                    break;

                case Species::CLEFAIRY:
                    types.first = "Normal";
                    break;

                case Species::CLEFABLE:
                    types.first = "Normal";
                    if(generation > 1)
                        base_stats["Special Attack"] = 85;
                    break;

                case Species::JIGGLYPUFF:
                    types.second = "None";
                    break;

                case Species::WIGGLYTUFF:
                    types.second = "None";
                    if(generation > 1)
                        base_stats["Special Attack"] = 75;
                    break;

                case Species::VILEPLUME:
                    if(generation > 1)
                        base_stats["Special Attack"] = 75;
                    break;

                case Species::POLIWRATH:
                    base_stats["Attack"] = 85;
                    break;

                case Species::ALAKAZAM:
                    if(generation > 1)
                        base_stats["Special Defense"] = 85;
                    break;

                case Species::VICTREEBEL:
                    if(generation > 1)
                        base_stats["Special Defense"] = 60;
                    break;

                case Species::GOLEM:
                    base_stats["Attack"] = 210;
                    break;

                case Species::MR_MIME:
                    types.second = "None";
                    break;

                case Species::IGGLYBUFF:
                    types.second = "None";
                    break;

                case Species::AMPHAROS:
                    base_stats["Defense"] = 75;
                    break;

                case Species::BELLOSSOM:
                    base_stats["Defense"] = 85;
                    break;

                case Species::MARILL:
                    types.second = "None";
                    break;

                case Species::AZUMARILL:
                    types.second = "None";
                    base_stats["Defense"] = 85;
                    break;

                case Species::JUMPLUFF:
                    base_stats["Special Defense"] = 85;
                    break;

                case Species::BEAUTIFLY:
                    base_stats["Special Attack"] = 90;
                    break;

                case Species::RALTS:
                case Species::KIRLIA:
                case Species::GARDEVOIR:
                    types.second = "None";
                    break;

                case Species::EXPLOUD:
                    base_stats["Special Defense"] = 63;
                    break;

                case Species::AZURILL:
                case Species::MAWILE:
                    types.second = "None";
                    break;

                case Species::STARAPTOR:
                    base_stats["Special Defense"] = 50;
                    break;

                case Species::ROSERADE:
                    base_stats["Defense"] = 55;
                    break;

                case Species::MIME_JR:
                    types.second = "None";
                    break;

                case Species::STOUTLAND:
                    base_stats["Attack"] = 100;
                    break;

                case Species::UNFEZANT:
                    base_stats["Attack"] = 105;
                    break;

                case Species::GIGALITH:
                    base_stats["Special Defense"] = 70;
                    break;

                case Species::SEISMITOAD:
                    base_stats["Attack"] = 85;
                    break;

                case Species::LEAVANNY:
                    base_stats["Special Defense"] = 70;
                    break;

                case Species::SCOLIPEDE:
                    base_stats["Attack"] = 90;
                    break;

                case Species::COTTONEE:
                case Species::WHIMSICOTT:
                    types.second = "None";
                    break;

                case Species::KROOKODILE:
                    base_stats["Defense"] = 70;
                    break;
            }

            hidden_ability = "None";
        }
        if(generation < 4)
        {
            has_gender_differences = false;
        }
        if(generation < 3)
        {
            abilities       = pkmn::pkstring_pair_t("None", "None");
            base_friendship = 0;
        }
        if(generation == 1)
        {
            egg_groups    = pkmn::pkstring_pair_t("None", "None");
            chance_male   = 0.0;
            chance_female = 0.0;
        }
    }

    pokemon_entry_t::pokemon_entry_t(const pkmn::pkstring& version_name,
                                     const pkmn::pkstring& species_name,
                                     const pkmn::pkstring& form_name)
    {
        uint16_t form_id;
        if(form_name == "Standard" or form_name == species_name or form_name == "")
            form_id = database::get_species_id(species_name);
        else
            form_id = database::get_form_id(species_name, form_name);

        pokemon_entry_t(database::get_version_id(version_name),
                        database::get_species_id(species_name),
                        form_id);
    }
} /* namespace pkmn */
