/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_IO_3GPKM_HPP
#define INCLUDED_IO_3GPKM_HPP

#include <pkmn/pokemon.hpp>
#include <pkmn/types/pkstring.hpp>

namespace pkmn
{
    namespace io
    {
        class _3gpkm
        {
            public:

                static bool valid(const pkmn::pkstring &filename);

                static pokemon::sptr from(const pkmn::pkstring &filename);

                static void to(pokemon::sptr pkmn, const pkmn::pkstring &filename);
        };
    }
}

#endif /* INCLUDED_IO_3GPKM_HPP */
