/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_DATABASE_LISTS_HPP
#define INCLUDED_PKMN_DATABASE_LISTS_HPP

#include <iostream>
#include <string>
#include <vector>

#include <pkmn/config.hpp>

namespace pkmn
{
    namespace database
    {
        //! Return a list of the tables in LibPKMN's database.
        /*!
         * Runs the SQLite query: "SELECT name FROM sqlite_master WHERE type='table'"
         *
         * \param vec vector in which to place the list table list
         */
        void PKMN_API get_table_list(std::vector<std::string> &vec);

        //! Return a list of columns in the given table in LibPKMN's database.
        /*!
         * Runs the SQLite query: "PRAGMA table_info(table_name)"
         *
         * \param vec vector in which to place the column list
         * \param table_name name of table whose columns to list
         */
        void PKMN_API get_column_list(std::vector<std::string> &vec, const std::string &table_name);
    }
}

#endif /* INCLUDED_PKMN_DATABASE_LISTS_HPP */
