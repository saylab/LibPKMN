/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_INTERNAL_HPP
#define INCLUDED_INTERNAL_HPP

#include <cstdint>

#include <pkmn/config.hpp>

#define PKSTRING_UPPERCASE(str) boost::algorithm::to_upper_copy(str.std_wstring())

namespace pkmn
{
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
}

#endif /* INCLUDED_INTERNAL_HPP */
