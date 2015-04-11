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
#include <pkmn/pokedex/item_entry.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include "internal.hpp"
#include "SQLiteCpp/SQLiteC++.h"

namespace pkmn
{
    static pkmn::shared_ptr<SQLite::Database> db; 
    static pkmn::item_entry_t none_entry;
    static pkmn::item_entry_t invalid_entry;
    static bool entries_created = false;

    static void create_none_invalid_entries()
    {
        // None entry
        none_entry.name = "None";
        none_entry.category = "None";
        none_entry.pocket = "None";
        none_entry.description = "None";
        none_entry.cost = 0;
        none_entry.fling_power = 0;

        // Invalid entry
        invalid_entry = none_entry;
        none_entry.name = "Invalid";

        entries_created = true;
    }

    item_entry_t::item_entry_t(int version_id,
                               int item_id)
    {   
        CONNECT_TO_DB(db);
        if(not entries_created)
            create_none_invalid_entries();

        if(item_id == Items::NONE)
        {
            *this = none_entry;
            return;
        }
        else if(item_id == Items::INVALID)
        {
            *this = invalid_entry;
            return;
        }

        std::ostringstream query_stream;
        query_stream << "SELECT generation_id FROM item_game_indices WHERE item_id="
                     << item_id;
        SQLite::Statement query(*db, query_stream.str().c_str());
        if(not query.executeStep())
            throw std::runtime_error("This item did not exist in this game.");

        int generation        = database::get_generation(version_id);
        int version_group_id = database::get_version_group_id(version_id);

        query_stream.str("");
        query_stream << "SELECT * FROM items WHERE id=" << item_id;
        SQLite::Statement items_query(*db, query_stream.str().c_str());
        items_query.executeStep();

        name = database::get_item_name(item_id);
        category = database::get_item_category_name(int(items_query.getColumn(2))); // category_id

        /*
         * Pocket name
         */
        query_stream.str("");
        query_stream << "SELECT name FROM pocket_names WHERE version_group_id="
                     << version_group_id << " AND pocket_id=(SELECT pocket_id" // category_id
                     << " FROM item_categories WHERE id=" << int(items_query.getColumn(2))
                     << ")";
        pocket = db->execAndGet(query_stream.str().c_str());

        /*
         * Description
         */
        query_stream.str("");
        int machine_id;
        bool is_machine = ((item_id >= Items::TM01 and item_id <= Items::HM08)
                           or (item_id >= Items::TM93 and item_id <= Items::TM95)
                           or (item_id >= Items::TM96 and item_id <= Items::TM100));

        if(is_machine)
        {
            if(item_id >= Items::TM01 and item_id <= Items::TM92)
                machine_id = item_id - 304;
            else
                machine_id = 101 + (item_id - Items::HM01);

            query_stream << "SELECT move_id FROM machines WHERE machine_number=" << machine_id
                         << " AND version_group_id=" << version_group_id;
            SQLite::Statement machines_query(*db, query_stream.str().c_str());
            int move_id = get_num_from_query<int>(machines_query);

            query_stream.str("");
            query_stream << "SELECT flavor_text FROM move_flavor_text WHERE move_id="
                         << move_id << " AND version_group_id=" << version_group_id
                         << " AND language_id=9";
            SQLite::Statement description_query(*db, query_stream.str().c_str());
            try
            {
                pkmn::pkstring move_description = get_pkstring_from_query(description_query);

                description = str(boost::format("%s - %s")
                                        % database::get_move_name(move_id)
                                        % move_description);
            }
            catch(...)
            {
                description = "Unavailable";
            }
        }
        else
        {
            query_stream << "SELECT flavor_text FROM item_flavor_text WHERE item_id="
                         << item_id << " AND version_group_id=" << version_group_id
                         << " AND language_id=9";
            SQLite::Statement description_query(*db, query_stream.str().c_str());

            try
            {
                description = get_pkstring_from_query(description_query);
            }
            catch(...)
            {
                description = "Unavailable";
            }
        }

        cost = items_query.getColumn(3); // cost

        /*
         * Fling power
         */
        if(generation < 4)
            fling_power  = 0;
        else
            fling_power = items_query.getColumn(4); // fling_power
    }

    item_entry_t::item_entry_t(const pkmn::pkstring &version_name,
                               const pkmn::pkstring &item_name)
    {
        item_entry_t(database::get_version_id(version_name),
                     database::get_item_id(item_name));
    }
} /* namespace pkmn */
