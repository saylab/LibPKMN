/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
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
#include <pkmn/types/shared_ptr.hpp>

namespace pkmn
{
    typedef struct
    {
        pkmn::pkstring species_name;
        uint16_t pokedex_num;
        pkmn::pkstring_pair_t types;
        pkmn::pkstring_pair_t abilities;
        pkmn::pkstring hidden_ability;
        pkmn::pkstring_pair_t egg_groups;
        uint8_t catch_rate;
        uint16_t exp_yield;
        uint8_t base_friendship;
        float height;
        float weight;
        float chance_male;
        float chance_female;
        bool has_gender_differences;

        pkmn::dict<pkmn::pkstring, uint16_t> base_stats;
        pkmn::dict<pkmn::pkstring, uint16_t> ev_yields;
    } pokedex_entry_t;

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

            virtual pokedex_entry_t& get_entry(const uint16_t species_id,
                                               const uint16_t form_id = 0) = 0;

            virtual pokedex_entry_t& get_entry(const pkmn::pkstring& species_name,
                                               const pkmn::pkstring& form_name = "") = 0;
    };
}

#endif /* INCLUDED_PKMN_POKEDEX_HPP */
