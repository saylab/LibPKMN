/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_IO_TRSQL_HPP
#define INCLUDED_IO_TRSQL_HPP

#include <cstdint>

#include <pkmn/pokemon.hpp>
#include <pkmn/trainer.hpp>
#include <pkmn/types/pkstring.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include "../SQLiteCpp/SQLiteC++.h"

/*
 * If the TRSQL format changes, this number must be incremented. If not,
 * any import will fail.
 */
#define TRSQL_COMPAT_NUM 2

namespace pkmn
{
    namespace io
    {
        class trsql
        {
            public:

                static void create_tables(pkmn::database_sptr db);

                static bool valid(pkmn::database_sptr db);

                static bool valid(const pkmn::pkstring &filename);

                static trainer::sptr from(pkmn::database_sptr db, uint64_t id);

                static trainer::sptr from(const pkmn::pkstring &filename);

                static void add_invalid_pokemon(pkmn::database_sptr db, pokemon::sptr pkmn);

                static uint64_t to(trainer::sptr tr, pkmn::database_sptr db);

                static pkmn::database_sptr to(trainer::sptr tr,
                                              const pkmn::pkstring &filename);

                static uint64_t latest_id;
        };
    }
}

#endif /* INCLUDED_IO_TRSQL_HPP */
