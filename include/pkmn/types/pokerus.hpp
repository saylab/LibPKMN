/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TYPES_POKERUS_HPP
#define INCLUDED_PKMN_TYPES_POKERUS_HPP

#include <cstdint>

#include <pkmn/config.hpp>

namespace pkmn
{
    struct PKMN_API pokerus_t
    {
        pokerus_t();

        //! Instantiate the class with the in-game storage method.
        pokerus_t(uint8_t native);

        enum strain_t
        {
            A,
            B,
            C,
            D
        };

        strain_t strain;
        int num_days;

        //! Convert this struct to its in-game storage method.
        operator uint8_t();
    };
}

#endif /* INCLUDED_PKMN_TYPES_POKERUS_HPP */
