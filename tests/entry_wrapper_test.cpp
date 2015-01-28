/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/pokedex.hpp>
#include "entry_wrappers.hpp"

/*
 * Due to a SWIG bug in which C# wrappers don't properly propogate typemaps, it
 * was necessary to manually write wrapper classes that use std::wstring instead
 * of pkmn::pkstring, except in the case of dictionaries. All entries should
 * be equal on both sides.
 */

BOOST_AUTO_TEST_CASE(item_entry_wrapper_test)
{
    pkmn::item_entry_t entry1(pkmn::Versions::DIAMOND, pkmn::Items::POTION);
    ItemEntry entry2(pkmn::Versions::DIAMOND, pkmn::Items::POTION);

    BOOST_CHECK(entry1.name == entry2.name);
    BOOST_CHECK(entry1.category == entry2.category);
    BOOST_CHECK(entry1.pocket == entry2.pocket);
    BOOST_CHECK(entry1.description == entry2.description);
    BOOST_CHECK(entry1.cost == entry2.cost);
    BOOST_CHECK(entry1.fling_power == entry2.flingPower);
    BOOST_CHECK(entry1.fling_effect == entry2.flingEffect);
}

BOOST_AUTO_TEST_CASE(move_entry_wrapper_test)
{
    pkmn::move_entry_t entry1(pkmn::Versions::DIAMOND, pkmn::Moves::BUBBLE);
    MoveEntry entry2(pkmn::Versions::DIAMOND, pkmn::Moves::BUBBLE);

    BOOST_CHECK(entry1.name == entry2.name);
    BOOST_CHECK(entry1.type == entry2.type);
    BOOST_CHECK(entry1.description == entry2.description);
    BOOST_CHECK(entry1.damage_class == entry2.damageClass);
    BOOST_CHECK(entry1.target == entry2.target);
    BOOST_CHECK(entry1.power == entry2.power);
    BOOST_CHECK(entry1.pp == entry2.pp);
    BOOST_CHECK(entry1.accuracy == entry2.accuracy);
    BOOST_CHECK(entry1.priority == entry2.priority);
    BOOST_CHECK(entry1.effect == entry2.effect);
    BOOST_CHECK(entry1.effect_chance == entry2.effectChance);
    BOOST_CHECK(entry1.contest_type == entry2.contestType);
    BOOST_CHECK(entry1.contest_effect == entry2.contestEffect);
    BOOST_CHECK(entry1.super_contest_effect == entry2.superContestEffect);
}

BOOST_AUTO_TEST_CASE(pokemon_entry_wrapper_test)
{
    pkmn::pokemon_entry_t entry1(pkmn::Versions::DIAMOND, pkmn::Species::LUCARIO,
                                 pkmn::Forms::Lucario::STANDARD);
    PokemonEntry entry2(pkmn::Versions::DIAMOND, pkmn::Species::LUCARIO,
                        pkmn::Forms::Lucario::STANDARD);

    BOOST_CHECK(entry1.species_name == entry2.speciesName);
    BOOST_CHECK(entry1.form == entry2.form);
    BOOST_CHECK(entry1.pokedex_num == entry2.pokedexNum);
    BOOST_CHECK(entry1.pokedex_entry == entry2.pokedexEntry);
    BOOST_CHECK(entry1.types.first == entry2.types.first);
    BOOST_CHECK(entry1.types.second == entry2.types.second);
    BOOST_CHECK(entry1.abilities.first == entry2.abilities.first);
    BOOST_CHECK(entry1.abilities.second == entry2.abilities.second);
    BOOST_CHECK(entry1.hidden_ability == entry2.hiddenAbility);
    BOOST_CHECK(entry1.egg_groups.first == entry2.eggGroups.first);
    BOOST_CHECK(entry1.egg_groups.second == entry2.eggGroups.second);
    BOOST_CHECK(entry1.catch_rate == entry2.catchRate);
    BOOST_CHECK(entry1.exp_yield == entry2.expYield);
    BOOST_CHECK(entry1.base_friendship == entry2.baseFriendship);
    BOOST_CHECK(entry1.height == entry2.height);
    BOOST_CHECK(entry1.weight == entry2.weight);
    BOOST_CHECK(entry1.chance_male == entry2.chanceMale);
    BOOST_CHECK(entry1.chance_female == entry2.chanceFemale);
    BOOST_CHECK(entry1.has_gender_differences == entry2.hasGenderDifferences);
    BOOST_FOREACH(const pkmn::pkstring& stat, entry1.base_stats.keys())
    {
        BOOST_CHECK(entry1.base_stats[stat] == entry2.baseStats[stat]);
    }
    BOOST_FOREACH(const pkmn::pkstring& stat, entry1.ev_yields.keys())
    {
        BOOST_CHECK(entry1.ev_yields[stat] == entry2.EVYields[stat]);
    }
}
