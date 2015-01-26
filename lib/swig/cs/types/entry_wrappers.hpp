/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_ENTRY_WRAPPERS_HPP
#define INCLUDED_ENTRY_WRAPPERS_HPP

#include <utility>

#include <pkmn/database.hpp>
#include <pkmn/pokedex.hpp>
#include <pkmn/pokedex/pokemon_entry.hpp>
#include <pkmn/pokedex/move_entry.hpp>
#include <pkmn/pokedex/item_entry.hpp>
#include <pkmn/types/dict.hpp>

struct ItemEntry
{
    ItemEntry() {};

    ItemEntry(uint16_t versionID, uint16_t itemID)
    {
        pkmn::item_entry_t entry(versionID, itemID);

        name = entry.name;
        category = entry.category;
        pocket = entry.pocket;
        description = entry.description;
        cost = entry.cost;
        flingPower = entry.fling_power;
        flingEffect = entry.fling_effect;
    }

    ItemEntry(const std::wstring& versionName, const std::wstring& itemName)
    {
        ItemEntry(pkmn::database::get_version_id(versionName),
                  pkmn::database::get_item_id(itemName));
    }

    std::wstring name;
    std::wstring category;
    std::wstring pocket;
    std::wstring description;

    uint16_t cost;

    uint8_t flingPower;
    std::wstring flingEffect;
};

struct MoveEntry
{
    MoveEntry() {};

    MoveEntry(uint16_t versionID, uint16_t moveID)
{
        pkmn::move_entry_t entry(versionID, moveID);

        name = entry.name;
        type = entry.type;
        description = entry.description;
        damageClass = entry.damage_class;
        target = entry.target;
        power = entry.power;
        pp = entry.pp;
        accuracy = entry.accuracy;
        priority = entry.priority;
        effect = entry.effect;
        effectChance = entry.effect_chance;
        contestType = entry.contest_type;
        contestEffect = entry.contest_effect;
        superContestEffect = entry.super_contest_effect;
    }

    MoveEntry(const std::wstring& versionName, const std::wstring& moveName)
    {
        MoveEntry(pkmn::database::get_version_id(versionName),
                  pkmn::database::get_move_id(moveName));
    }

    std::wstring name;
    std::wstring type;
    std::wstring description;
    std::wstring damageClass;
    std::wstring target;

    uint8_t power;
    uint8_t pp;
    float accuracy;
    int8_t priority;

    std::wstring effect;
    float effectChance;

    std::wstring contestType;
    std::wstring contestEffect;
    std::wstring superContestEffect;
};

struct PokemonEntry
{
    PokemonEntry() {};

    PokemonEntry(uint16_t versionID, uint16_t speciesID, uint16_t formID)
    {
        pkmn::pokemon_entry_t entry(versionID, speciesID, formID);

        speciesName = entry.species_name;
        form = entry.form;
        pokedexNum = entry.pokedex_num;
        pokedexEntry = entry.pokedex_entry;
        types = std::make_pair(entry.types.first, entry.types.second);
        abilities = std::make_pair(entry.abilities.first, entry.abilities.second);
        hiddenAbility = entry.hidden_ability;
        eggGroups = std::make_pair(entry.egg_groups.first, entry.egg_groups.second);
        catchRate = entry.catch_rate;
        expYield = entry.exp_yield;
        baseFriendship = entry.base_friendship;
        height = entry.height;
        weight = entry.weight;
        chanceMale = entry.chance_male;
        chanceFemale = entry.chance_female;
        hasGenderDifferences = entry.has_gender_differences;
        baseStats = entry.base_stats;
        EVYields = entry.ev_yields;
    }

    PokemonEntry(const std::wstring& versionName,
                 const std::wstring& speciesName,
                 const std::wstring& formName = pkmn::pkstring(""))
    {
        uint16_t formID;
        if((pkmn::pkstring(formName) == "Standard") or
           (formName == speciesName) or
           (pkmn::pkstring(formName) == ""))
        {
            formID = pkmn::database::get_species_id(speciesName);
        }
        else
            formID = pkmn::database::get_form_id(speciesName, formName);

        PokemonEntry(pkmn::database::get_version_id(versionName),
                     pkmn::database::get_species_id(speciesName),
                     formID);
    }

    std::wstring speciesName;
    std::wstring form;
    uint16_t pokedexNum;
    std::wstring pokedexEntry;
    std::pair<std::wstring, std::wstring> types;
    std::pair<std::wstring, std::wstring> abilities;
    std::wstring hiddenAbility;
    std::pair<std::wstring, std::wstring> eggGroups;
    uint8_t catchRate;
    uint16_t expYield;
    uint8_t baseFriendship;
    float height;
    float weight;
    float chanceMale;
    float chanceFemale;
    bool hasGenderDifferences;

    pkmn::dict<pkmn::pkstring, uint16_t> baseStats;
    pkmn::dict<pkmn::pkstring, uint16_t> EVYields;
};

#endif /* INCLUDED_ENTRY_WRAPPERS_HPP */
