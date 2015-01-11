/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "std_wstring.i"

/*
 * This workaround is necessary because of a bug in SWIG in which typemaps
 * are not properly propogated into structs that use the given class.
 * This bug is theoretically only present in versions 2.0.5-3.0.0, but I
 * have encountered it in 3.0.1, so this workaround is necessary.
 */

%{
    struct PokemonEntry {
        std::wstring speciesName;
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

    struct MoveEntry {
        std::wstring name;
        std::wstring type;
        std::wstring description;
        std::wstring damageClass;
        std::wstring target;

        uint8_t power;
        uint8_t pp;
        float accuracy;
        int8_t priority;

        pkmn::pkstring effect;
        float effectChance;

        std::wstring contestType;
        std::wstring contestEffect;
        std::wstring superContestEffect;
    };

    struct ItemEntry {
        std::wstring name;
        std::wstring category;
        std::wstring pocket;
        std::wstring description;

        uint16_t cost;

        uint8_t flingPower;
        std::wstring flingEffect;
    };
%}

struct PokemonEntry {
    std::wstring speciesName;
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

struct MoveEntry {
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

struct ItemEntry {
    std::wstring name;
    std::wstring category;
    std::wstring pocket;
    std::wstring description;

    uint16_t cost;

    uint8_t flingPower;
    std::wstring flingEffect;
};

%extend pkmn::pokedex{
    PokemonEntry getPokemonEntry(const uint16_t speciesID,
                                const uint16_t formID = 0){

        pkmn::pokemon_entry_t entry1 = self->get_pokemon_entry(speciesID, formID);
        PokemonEntry entry2;

        entry2.speciesName = entry1.species_name;
        entry2.pokedexNum = entry1.pokedex_num;
        entry2.pokedexEntry = entry1.pokedex_entry;
        entry2.types.first = entry1.types.first;
        entry2.types.second = entry1.types.second;
        entry2.abilities.first = entry1.abilities.first;
        entry2.abilities.second = entry1.abilities.second;
        entry2.hiddenAbility = entry1.hidden_ability;
        entry2.eggGroups.first = entry1.egg_groups.first;
        entry2.eggGroups.second = entry1.egg_groups.second;
        entry2.catchRate = entry1.catch_rate;
        entry2.expYield = entry1.exp_yield;
        entry2.baseFriendship = entry1.base_friendship;
        entry2.height = entry1.height;
        entry2.weight = entry1.weight;
        entry2.chanceMale = entry1.chance_male;
        entry2.chanceFemale = entry1.chance_female;
        entry2.hasGenderDifferences = entry1.has_gender_differences;

        entry2.baseStats = entry1.base_stats;
        entry2.EVYields = entry1.ev_yields;

        return entry2;
    }

    PokemonEntry getPokemonEntry(const std::wstring& speciesName,
                                 const std::wstring& formName = std::wstring()){

        pkmn::pokemon_entry_t entry1 = self->get_pokemon_entry(speciesName, formName);
        PokemonEntry entry2;

        entry2.speciesName = entry1.species_name;
        entry2.pokedexNum = entry1.pokedex_num;
        entry2.pokedexEntry = entry1.pokedex_entry;
        entry2.types.first = entry1.types.first;
        entry2.types.second = entry1.types.second;
        entry2.abilities.first = entry1.abilities.first;
        entry2.abilities.second = entry1.abilities.second;
        entry2.hiddenAbility = entry1.hidden_ability;
        entry2.eggGroups.first = entry1.egg_groups.first;
        entry2.eggGroups.second = entry1.egg_groups.second;
        entry2.catchRate = entry1.catch_rate;
        entry2.expYield = entry1.exp_yield;
        entry2.baseFriendship = entry1.base_friendship;
        entry2.height = entry1.height;
        entry2.weight = entry1.weight;
        entry2.chanceMale = entry1.chance_male;
        entry2.chanceFemale = entry1.chance_female;
        entry2.hasGenderDifferences = entry1.has_gender_differences;

        entry2.baseStats = entry1.base_stats;
        entry2.EVYields = entry1.ev_yields;

        return entry2;
    }

    MoveEntry getMoveEntry(const uint16_t moveID){
        pkmn::move_entry_t entry1 = self->get_move_entry(moveID);
        MoveEntry entry2;

        entry2.name = entry1.name;
        entry2.type = entry1.type;
        entry2.description = entry1.description;
        entry2.damageClass = entry1.damage_class;
        entry2.target = entry1.target;
        entry2.power = entry1.power;
        entry2.pp = entry1.pp;
        entry2.accuracy = entry1.accuracy;
        entry2.priority = entry1.priority;
        entry2.effect = entry1.effect;
        entry2.effectChance = entry1.effect_chance;
        entry2.contestType = entry1.contest_type;
        entry2.contestEffect = entry1.contest_effect;
        entry2.superContestEffect = entry1.super_contest_effect;

        return entry2;
    }

    MoveEntry getMoveEntry(const std::wstring& moveName){
        pkmn::move_entry_t entry1 = self->get_move_entry(moveName);
        MoveEntry entry2;

        entry2.name = entry1.name;
        entry2.type = entry1.type;
        entry2.description = entry1.description;
        entry2.damageClass = entry1.damage_class;
        entry2.target = entry1.target;
        entry2.power = entry1.power;
        entry2.pp = entry1.pp;
        entry2.accuracy = entry1.accuracy;
        entry2.priority = entry1.priority;
        entry2.effect = entry1.effect;
        entry2.effectChance = entry1.effect_chance;
        entry2.contestType = entry1.contest_type;
        entry2.contestEffect = entry1.contest_effect;
        entry2.superContestEffect = entry1.super_contest_effect;

        return entry2;
    }

    ItemEntry getItemEntry(const uint16_t itemID){
        pkmn::item_entry_t entry1 = self->get_item_entry(itemID);
        ItemEntry entry2;

        entry2.name = entry1.name;
        entry2.category = entry1.category;
        entry2.pocket = entry1.pocket;
        entry2.description = entry1.description;
        entry2.cost = entry1.cost;
        entry2.flingPower = entry1.fling_power;;
        entry2.flingEffect = entry1.fling_effect;

        return entry2;
    }

    ItemEntry getItemEntry(const std::wstring& itemName){
        pkmn::item_entry_t entry1 = self->get_item_entry(itemName);
        ItemEntry entry2;

        entry2.name = entry1.name;
        entry2.category = entry1.category;
        entry2.pocket = entry1.pocket;
        entry2.description = entry1.description;
        entry2.cost = entry1.cost;
        entry2.flingPower = entry1.fling_power;;
        entry2.flingEffect = entry1.fling_effect;

        return entry2;
    }
}

%ignore pkmn::pokemon_entry_t;
%ignore pkmn::move_entry_t;
%ignore pkmn::item_entry_t;
%ignore pkmn::pokedex::get_pokemon_entry;
%ignore pkmn::pokedex::get_move_entry;
%ignore pkmn::pokedex::get_item_entry;

%{
    #include "pkmn/pokedex/pokemon_entry.hpp"
    #include "pkmn/pokedex/move_entry.hpp"
    #include "pkmn/pokedex/item_entry.hpp"
    #include "pkmn/pokedex.hpp"
%}

%include "pkmn/pokedex.hpp"
