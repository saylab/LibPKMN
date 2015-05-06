/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_INTERNAL_HPP
#define INCLUDED_INTERNAL_HPP

#include <cstdint>
#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include <pkmn/config.hpp>

#include "SQLiteCpp/SQLiteC++.h"

#define CONNECT_TO_DB(db) if(!db) db = pkmn::database_sptr(new SQLite::Database(get_database_path()));
#define THROW_QUERY_ERROR(stream) throw std::runtime_error(str(boost::format("Invalid query \"%s\"") \
                                                               % stream.str().c_str()));

#define PKSTRING_UPPERCASE(str) boost::algorithm::to_upper_copy(str.std_wstring())

namespace pkmn
{
    uint8_t PKMN_INLINE count_ones(uint16_t val)
    {   
        uint8_t num_ones = 0;

        for(size_t i = 0; i < 16; i++)
        {   
            if(val & 1) num_ones++;
            val >>= 1;
        }

        return num_ones;
    }

    /*
     * This is an abstraction for getting the value of an environment
     * variable, using the UNIX or Windows functions as appropriate.
     */
    std::string PKMN_INLINE getenv(const char* var)
    {
        #ifdef PKMN_PLATFORM_WIN32
        char* arr;
        size_t len;
        _dupenv_s(&arr, &len, var);
        if(!arr) return std::string("");
        else
        {
            std::string value = std::string(arr);
            free(arr);
            return value;
        }
        #else
        const char* arr = ::getenv(var);
        return arr ? std::string(arr) : std::string("");
        #endif
    }

    template<typename T>
    T get_num_from_query(SQLite::Statement& query)
    {
        std::stringstream query_stream(query.getQuery());

        if(query.executeStep()) return query.getColumn(0); \
        else THROW_QUERY_ERROR(query_stream);
    }

    pkmn::pkstring PKMN_INLINE get_pkstring_from_query(SQLite::Statement& query)
    {
        std::stringstream query_stream(query.getQuery());

        if(query.executeStep())
        {
            pkmn::pkstring entry = query.getColumn(0);
            std::wstring intermediate, s;
            std::wistringstream iss(entry.std_wstring());
            entry = "";
            while(iss >> s)
            {
                if(intermediate.size() > 0) intermediate += L" " + s;
                else intermediate = s;
            }
            return pkmn::pkstring(intermediate);
        }
        else THROW_QUERY_ERROR(query_stream);
    }
}

#endif /* INCLUDED_INTERNAL_HPP */
