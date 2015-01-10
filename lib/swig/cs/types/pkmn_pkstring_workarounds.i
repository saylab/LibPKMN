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
 * bug is only present in versions 2.0.5-3.0.0, but I have encountered it
 * in 3.0.1, so this workaround is necessary.
 */

%extend pkmn::base_pokemon{
    std::pair<std::wstring, std::wstring> getAbilities(){
        pkmn::pkstring_pair_t abilities = self->get_abilities();

        std::pair<std::wstring, std::wstring> abilities2;
        abilities2.first = abilities.first;
        abilities2.second = abilities.second;

        return abilities2;
    }

    std::pair<std::wstring, std::wstring> getTypes(){
        pkmn::pkstring_pair_t types = self->get_types();

        std::pair<std::wstring, std::wstring> types2;
        types2.first = types.first;
        types2.second = types.second;

        return types2;
    }
}

%{
    struct PokemonEntry {
        std::wstring speciesName;
        uint16_t pokedexNum;
        std::wstring entry;
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
%}

struct PokemonEntry {
    std::wstring speciesName;
    uint16_t pokedexNum;
    std::wstring entry;
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

%extend pkmn::pokedex{
    PokemonEntry getEntry(const uint16_t speciesID,
                          const uint16_t formID = 0){

        pkmn::pokemon_entry_t entry1 = self->get_entry(speciesID, formID);
        PokemonEntry entry2;

        entry2.speciesName = entry1.species_name;
        entry2.pokedexNum = entry1.pokedex_num;
        entry2.entry = entry1.entry;
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

    PokemonEntry getEntry(const std::wstring& speciesName,
                          const std::wstring& formName = std::wstring()){

        pkmn::pokemon_entry_t entry1 = self->get_entry(speciesName, formName);
        PokemonEntry entry2;

        entry2.speciesName = entry1.species_name;
        entry2.pokedexNum = entry1.pokedex_num;
        entry2.entry = entry1.entry;
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
}

%extend pkmn::team_pokemon{
    std::pair<std::wstring, std::wstring> getTypes(){
        pkmn::pkstring_pair_t types = self->get_types();

        std::pair<std::wstring, std::wstring> types2;
        types2.first = types.first;
        types2.second = types.second;

        return types2;
    }
}

%ignore pkmn::base_pokemon::get_abilities;
%ignore pkmn::base_pokemon::get_types;
%ignore pkmn::pokemon_entry_t;
%ignore pkmn::pokedex::get_entry;
%ignore pkmn::team_pokemon::get_types;
