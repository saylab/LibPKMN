/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_IO_PKSQL_HPP
#define INCLUDED_IO_PKSQL_HPP

#include <cstdint>

#include <pkmn/pokemon.hpp>
#include <pkmn/types/pkstring.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include "../SQLiteCpp/SQLiteC++.h"

/*
 * If the PKSQL format changes, this number must be incremented. If not,
 * any import will fail.
 */
#define PKSQL_COMPAT_NUM 2

namespace pkmn
{
    namespace io
    {
        class pksql
        {
            public:

                static void create_tables(pkmn::database_sptr db);

                static bool valid(pkmn::database_sptr db);

                static bool valid(const pkmn::pkstring &filename);

                static pokemon::sptr from(pkmn::database_sptr db, uint64_t id);

                static pokemon::sptr from(const pkmn::pkstring &filename);

                static uint64_t to(pokemon::sptr pkmn, pkmn::database_sptr db);

                static pkmn::database_sptr to(pokemon::sptr pkmn,
                                              const pkmn::pkstring &filename);

                static int native_sizes[7];

                static uint64_t latest_id;
        };
    }
}

#endif /* INCLUDED_IO_PKSQL_HPP */
