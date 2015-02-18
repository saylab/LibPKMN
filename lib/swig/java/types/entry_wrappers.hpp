/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_JAVA_ENTRY_WRAPPERS_HPP
#define INCLUDED_JAVA_ENTRY_WRAPPERS_HPP

#include <utility>

#include <pkmn/database.hpp>
#include <pkmn/pokedex.hpp>
#include <pkmn/pokedex/pokemon_entry.hpp>
#include <pkmn/pokedex/move_entry.hpp>
#include <pkmn/pokedex/item_entry.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/pkstring.hpp>

namespace pkmn
{
    namespace java
    {
        /*! 
         * This is the Pokédex entry for an in-game <a href="http://bulbapedia.bulbagarden.net/wiki/Item">item</a>.
         * When returned from a Pokédex, all information is game-specific, with the pocket name corresponding
         * to that game's bag structure, etc.
         */
        class ItemEntry
        {
            public:

                //! Create an empty entry.
                ItemEntry() {};

                //! Create an entry using the SQLite ID's of the game version and item.
                ItemEntry(uint16_t versionID, uint16_t itemID)
                {
                    entry = pkmn::item_entry_t(versionID, itemID);
                }

                //! Create an entry using the names of the game version and item.
                ItemEntry(const pkmn::pkstring &versionName, const pkmn::pkstring &itemName)
                {
                    entry = pkmn::item_entry_t(versionName, itemName);
                }

                //! Item's name
                pkmn::pkstring& getName()
                {
                    return entry.name;
                }

                //! Type of item (healing, ball, etc).
                pkmn::pkstring& getCategory()
                {
                    return entry.category;
                }

                //! Name of pocket this item appears in in-game (game-specific).
                pkmn::pkstring& getPocket()
                {
                    return entry.pocket;
                }

                //! Item description (game-specific).
                pkmn::pkstring& getDescription()
                {
                    return entry.description;
                }

                //! Cost when bought at Poké Mart.
                uint16_t getCost()
                {
                    return entry.cost;
                }

                //! Base power when used by the move Fling.
                uint16_t getFlingPower()
                {
                    return entry.fling_power;
                }

            private:

                pkmn::item_entry_t entry;
        };

        /*! 
         * This is the Pokédex entry for a <a href="http://bulbapedia.bulbagarden.net/wiki/Move">move</a>.
         * When returned from a Pokédex, all information is game-specific, taking into account changes
         * in move types, powers, and accuracies.
         */
        class MoveEntry
        {
            public:

                //! Create an empty entry.
                MoveEntry() {};

                //! Create an entry using the SQLite ID's of the game version and item.
                MoveEntry(uint16_t versionID, uint16_t moveID)
                {
                    entry = pkmn::move_entry_t(versionID, moveID);
                }

                //! Create an entry using the names of the game version and item.
                MoveEntry(const pkmn::pkstring &versionName, const pkmn::pkstring &moveName)
                {
                    entry = pkmn::move_entry_t(versionName, moveName);
                }

                //! Move's name
                pkmn::pkstring& getName()
                {
                    return entry.name;
                }

                //! Move's type (generation-specific).
                pkmn::pkstring& getType()
                {
                    return entry.type;
                }

                //! Move's description (game_specific).
                pkmn::pkstring& getDescription()
                {
                    return entry.description;
                }

                //! Move's damage class (Physical, Special, Effect)
                pkmn::pkstring& getDamageClass()
                {
                    return entry.damage_class;
                }

                //! Which Pokémon are affected in a Double/Triple battle.
                pkmn::pkstring& getTarget()
                {
                    return entry.target;
                }

                //! Move's base power
                uint16_t getPower()
                {
                    return entry.power;
                }

                //! Move's base <a href="http://bulbapedia.bulbagarden.net/wiki/PP">PP</a>, without using any items.
                uint16_t getPP()
                {
                    return entry.pp;
                }

                //! Move's base accuracy (0.0-1.0).
                float getAccuracy()
                {
                    return entry.accuracy;
                }

                //! Move's <a href="http://bulbapedia.bulbagarden.net/wiki/Priority">move priority</a> (-8 - 8).
                uint16_t getPriority()
                {
                    return entry.priority;
                }

                //! Move's effect (paralysis, etc).
                pkmn::pkstring& getEffect()
                {
                    return entry.effect;
                }

                //! Chance of move's effect being inflicted (0.0-1.0).
                float getEffectChance()
                {
                    return entry.effect_chance;
                }

                //! Move's <a href="http://bulbapedia.bulbagarden.net/wiki/Pok%C3%A9mon_Contest">contest</a> type (Cool, Beauty, Cute, Smart).
                pkmn::pkstring& getContestType()
                {
                    return entry.contest_type;
                }

                //! Effect of move when used in a <a href="http://bulbapedia.bulbagarden.net/wiki/Pok%C3%A9mon_Contest">contest</a>.
                pkmn::pkstring& getContestEffect()
                {
                    return entry.contest_effect;
                }

                //! Effect of move when used in a <a href="http://bulbapedia.bulbagarden.net/wiki/Pok%C3%A9mon_Super_Contest">Super Contest</a>.
                pkmn::pkstring& getSuperContestEffect()
                {
                    return entry.super_contest_effect;
                }

            private:

                pkmn::move_entry_t entry;
        };

        /*! 
         * This is the Pokédex entry for a Pokémon. When returned from a Pokédex, all
         * information is game-specific, taking into account type changes, added forms,
         * etc. All information is form-specific.
         */
        class PokemonEntry
        {
            public:

                //! Create an empty entry.
                PokemonEntry() {};

                //! Create an entry using the SQLite ID's of the game version, species, and form.
                PokemonEntry(uint16_t versionID, uint16_t speciesID, uint16_t formID)
                {
                    entry = pkmn::pokemon_entry_t(versionID, speciesID, formID);
                }

                //! Create an entry using the names of the game version, species, and form.
                PokemonEntry(const pkmn::pkstring &versionName,
                             const pkmn::pkstring &speciesName,
                             const pkmn::pkstring &formName)
                {
                    entry = pkmn::pokemon_entry_t(versionName, speciesName, formName);
                }

                //! Pokémon's species name (independent of form).
                pkmn::pkstring& getSpeciesName()
                {
                    return entry.species_name;
                }

                //! Pokémon's <a href="http://bulbapedia.bulbagarden.net/wiki/National_Pok%C3%A9dex">National Pokédex</a> number.
                uint16_t getPokedexNum()
                {
                    return entry.pokedex_num;
                }

                //! Pokémon's form. "Standard" if not in any alternate form.
                pkmn::pkstring& getForm()
                {
                    return entry.form;
                }

                //! Pokémon's Pokédex entry text for the given game.
                pkmn::pkstring& getPokedexEntry()
                {
                    return entry.pokedex_entry;
                }

                //! Pokémon's <a href="http://bulbapedia.bulbagarden.net/wiki/Type">type(s)</a> (generation-specific).
                pkmn::pkstring_pair_t& getTypes()
                {
                    return entry.types;
                }

                //! Pokémon's <a href="http://bulbapedia.bulbagarden.net/wiki/Ability">abilities</a> (both "None" pre-Generation III).
                pkmn::pkstring_pair_t& getAbilities()
                {
                    return entry.abilities;
                }

                //! Pokémon's hidden <a href="http://bulbapedia.bulbagarden.net/wiki/Ability">ability</a> ("None" pre-Generation V).
                pkmn::pkstring& getHiddenAbility()
                {
                    return entry.hidden_ability;
                }

                //! Pokémon's <a href="http://bulbapedia.bulbagarden.net/wiki/Egg_Group">egg groups</a>.
                pkmn::pkstring_pair_t& getEggGroups()
                {
                    return entry.egg_groups;
                }

                //! Pokémon's <a href="http://bulbapedia.bulbagarden.net/wiki/Catch_rate">catch rate</a>.
                uint16_t getCatchRate()
                {
                    return entry.catch_rate;
                }

                //! The base value that determines how much experience is given when this Pokémon faints.
                uint16_t getExpYield()
                {
                    return entry.exp_yield;
                }

                //! The level of <a href="http://bulbapedia.bulbagarden.net/wiki/Friendship">friendship</a> this Pokémon has upon capture.
                uint16_t getBaseFriendship()
                {
                    return entry.base_friendship;
                }

                //! Pokémon's height.
                float getHeight()
                {
                    return entry.height;
                }

                //! Pokémon's weight.
                float getWeight()
                {
                    return entry.weight;
                }

                //! Percentage of this Pokémon that are male (0.0-1.0).
                float getChanceMale()
                {
                    return entry.chance_male;
                }

                //! Percentage of this Pokémon that are female (0.0-1.0).
                float getChanceFemale()
                {
                    return entry.chance_female;
                }

                //! Whether or not there are visual differences between male and female Pokémon of this species.
                bool hasGenderDifferences()
                {
                    return entry.has_gender_differences;
                }

                //! Pokémon's <a href="http://bulbapedia.bulbagarden.net/wiki/Base_stats">base stats</a>, used to determine its specific stats.
                pkmn::dict<pkmn::pkstring, uint16_t>& getBaseStats()
                {
                    return entry.base_stats;
                }

                //! Pokémon's <a href="http://bulbapedia.bulbagarden.net/wiki/Effort_value">EV</a> yields.
                pkmn::dict<pkmn::pkstring, uint16_t>& getEVYields()
                {
                    return entry.ev_yields;
                }

            private:

                pkmn::pokemon_entry_t entry;
        };
    }
}

#endif /* INCLUDED_JAVA_ENTRY_WRAPPERS_HPP */
