/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <sstream>

#include <pkmn/database.hpp>
#include <pkmn/paths.hpp>

#include "pokedex_impl.hpp"

namespace pkmn
{
    pokedex::sptr pokedex::make(const pkmn::pkstring& game)
    {
        return sptr(new pokedex_impl(game));
    }

    pokedex_impl::pokedex_impl(const pkmn::pkstring& game):
        pokedex(),
        _version_id(database::get_version_id(game))
    {
        if(!_db) _db = pkmn::shared_ptr<SQLite::Database>(new SQLite::Database(get_database_path()));
    }

    pkmn::dict<uint16_t, pkmn::dict<uint16_t, pokedex_entry_t> > pokedex_impl::_entry_cache;
    pkmn::shared_ptr<SQLite::Database>                           pokedex_impl::_db;

    pkmn::pkstring pokedex_impl::get_game() const
    {
        return database::get_version_name(_version_id);
    }

    uint16_t pokedex_impl::get_generation() const
    {
        return database::get_generation(_version_id);
    }

    pokedex_entry_t& pokedex_impl::get_entry(const uint16_t species_id,
                                             const uint16_t form_id)
    {
        uint16_t pokemon_id;

        // Use default form if nothing given
        if(form_id == 0 or form_id == species_id) pokemon_id = species_id;
        else
        {
            std::ostringstream query_stream;
            query_stream << "SELECT pokemon_id FROM pokemon_forms WHERE id="
                         << form_id;
            pokemon_id = _db->execAndGet(query_stream.str().c_str());
        }

        _create_entry(pokemon_id);
        return _entry_cache[_version_id][pokemon_id];
    }

    pokedex_entry_t& pokedex_impl::get_entry(const pkmn::pkstring& species_name,
                                             const pkmn::pkstring& form_name)
    {
        // TODO: form name to ID
        return get_entry(database::get_species_id(species_name));
    }

    void pokedex_impl::_create_entry(const uint16_t pokemon_id)
    {
        if(_entry_cache[_version_id].has_key(pokemon_id)) return;

        pokedex_entry_t entry;

        std::ostringstream query_stream;

        /*
         * Everything from "pokemon" table
         */
        query_stream << "SELECT * FROM pokemon WHERE id=" << pokemon_id;
        SQLite::Statement pokemon_query(*_db, query_stream.str().c_str());
        pokemon_query.executeStep();
        entry.pokedex_num = pokemon_query.getColumn(2);           // species_id
        entry.species_name = database::get_species_name(entry.pokedex_num);
        entry.height = float(pokemon_query.getColumn(3)) / float(10.0); // height
        entry.weight = float(pokemon_query.getColumn(4)) / float(10.0); // weight
        entry.exp_yield = pokemon_query.getColumn(5);             // base_experience

        /*
         * Everything from "pokemon_species" table
         */
        query_stream.str("");
        query_stream << "SELECT * FROM pokemon_species WHERE id="
                     << database::get_species_id(pokemon_id);
        SQLite::Statement pokemon_species_query(*_db, query_stream.str().c_str());
        pokemon_species_query.executeStep();
        entry.chance_male = _get_gender_chance(entry.pokedex_num,
                                               pokemon_species_query.getColumn(8),
                                               true);                       // gender_rate
        entry.chance_female = _get_gender_chance(entry.pokedex_num,
                                                 pokemon_species_query.getColumn(8),
                                                 false);                    // gender_rate
        entry.catch_rate = pokemon_species_query.getColumn(9);              // capture_rate
        entry.base_friendship = pokemon_species_query.getColumn(10);        // base_happiness
        entry.has_gender_differences = pokemon_species_query.getColumn(13); // has_gender_differences

        bool old_games = get_generation() < 3;

        /*
         * Abilities
         */
        query_stream.str("");
        query_stream << "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id="
                     << pokemon_id << " AND is_hidden=0";
        SQLite::Statement pokemon_abilities_query(*_db, query_stream.str().c_str());
        pokemon_abilities_query.executeStep();
        entry.abilities.first = database::get_ability_name(pokemon_abilities_query.getColumn(0)); // ability_id
        if(pokemon_abilities_query.executeStep())
            entry.abilities.second = database::get_ability_name(pokemon_abilities_query.getColumn(0)); // ability_id
        else
            entry.abilities.second = "None";
        if(get_generation() < 5)
            entry.hidden_ability = "None";
        else
        {
            query_stream.str("");
            query_stream << "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id="
                         << pokemon_id << " AND is_hidden=1";
            SQLite::Statement pokemon_abilities_query2(*_db, query_stream.str().c_str());
            if(pokemon_abilities_query2.executeStep())
                entry.hidden_ability = database::get_ability_name(pokemon_abilities_query2.getColumn(0));
            else
                entry.hidden_ability = "None";
        }

        /*
         * Egg groups
         */
        query_stream.str("");
        query_stream << "SELECT egg_group_id FROM pokemon_egg_groups WHERE species_id="
                     << entry.pokedex_num;
        SQLite::Statement pokemon_egg_groups_query(*_db, query_stream.str().c_str());
        pokemon_egg_groups_query.executeStep();
        entry.egg_groups.first = database::get_egg_group_name(pokemon_egg_groups_query.getColumn(0));
        if(pokemon_egg_groups_query.executeStep())
            entry.egg_groups.second = database::get_egg_group_name(pokemon_egg_groups_query.getColumn(0));
        else
            entry.egg_groups.second = "None";

        /*
         * Base stats, effort yields
         */
        query_stream.str("");
        query_stream << "SELECT base_stat,effort FROM pokemon_stats WHERE pokemon_id="
                     << pokemon_id << " AND stat_id IN (1,2,3,6)";
        SQLite::Statement pokemon_stats_query(*_db, query_stream.str().c_str());
        pokemon_stats_query.executeStep();
        entry.base_stats["HP"] = pokemon_stats_query.getColumn(0);                      // base_stat
        entry.ev_yields["HP"]  = pokemon_stats_query.getColumn(old_games ? 0 : 1);      // base_stat, effort
        pokemon_stats_query.executeStep();
        entry.base_stats["Attack"] = pokemon_stats_query.getColumn(0);                  // base_stat
        entry.ev_yields["Attack"]  = pokemon_stats_query.getColumn(old_games ? 0 : 1);  // base_stat, effort
        pokemon_stats_query.executeStep();
        entry.base_stats["Defense"] = pokemon_stats_query.getColumn(0);                 // base_stat
        entry.ev_yields["Defense"]  = pokemon_stats_query.getColumn(old_games ? 0 : 1); // base_stat, effort
        pokemon_stats_query.executeStep();
        entry.base_stats["Speed"] = pokemon_stats_query.getColumn(0);                   // base_stat
        entry.ev_yields["Speed"]  = pokemon_stats_query.getColumn(old_games ? 0 : 1);   // base_stat, effort
        query_stream.str("");
        if(old_games)
        {
            query_stream << "SELECT base_stat FROM pokemon_stats WHERE pokemon_id="
                         << pokemon_id << " AND stat_id=9";
            entry.base_stats["Special"] = _db->execAndGet(query_stream.str().c_str());  // base_stat
            entry.ev_yields["Special"]  = _db->execAndGet(query_stream.str().c_str());  // base_stat
        }
        else
        {
            query_stream << "SELECT base_stat,effort FROM pokemon_stats WHERE pokemon_id="
                         << pokemon_id << " AND stat_id IN (4,5)";
            SQLite::Statement pokemon_stats_query2(*_db, query_stream.str().c_str());
            pokemon_stats_query2.executeStep();
            entry.base_stats["Special Attack"] = pokemon_stats_query2.getColumn(0);     // base_stat
            entry.ev_yields["Special Attack"]  = pokemon_stats_query2.getColumn(0);     // effort
        }

        /*
         * Types
         */
        query_stream.str("");
        query_stream << "SELECT type_id FROM pokemon_types WHERE pokemon_id="
                     << pokemon_id;
        SQLite::Statement pokemon_types_query(*_db, query_stream.str().c_str());
        pokemon_types_query.executeStep();
        entry.types.first = database::get_type_name(pokemon_types_query.getColumn(0)); // type_id
        if(pokemon_types_query.executeStep())
            entry.types.second = database::get_type_name(pokemon_types_query.getColumn(0));
        else
            entry.types.second = "None";

        _adjust_entry(entry);
        _entry_cache[_version_id][pokemon_id] = entry;
    }

    float pokedex_impl::_get_gender_chance(const uint16_t species_id,
                                           const uint8_t gender_rate,
                                           bool male) const
    {
        /*
         * gender_val_dict's keys correspond to how the different
         * gender rates are represented in the database. The values
         * are the actual decimal representations of the percentages.
         */
        pkmn::dict<uint8_t, float> gender_val_dict; // Double is percentage male
        gender_val_dict[0] = 1.0;
        gender_val_dict[1] = 0.875;
        gender_val_dict[2] = 0.75;
        gender_val_dict[4] = 0.5;
        gender_val_dict[6] = 0.25;
        gender_val_dict[8] = 0.0;

        std::ostringstream query_stream;
        query_stream << "SELECT gender_rate FROM pokemon_species WHERE id=" << species_id;
        int gender_val = _db->execAndGet(query_stream.str().c_str());

        if(gender_val == -1) return 0.0;
        else return male ? gender_val_dict[gender_val]
                         : (1 - gender_val_dict[gender_val]);
    }

    /*
     * Database values are valid for Generation VI, this fixes entries
     * from earlier generations.
     */
    void pokedex_impl::_adjust_entry(pokedex_entry_t& entry)
    {
        uint8_t generation = get_generation();

        if(generation < 5)
        {
            entry.hidden_ability = "None";
        }
        if(generation < 4)
        {
            entry.has_gender_differences = false;
        }
        if(generation < 3)
        {
            entry.abilities       = pkmn::pkstring_pair_t("None", "None");
            entry.base_friendship = 0;
        }
        if(generation == 1)
        {
            entry.egg_groups    = pkmn::pkstring_pair_t("None", "None");
            entry.chance_male   = 0.0;
            entry.chance_female = 0.0;
        }
    }
} /* namespace pkmn */
