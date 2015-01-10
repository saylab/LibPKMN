/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_POKEDEX_HPP
#define INCLUDED_PKMN_POKEDEX_HPP

#include <cstdint>

#include <pkmn/config.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/pkstring.hpp>
#include <pkmn/types/pokemon_entry.hpp>
#include <pkmn/types/shared_ptr.hpp>

namespace pkmn
{
    class PKMN_API pokedex
    {
        public:

            typedef pkmn::shared_ptr<pokedex> sptr;

            static sptr make(const pkmn::pkstring &game);

            //! Class constructor (use factory function instead)
            pokedex() {};
            virtual ~pokedex() {};

            virtual pkmn::pkstring get_game() const = 0;

            virtual uint16_t get_generation() const = 0;

            virtual pokemon_entry_t& get_entry(const uint16_t species_id,
                                               const uint16_t form_id = 0) = 0;

            virtual pokemon_entry_t& get_entry(const pkmn::pkstring& species_name,
                                               const pkmn::pkstring& form_name = "") = 0;
    };
}

#endif /* INCLUDED_PKMN_POKEDEX_HPP */
