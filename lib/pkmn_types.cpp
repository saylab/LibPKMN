/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <pkmnsim/pkmn_types.hpp>

#include "internal.hpp"
#include "sqlitecpp/SQLiteCPP.h"

using namespace std;

namespace pkmnsim
{
    double get_type_damage_mod(string type1, string type2, bool gen1)
    {
        if(gen1 and (type1 == "Dark" or type1 == "Steel" or type2 == "Dark" or type2 == "Steel"))
        {
            throw runtime_error("Dark and Steel types invalid for Generation 1.");
        }

        double damage_mod;
        string query_string;

        if(type1 != "None" and type1 != "???" and type2 != "None" and type2 != "???" and type1 != "Shadow" and type2 != "Shadow")
        {
            SQLite::Database db(get_database_path().c_str());

            //Get type IDs
            query_string = "SELECT type_id FROM type_names WHERE name='" + type1 + "'";
            int type1_id = db.execAndGet(query_string.c_str(), type1);
            query_string = "SELECT type_id FROM type_names WHERE name='" + type2 + "'";
            int type2_id = db.execAndGet(query_string.c_str(), type1);

            //Get damage mod from database
            query_string = "SELECT damage_factor FROM type_efficacy WHERE damage_type_id=" + to_string(type1_id)
                         + " AND target_type_id=" + to_string(type2_id);
            damage_mod = db.execAndGet(query_string.c_str());

            //TODO: Verify
            damage_mod /= 100; //Stored as 50, 100, or 200 in database

            return damage_mod;
        }
        else return 1.0;
    }
}
