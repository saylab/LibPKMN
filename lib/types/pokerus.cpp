/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/types/pokerus.hpp>
#include <pkmn/types/prng.hpp>

namespace pkmn
{
    pokerus_t::pokerus_t()
    {
        prng::sptr rng = prng::make(3);

        strain = strain_t((rng->mtrng() % 16) / 4);
        num_days = (strain % 4) + 1;
    }

    pokerus_t::pokerus_t(uint8_t native)
    {
        strain = strain_t((native & 0xFF00) >> 4);
        num_days = (native & 0xFF);
    }

    pokerus_t::operator uint8_t()
    {
        return (uint8_t(strain << 4) | uint8_t(num_days & 0xFF));
    }
}
