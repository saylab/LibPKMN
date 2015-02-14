/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_POKEDEX_MOVE_ENTRY_HPP
#define INCLUDED_PKMN_POKEDEX_MOVE_ENTRY_HPP

#include <cstdint>
#include <vector>

#include <pkmn/config.hpp>
#include <pkmn/types/pkstring.hpp>

namespace pkmn
{
    /*!
     * This is the Pokédex entry for a <a href="http://bulbapedia.bulbagarden.net/wiki/Move">move</a>.
     * When returned from a Pokédex, all information is game-specific, taking into account changes
     * in move types, powers, and accuracies.
     */
    struct PKMN_API move_entry_t
    {
        //! Create an empty entry.
        move_entry_t() {};

        //! Create an entry using the SQLite ID's of the game version and item.
        move_entry_t(uint16_t version_id,
                     uint16_t move_id);

        //! Create an entry using the names of the game version and item.
        move_entry_t(const pkmn::pkstring &version_name,
                     const pkmn::pkstring &move_name);

        //! Move's name
        pkmn::pkstring name;

        //! Move's type (generation-specific).
        pkmn::pkstring type;

        //! Move's description (game-specific).
        pkmn::pkstring description;

        //! Move's damage class (Physical, Special, Effect)
        pkmn::pkstring damage_class;

        //! Which Pokémon are affected in a Double/Triple battle
        pkmn::pkstring target;

        //! Move's base power
        uint16_t power;

        //! Move's base <a href="http://bulbapedia.bulbagarden.net/wiki/PP">PP</a>, without using any items.
        uint16_t pp;

        //! Move's base accuracy (0.0-1.0).
        float accuracy;

        //! Move's <a href="http://bulbapedia.bulbagarden.net/wiki/Priority">move priority</a> (-8 - 8).
        int8_t priority;

        //! Move's effect (paralysis, etc).
        pkmn::pkstring effect;

        //! Chance of move's effect being inflicted (0.0-1.0).
        float effect_chance;

        //! Move's <a href="http://bulbapedia.bulbagarden.net/wiki/Pok%C3%A9mon_Contest">contest</a> type (Cool, Beauty, Cute, Smart).
        pkmn::pkstring contest_type;

        //! Effect of move when used in a <a href="http://bulbapedia.bulbagarden.net/wiki/Pok%C3%A9mon_Contest">contest</a>.
        pkmn::pkstring contest_effect;

        //! Effect of move when used in a <a href="http://bulbapedia.bulbagarden.net/wiki/Pok%C3%A9mon_Super_Contest">contest</a>.
        pkmn::pkstring super_contest_effect;
    };

    //! Representation of moveset in the pokemon class
    typedef std::vector<move_entry_t> moveset_t;
}

#endif /* INCLUDED_PKMN_POKEDEX_MOVE_ENTRY_HPP */
