/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TYPES_POKEMON_ENTRY_HPP
#define INCLUDED_PKMN_TYPES_POKEMON_ENTRY_HPP

#include <cstdint>

#include <pkmn/config.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/pkstring.hpp>

namespace pkmn
{
    struct PKMN_API pokemon_entry_t
    {
        pokemon_entry_t() {};
        pokemon_entry_t(uint16_t version_id,
                        uint16_t species_id,
                        uint16_t form_id);
        pokemon_entry_t(const pkmn::pkstring& version_name,
                        const pkmn::pkstring& species_name,
                        const pkmn::pkstring& form_name);

        pkmn::pkstring species_name;
        uint16_t pokedex_num;
        pkmn::pkstring pokedex_entry;
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
    };
}

#endif /* INCLUDED_PKMN_TYPES_POKEMON_ENTRY_HPP */
