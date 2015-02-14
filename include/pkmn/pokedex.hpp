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
#include <pkmn/pokedex/item_entry.hpp>
#include <pkmn/pokedex/move_entry.hpp>
#include <pkmn/pokedex/pokemon_entry.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/pkstring.hpp>
#include <pkmn/types/shared_ptr.hpp>

namespace pkmn
{
    /*!
     * This class represents a <a href="http://bulbapedia.bulbagarden.net/wiki/Pok%C3%A9dex">Pokédex</a>.
     * From this class, you can query any information about all Pokémon, items, and moves present in
     * the given game.
     */
    class PKMN_API pokedex
    {
        public:

            typedef pkmn::shared_ptr<pokedex> sptr;

            //! Factory function
            /*!
             * Takes in a game version name. All information will correspond to the
             * values in the given game, which takes into account changes between games
             * for things like Pokémon types and move base powers.
             *
             * \param game version of game this instance corresponds to
             * \return pokedex instance
             */
            static sptr make(const pkmn::pkstring &game);

            //! Class constructor (use factory function instead)
            pokedex() {};
            virtual ~pokedex() {};

            //! Return the name of the game this pokedex instance corresponds to
            virtual pkmn::pkstring get_game() const = 0;

            //! Return the generation of the game this pokedex instance corresponds to
            virtual uint16_t get_generation() const = 0;

            //! Return the Pokédex entry of the given Pokémon, using its SQLite ID's.
            /*!
             * From the given Pokémon species ID and (optional) form ID, generate and
             * return the given Pokémon entry. This entry has all relevant information
             * common to all instances of the given Pokémon. For Pokémon with different
             * forms, there is a different entry for each form.
             *
             * \param species_id SQLite ID of species whose entry to return
             * \param form_id SQLite ID of form of species whose entry to return (standard by default)
             * \return Pokédex entry for given Pokémon
             */
            virtual pokemon_entry_t get_pokemon_entry(uint16_t species_id,
                                                      uint16_t form_id = 0) = 0;

            //! Return the Pokédex entry of the given Pokémon.
            /*!
             * From the given Pokémon species and (optional) form, generate and
             * return the given Pokémon entry. This entry has all relevant information
             * common to all instances of the given Pokémon. For Pokémon with different
             * forms, there is a different entry for each form.
             *
             * \param species_name Name of species whose entry to return
             * \param form_name Name of form of species whose entry to return (standard by default)
             * \return Pokédex entry for given Pokémon
             */
            virtual pokemon_entry_t get_pokemon_entry(const pkmn::pkstring &species_name,
                                                      const pkmn::pkstring &form_name = "") = 0;

            //! Return the Pokédex entry of the given move, using its SQLite ID.
            /*!
             * Return all information regarding the given move.
             *
             * \param move_id SQLite ID of move whose entry to return
             * \return Pokédex entry for given move
             */
            virtual move_entry_t get_move_entry(uint16_t move_id) = 0;

            //! Return the Pokédex entry of the given move.
            /*!
             * Return all information regarding the given move.
             *
             * \param move_name SQLite ID of move whose entry to return
             * \return Pokédex entry for given move
             */
            virtual move_entry_t get_move_entry(const pkmn::pkstring &move_name) = 0;

            //! Return the Pokédex entry of the given item, using its SQLite ID.
            /*!
             * Return all information regarding the given item.
             *
             * \param item_id SQLite ID of item whose entry to return
             * \return Pokédex entry for given item
             */
            virtual item_entry_t get_item_entry(uint16_t item_id) = 0;

            //! Return the Pokédex entry of the given item.
            /*!
             * Return all information regarding the given item.
             *
             * \param item_name SQLite ID of item whose entry to return
             * \return Pokédex entry for given item
             */
            virtual item_entry_t get_item_entry(const pkmn::pkstring &item_name) = 0;

            //! Return the SQLite ID of the game this pokedex instance corresponds to
            virtual uint16_t get_game_id() const = 0;
    };
}

#endif /* INCLUDED_PKMN_POKEDEX_HPP */
