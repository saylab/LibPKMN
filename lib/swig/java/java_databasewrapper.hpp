/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_JAVA_DATABASEWRAPPER_HPP
#define INCLUDED_JAVA_DATABASEWRAPPER_HPP

#include <string>

#include <pkmn/config.hpp>
#include <pkmn/database/queries.hpp>

short PKMN_INLINE getAbilityID(const std::wstring &abilityName)
{
    return pkmn::database::get_ability_id(abilityName);
}
std::wstring PKMN_INLINE getAbilityName(const short abilityID)
{
    return pkmn::database::get_ability_name(abilityID);
}
std::wstring PKMN_INLINE getAbilityDescription(const short abilityID, const short versionID)
{
    return pkmn::database::get_ability_description(abilityID, versionID);
}
std::wstring PKMN_INLINE getAbilityDescription(const std::wstring &abilityName, const std::wstring &versionName)
{
    return pkmn::database::get_ability_description(abilityName, versionName);
}

short PKMN_INLINE getEggGroupID(const std::wstring &eggGroupName)
{
    return pkmn::database::get_egg_group_id(eggGroupName);
}
std::wstring PKMN_INLINE getEggGroupName(const short eggGroupID)
{
    return pkmn::database::get_egg_group_name(eggGroupID);
}

int PKMN_INLINE getExperience(const short speciesID, const short level)
{
    return pkmn::database::get_experience(speciesID, level);
}
int PKMN_INLINE getExperience(const std::wstring &speciesName, const short level)
{
    return pkmn::database::get_experience(speciesName, level);
}
short PKMN_INLINE getLevel(const short speciesID, const int experience)
{
    return pkmn::database::get_level(speciesID, experience);
}
short PKMN_INLINE getLevel(const std::wstring &speciesName, const int experience)
{
    return pkmn::database::get_level(speciesName, experience);
}

short PKMN_INLINE getGeneration(const short versionID)
{
    return pkmn::database::get_generation(versionID);
}
short PKMN_INLINE getGeneration(const std::wstring &versionName)
{
    return pkmn::database::get_generation(versionName);
}

short PKMN_INLINE getItemID(const std::wstring &itemName)
{
    return pkmn::database::get_item_id(itemName);
}
short PKMN_INLINE getItemID(const short gameIndex, const short versionID)
{
    return pkmn::database::get_item_id(gameIndex, versionID);
}
short PKMN_INLINE getItemID(const short gameIndex, const std::wstring &versionName)
{
    return pkmn::database::get_item_id(gameIndex, versionName);
}
std::wstring PKMN_INLINE getItemName(const short itemID)
{
    return pkmn::database::get_item_name(itemID);
}
std::wstring PKMN_INLINE getItemName(const short gameIndex, const short versionID)
{
    return pkmn::database::get_item_name(gameIndex, versionID);
}
std::wstring PKMN_INLINE getItemName(const short gameIndex, const std::wstring &versionName)
{
    return pkmn::database::get_item_name(gameIndex, versionName);
}
short PKMN_INLINE getItemCategoryID(const short itemID)
{
    return pkmn::database::get_item_category_id(itemID);
}
short PKMN_INLINE getItemCategoryID(const std::wstring &itemName)
{
    return pkmn::database::get_item_category_id(itemName);
}
std::wstring PKMN_INLINE getItemCategoryName(const short itemID)
{
    return pkmn::database::get_item_category_name(itemID);
}
std::wstring PKMN_INLINE getItemCategoryName(const std::wstring &itemName)
{
    return pkmn::database::get_item_category_name(itemName);
}
std::wstring PKMN_INLINE getItemDescription(const short itemID, const short versionID)
{
    return pkmn::database::get_item_description(itemID, versionID);
}
std::wstring PKMN_INLINE getItemDescription(const std::wstring &itemName, const std::wstring &versionName)
{
    return pkmn::database::get_item_description(itemName, versionName);
}
short PKMN_INLINE getItemGameIndex(const short itemID, const short versionID)
{
    return pkmn::database::get_item_game_index(itemID, versionID);
}
short PKMN_INLINE getItemGameIndex(const std::wstring &itemName, const std::wstring &versionName)
{
    return pkmn::database::get_item_game_index(itemName, versionName);
}

short PKMN_INLINE getMoveID(const std::wstring &moveName)
{
    return pkmn::database::get_move_id(moveName);
}
std::wstring PKMN_INLINE getMoveName(const short moveID)
{
    return pkmn::database::get_move_name(moveID);
}
short PKMN_INLINE getMoveDamageClassID(const short moveID)
{
    return pkmn::database::get_move_damage_class_id(moveID);
}
short PKMN_INLINE getMoveDamageClassID(const std::wstring &moveName)
{
    return pkmn::database::get_move_damage_class_id(moveName);
}
std::wstring PKMN_INLINE getMoveDamageClassName(const short moveID)
{
    return pkmn::database::get_move_damage_class_name(moveID);
}
std::wstring PKMN_INLINE getMoveDamageClassName(const std::wstring &moveName)
{
    return pkmn::database::get_move_damage_class_name(moveName);
}
std::wstring PKMN_INLINE getMoveDescription(const short moveID, const short versionID)
{
    return pkmn::database::get_move_description(moveID, versionID);
}
std::wstring PKMN_INLINE getMoveDescription(const std::wstring &moveName, const std::wstring &versionName)
{
    return pkmn::database::get_move_description(moveName, versionName);
}
short PKMN_INLINE getMovePP(const short moveID)
{
    return pkmn::database::get_move_pp(moveID);
}
short PKMN_INLINE getMovePP(const std::wstring &moveName)
{
    return pkmn::database::get_move_pp(moveName);
}

short PKMN_INLINE getNatureID(const std::wstring &natureName)
{
    return pkmn::database::get_nature_id(natureName);
}
std::wstring PKMN_INLINE getNatureName(const short natureID)
{
    return pkmn::database::get_nature_name(natureID);
}
double PKMN_INLINE getNatureStatMod(const short natureID, const short statID)
{
    return pkmn::database::get_nature_stat_mod(natureID, statID);
}
double PKMN_INLINE getNatureStatMod(const std::wstring &natureName, const std::wstring &statName)
{
    return pkmn::database::get_nature_stat_mod(natureName, statName);
}

std::wstring PKMN_INLINE getPokedexEntry(const short speciesID, const short versionID)
{
    return pkmn::database::get_pokedex_entry(speciesID, versionID);
}
std::wstring PKMN_INLINE getPokedexEntry(const std::wstring &speciesName, const std::wstring &versionName)
{
    return pkmn::database::get_pokedex_entry(speciesName, versionName);
}

short PKMN_INLINE getPokemonID(const short gameIndex, const short versionID)
{
    return pkmn::database::get_pokemon_id(gameIndex, versionID);
}
short PKMN_INLINE getPokemonID(const short gameIndex, const std::wstring &versionName)
{
    return pkmn::database::get_pokemon_id(gameIndex, versionName);
}
short PKMN_INLINE getPokemonID(const short formID)
{
    return pkmn::database::get_pokemon_id(formID);
}
short PKMN_INLINE getPokemonGameIndex(const short pokemonID, const short versionID)
{
    return pkmn::database::get_pokemon_game_index(pokemonID, versionID);
}
short PKMN_INLINE getPokemonGameIndex(const short pokemonID, const std::wstring &versionName)
{
    return pkmn::database::get_pokemon_game_index(pokemonID, versionName);
}

short PKMN_INLINE getSpeciesID(const std::wstring &speciesName)
{
    return pkmn::database::get_species_id(speciesName);
}
short PKMN_INLINE getSpeciesID(const short pokemonID)
{
    return pkmn::database::get_species_id(pokemonID);
}
std::wstring PKMN_INLINE getSpeciesName(const short speciesID)
{
    return pkmn::database::get_species_name(speciesID);
}

short PKMN_INLINE getStatID(const std::wstring &statName)
{
    return pkmn::database::get_stat_id(statName);
}
std::wstring PKMN_INLINE getStatName(const short statID)
{
    return pkmn::database::get_stat_name(statID);
}

short PKMN_INLINE getTypeID(const std::wstring &typeName)
{
    return pkmn::database::get_type_id(typeName);
}
std::wstring PKMN_INLINE getTypeName(const short typeID)
{
    return pkmn::database::get_type_name(typeID);
}

short PKMN_INLINE getVersionID(const std::wstring &versionName)
{
    return pkmn::database::get_version_id(versionName);
}
std::wstring PKMN_INLINE getVersionName(const short versionID)
{
    return pkmn::database::get_version_name(versionID);
}

short PKMN_INLINE getVersionGroupID(const short versionID)
{
    return pkmn::database::get_version_group_id(versionID);
}
short PKMN_INLINE getVersionGroupID(const std::wstring &versionName)
{
    return pkmn::database::get_version_group_id(versionName);
}
std::wstring PKMN_INLINE getVersionGroupName(const short versionGroupID)
{
    return pkmn::database::get_version_group_name(versionGroupID);
}

#endif /* INCLUDED_JAVA_DATABASEWRAPPER_HPP */
