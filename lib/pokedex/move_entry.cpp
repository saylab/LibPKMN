/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <sstream>

#include <boost/format.hpp>

#include <pkmn/database.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/pokedex/move_entry.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include "internal.hpp"
#include "SQLiteCpp/SQLiteC++.h"

namespace pkmn
{
    static pkmn::shared_ptr<SQLite::Database> db; 

    move_entry_t::move_entry_t(uint16_t version_id,
                               uint16_t move_id)
    {   
        CONNECT_TO_DB(db);

        uint8_t generation = database::get_generation(version_id);

        std::ostringstream query_stream;
        query_stream << "SELECT generation_id FROM moves WHERE id=" << move_id;
        if(uint16_t(db->execAndGet(query_stream.str().c_str())) > generation)
            throw std::runtime_error("This move did not exist in this generation.");

        /*
         * The "moves" table should give us most things.
         */
        query_stream.str("");
        query_stream << "SELECT * FROM moves WHERE id=" << move_id;
        SQLite::Statement moves_query(*db, query_stream.str().c_str());
        moves_query.executeStep();

        name = database::get_move_name(move_id);
        type = database::get_type_name(moves_query.getColumn(3)); // type_id
        damage_class = database::get_move_damage_class_name(uint16_t(moves_query.getColumn(9))); // damage_class_id

        power = moves_query.getColumn(4); // power
        pp = moves_query.getColumn(5); // pp
        accuracy = float(moves_query.getColumn(6)) / float(100.0); // accuracy
        priority = moves_query.getColumn(7); // priority

        // effect = 
        effect_chance = float(moves_query.getColumn(11)) / float(100.0); // effect_chance

        /*
         * Description
         */
        query_stream.str("");
        query_stream << "SELECT flavor_text FROM move_flavor_text WHERE move_id="
                     << move_id << " AND language_id=9 AND version_group_id="
                     << database::get_version_group_id(version_id);
        SQLite::Statement flavor_text_query(*db, query_stream.str().c_str());
        description = get_pkstring_from_query(flavor_text_query);

        /*
         * Target
         */
        query_stream.str("");
        query_stream << "SELECT name FROM move_target_prose WHERE move_target_id="
                     << moves_query.getColumn(8) << " AND local_language_id=9"; // target_id
        SQLite::Statement move_target_prose_query(*db, query_stream.str().c_str());
        target = get_pkstring_from_query(move_target_prose_query);

        /*
         * Effect
         * TODO: parse tokens in short_effect
         */
        query_stream.str("");
        query_stream << "SELECT short_effect FROM move_effect_prose WHERE move_effect_id="
                     << moves_query.getColumn(10);  // effect_id
        SQLite::Statement move_effect_prose_query(*db, query_stream.str().c_str());
        effect = get_pkstring_from_query(move_effect_prose_query);

        /*
         * Contest type
         */
        query_stream.str("");
        query_stream << "SELECT name FROM contest_type_names WHERE contest_type_id="
                     << moves_query.getColumn(12) << " AND local_language_id=9"; // contest_type_id
        SQLite::Statement contest_type_names_query(*db, query_stream.str().c_str());
        contest_type = get_pkstring_from_query(contest_type_names_query);

        /*
         * Contest effect
         */
        query_stream.str("");
        query_stream << "SELECT flavor_text FROM contest_effect_prose WHERE contest_effect_id="
                     << moves_query.getColumn(13) << " AND local_language_id=9"; // contest_effect_id
        SQLite::Statement contest_effect_prose_query(*db, query_stream.str().c_str());
        contest_effect = get_pkstring_from_query(contest_effect_prose_query);

        /*
         * Super Contest effect
         */
        query_stream.str("");
        query_stream << "SELECT flavor_text FROM super_contest_effect_prose"
                     << " WHERE super_contest_effect_id=" << moves_query.getColumn(14)
                     << " AND local_language_id=9"; // super_contest_effect_id
        SQLite::Statement super_contest_effect_prose_query(*db, query_stream.str().c_str());
        super_contest_effect = get_pkstring_from_query(super_contest_effect_prose_query);

        /*
         * Database values are valid for Generation VI, this fixes entries
         * from earlier generations.
         */
        if(generation < 6)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT gen" << generation << "_accuracy FROM old_move_accuracies WHERE move_id=" << move_id;
            SQLite::Statement accuracy_query(*db, query_stream.str().c_str());
            if(accuracy_query.executeStep()) accuracy = float(accuracy_query.getColumn(0)) / 100.0;
        
            // Hypnosis varies in accuracy between games
            if(move_id == Moves::HYPNOSIS and (version_id == Versions::DIAMOND or version_id == Versions::PEARL))
                accuracy = 0.7;

            query_stream.str("");
            query_stream << "SELECT gen" << generation << "_power FROM old_move_powers WHERE move_id=" << move_id;
            SQLite::Statement power_query(*db, query_stream.str().c_str());
            if(power_query.executeStep()) power = power_query.getColumn(0);

            // Shadow Rush varies in power between Gamecube games
            if(move_id == Moves::SHADOW_RUSH and version_id == Versions::COLOSSEUM)
                power = 90; 

            query_stream.str("");
            query_stream << "SELECT gen" << generation << "_pp FROM old_move_pps WHERE move_id=" << move_id;
            SQLite::Statement pp_query(*db, query_stream.str().c_str());
            if(pp_query.executeStep())
                pp = pp_query.getColumn(0);

            // Not enough type changes to warrant a database table
            if(generation == 1)
            {
                if(move_id == Moves::BITE or move_id == Moves::GUST or
                   move_id == Moves::KARATE_CHOP or move_id == Moves::SAND_ATTACK)
                    type = "Normal";
            }
            else if(move_id == Moves::CURSE and generation < 4)
                type = "???";
            else if(move_id == Moves::CHARM or move_id == Moves::MOONLIGHT or
                    move_id == Moves::SWEET_KISS)
                type = "Normal";

            // Only one move changes categories before Generation IV
            if(generation == 1 and move_id == Moves::BITE)
                damage_class = "Physical";
        
            // TODO: targeting changes, making contact

            query_stream.str("");
            query_stream << "SELECT gen" << generation << "_priority FROM old_move_priorities WHERE move_id=" << move_id;
            SQLite::Statement priority_query(*db, query_stream.str().c_str());
            if(priority_query.executeStep())
                priority = priority_query.getColumn(0);

            // Only one move changed name between Generation II-III
            if(move_id == Moves::CONVERSION_2 and generation < 3)
                name = "Conversion2";

            query_stream.str("");
            query_stream << "SELECT name FROM old_move_names WHERE move_id=" << move_id;
            SQLite::Statement name_query(*db, query_stream.str().c_str());
            if(name_query.executeStep())
                name = name_query.getColumn(0);

            if(generation != 4)
                super_contest_effect = "None";

            if(generation < 4)
            {
                query_stream.str("");
                query_stream << "SELECT name FROM move_damage_class_prose WHERE local_language_id=9"
                             << " AND move_damage_class_id=(SELECT damage_class_id FROM types WHERE"
                             << " id=" << database::get_type_id(type) << ")";
                std::string damage_class_name = db->execAndGet(query_stream.str().c_str());
                damage_class_name[0] = ::toupper(damage_class_name[0]);
                damage_class = damage_class_name;
            }

            if(generation < 3)
            {
                contest_type   = "None";
                contest_effect = "None";
            }
        }
    }

    move_entry_t::move_entry_t(const pkmn::pkstring& version_name,
                               const pkmn::pkstring& move_name)
    {
        move_entry_t(database::get_version_id(version_name),
                     database::get_move_id(move_name));
    }
} /* namespace pkmn */
