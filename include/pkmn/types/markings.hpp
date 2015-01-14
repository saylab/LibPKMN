/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TYPES_MARKINGS_HPP
#define INCLUDED_PKMN_TYPES_MARKINGS_HPP

#include <cstdint>

#include <pkmn/config.hpp>

namespace pkmn
{
    /*!
     * This class represents a Pokémon's <a href="http://bulbapedia.bulbagarden.net/wiki/Pok%C3%A9mon_Storage_System#Markings">markings</a>.
     * In Generation III onward, these markings are used to organize Pokémon within the PC.
     */
    struct PKMN_API markings_t
    {
        markings_t() {};

        //! Instantiate the class with the in-game marking storage method.
        markings_t(uint8_t markint);

        bool circle;
        bool triangle;
        bool square;
        bool heart;
        bool star;
        bool diamond;

        //! Convert this struct to its in-game storage method.
        operator uint8_t();
    };
}

#endif /* INCLUDED_PKMN_TYPES_MARKINGS_HPP */
