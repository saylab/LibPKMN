/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_POKEDEX_ITEM_ENTRY_HPP
#define INCLUDED_PKMN_POKEDEX_ITEM_ENTRY_HPP

#include <pkmn/config.hpp>
#include <pkmn/types/pkstring.hpp>

namespace pkmn
{
    /*!
     * This is the Pokédex entry for an in-game <a href="http://bulbapedia.bulbagarden.net/wiki/Item">item</a>.
     * When returned from a Pokédex, all information is game-specific, with the pocket name corresponding
     * to that game's bag structure, etc.
     */
    struct PKMN_API item_entry_t
    {
        //! Create an empty entry.
        item_entry_t() {};

        //! Create an entry using the SQLite ID's of the game version and item.
        item_entry_t(int version_id,
                     int item_id);

        //! Create an entry using the names of the game version and item.
        item_entry_t(const pkmn::pkstring &version_name,
                     const pkmn::pkstring &item_name);

        //! Item's name
        pkmn::pkstring name;

        //! Type of item (healing, ball, etc).
        pkmn::pkstring category;

        //! Name of pocket this item appears in in-game (game-specific).
        pkmn::pkstring pocket;

        //! Item description (game-specific).
        pkmn::pkstring description;

        //! Cost when bought at Poké Mart.
        int cost;

        //! Base power when used by the move Fling.
        int fling_power;
    };
}

#endif /* INCLUDED_PKMN_POKEDEX_ITEM_ENTRY_HPP */
