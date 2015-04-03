/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TYPES_POKEMON_ENTRY_HPP
#define INCLUDED_PKMN_TYPES_POKEMON_ENTRY_HPP

#include <vector>

#include <pkmn/config.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/pkstring.hpp>

namespace pkmn
{
    /*!
     * This is the Pokédex entry for a Pokémon. When returned from a Pokédex, all
     * information is game-specific, taking into account type changes, added forms,
     * etc. All information is form-specific.
     */
    struct PKMN_API pokemon_entry_t
    {
        //! Create an empty entry.
        pokemon_entry_t() {};

        //! Create an entry using the SQLite ID's of the game version, species, and form.
        pokemon_entry_t(int version_id,
                        int species_id,
                        int form_id);

        //! Create an entry using the names of the game version, species, and form.
        pokemon_entry_t(const pkmn::pkstring& version_name,
                        const pkmn::pkstring& species_name,
                        const pkmn::pkstring& form_name);

        //! Pokémon's species name (independent of form).
        pkmn::pkstring species_name;

        //! Pokémon's <a href="http://bulbapedia.bulbagarden.net/wiki/National_Pok%C3%A9dex">National Pokédex</a> number.
        int pokedex_num;

        //! Pokémon's form. "Standard" if not in any alternate form.
        pkmn::pkstring form;

        //! Pokémon's Pokédex entry text for the given game.
        pkmn::pkstring pokedex_entry;

        //! Pokémon's <a href="http://bulbapedia.bulbagarden.net/wiki/Type">type(s)</a> (generation-specific).
        pkmn::pkstring_pair_t types;

        //! Pokémon's <a href="http://bulbapedia.bulbagarden.net/wiki/Ability">abilities</a> (both "None" pre-Generation III).
        pkmn::pkstring_pair_t abilities;

        //! Pokémon's hidden <a href="http://bulbapedia.bulbagarden.net/wiki/Ability">ability</a> ("None" pre-Generation V).
        pkmn::pkstring hidden_ability;

        //! Pokémon's <a href="http://bulbapedia.bulbagarden.net/wiki/Egg_Group">egg groups</a>.
        pkmn::pkstring_pair_t egg_groups;

        //! Pokémon's <a href="http://bulbapedia.bulbagarden.net/wiki/Catch_rate">catch rate</a>.
        int catch_rate;

        //! The base value that determines how much experience is given when this Pokémon faints.
        int exp_yield;

        //! The level of <a href="http://bulbapedia.bulbagarden.net/wiki/Friendship">friendship</a> this Pokémon has upon capture.
        int base_friendship;

        //! Pokémon's height.
        float height;

        //! Pokémon's weight.
        float weight;

        //! Percentage of this Pokémon that are male (0.0-1.0).
        float chance_male;

        //! Percentage of this Pokémon that are female (0.0-1.0).
        float chance_female;

        //! Whether or not there are visual differences between male and female Pokémon of this species.
        bool has_gender_differences;

        //! Pokémon's <a href="http://bulbapedia.bulbagarden.net/wiki/Base_stats">base stats</a>, used to determine its specific stats.
        pkmn::dict<pkmn::pkstring, int> base_stats;

        //! Pokémon's <a href="http://bulbapedia.bulbagarden.net/wiki/Effort_value">EV</a> yields.
        pkmn::dict<pkmn::pkstring, int> ev_yields;
    };

    //! List of Pokémon entries.
    typedef std::vector<pokemon_entry_t> pokemon_entry_vector_t;
}

#endif /* INCLUDED_PKMN_TYPES_POKEMON_ENTRY_HPP */
