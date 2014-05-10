/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <stdexcept>

#include <boost/format.hpp>

#include <pkmn/database/lists.hpp>
#include <pkmn/paths.hpp>

#include "../SQLiteCpp/src/SQLiteC++.h"

using namespace std;

namespace pkmn
{
    namespace database
    {
        static SQLite::Database db(get_database_path().c_str());

        void get_table_list(vector<string> &vec)
        {
            vec.clear();

            string query_str = "SELECT name FROM sqlite_master WHERE type='table'";
            SQLite::Statement query(db, query_str.c_str());

            while(query.executeStep())
            {
                string table_name = query.getColumn(0);
                vec.push_back(table_name);
            }
        }

        void get_column_list(vector<string> &vec, string table_name)
        {
            vec.clear();

            string query_str = str(boost::format("PRAGMA table_info(%s)") % table_name);
            SQLite::Statement query(db, query_str.c_str());

            while(query.executeStep())
            {
                string column_name = query.getColumn(1);
                vec.push_back(column_name);
            }
        }
    } /* namespace database */
} /* namespace pkmn */