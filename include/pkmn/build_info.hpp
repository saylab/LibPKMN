/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_BUILD_INFO_HPP
#define INCLUDED_PKMN_BUILD_INFO_HPP

#include <string>

#include <pkmn/config.hpp>

namespace pkmn
{
    //! Return version of SQLite used in build
    std::string PKMN_API get_sqlite3_version();

    //! Return version of SQLiteC++ used in build
    std::string PKMN_API get_sqlitecpp_version();

    //! Return version of Boost used in build
    std::string PKMN_API get_boost_version();

    //! Return version of Qt4 used in build
    std::string PKMN_API get_qt4_version();

    //! Return LibPKMN commit used in build
    std::string PKMN_API get_libpkmn_commit();

    //! Return PKMDS commit used in build
    std::string PKMN_API get_pkmds_commit();
}

#endif /* INCLUDED_PKMN_BUILD_INFO_HPP */
