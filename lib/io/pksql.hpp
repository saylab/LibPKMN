/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_IO_PKSQL_HPP
#define INCLUDED_IO_PKSQL_HPP

#include <pkmn/pokemon.hpp>
#include <pkmn/types/pkstring.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include "../SQLiteCpp/SQLiteC++.h"

namespace pkmn
{
    namespace io
    {
        class pksql
        {
            static std::string query(pokemon::sptr pkmn);

            static pokemon::sptr from(const pkmn::pkstring &filename);

            static pkmn::database_sptr to(pokemon::sptr pkmn,
                                          const pkmn::pkstring &filename);
        };
    }
}

#endif /* INCLUDED_IO_PKSQL_HPP */
