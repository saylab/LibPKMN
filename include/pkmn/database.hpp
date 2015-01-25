/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_DATABASE_HPP
#define INCLUDED_PKMN_DATABASE_HPP

#include <cstdint>

#include <pkmn/config.hpp>
#include <pkmn/types/pkstring.hpp>

namespace pkmn
{
    namespace database
    {
        uint16_t PKMN_API get_ability_id(const pkmn::pkstring& ability_name);
        pkmn::pkstring PKMN_API get_ability_name(uint16_t ability_id);
        pkmn::pkstring PKMN_API get_ability_description(uint16_t ability_id, uint16_t version_id);
        pkmn::pkstring PKMN_API get_ability_description(const pkmn::pkstring& ability_name, const pkmn::pkstring& version_name);

        uint16_t PKMN_API get_ball_id(const pkmn::pkstring& ball_name);
        pkmn::pkstring PKMN_API get_ball_name(uint16_t ball_id);

        uint16_t PKMN_API get_egg_group_id(const pkmn::pkstring& egg_group_name);
        pkmn::pkstring PKMN_API get_egg_group_name(uint16_t egg_group_id);

        uint16_t PKMN_API get_form_id(const pkmn::pkstring& species_name, const pkmn::pkstring& form_name);
        pkmn::pkstring PKMN_API get_form_name(uint16_t form_id);
        uint8_t PKMN_API get_form_game_index(uint16_t form_id);
        uint16_t PKMN_API get_form_id(uint16_t species_id, uint8_t form_index);

        uint32_t PKMN_API get_experience(uint16_t species_id, uint16_t level);
        uint32_t PKMN_API get_experience(const pkmn::pkstring& species_name, uint16_t level);
        uint16_t PKMN_API get_level(uint16_t species_id, uint32_t experience);
        uint16_t PKMN_API get_level(const pkmn::pkstring& species_name, uint32_t experience);

        uint16_t PKMN_API get_generation(uint16_t version_id);
        uint16_t PKMN_API get_generation(const pkmn::pkstring& version_name);

        uint16_t PKMN_API get_item_id(const pkmn::pkstring& item_name);
        uint16_t PKMN_API get_item_id(uint16_t game_index, uint16_t version_id);
        uint16_t PKMN_API get_item_id(uint16_t game_index, const pkmn::pkstring& version_name);
        pkmn::pkstring PKMN_API get_item_name(uint16_t item_id);
        pkmn::pkstring PKMN_API get_item_name(uint16_t game_index, uint16_t version_id);
        pkmn::pkstring PKMN_API get_item_name(uint16_t game_index, const pkmn::pkstring& version_name);
        uint16_t PKMN_API get_item_category_id(uint16_t item_id);
        uint16_t PKMN_API get_item_category_id(const pkmn::pkstring& item_name);
        pkmn::pkstring PKMN_API get_item_category_name(uint16_t item_id);
        pkmn::pkstring PKMN_API get_item_category_name(const pkmn::pkstring& item_name);
        pkmn::pkstring PKMN_API get_item_description(uint16_t item_id, uint16_t version_id);
        pkmn::pkstring PKMN_API get_item_description(const pkmn::pkstring& item_name, const pkmn::pkstring& version_name);
        uint16_t PKMN_API get_item_game_index(uint16_t item_id, uint16_t version_id);
        uint16_t PKMN_API get_item_game_index(const pkmn::pkstring& item_name, const pkmn::pkstring& version_name);

        uint16_t PKMN_API get_move_id(const pkmn::pkstring& move_name);
        pkmn::pkstring PKMN_API get_move_name(uint16_t move_id);
        uint16_t PKMN_API get_move_damage_class_id(uint16_t move_id);
        uint16_t PKMN_API get_move_damage_class_id(const pkmn::pkstring& move_name);
        pkmn::pkstring PKMN_API get_move_damage_class_name(uint16_t move_id);
        pkmn::pkstring PKMN_API get_move_damage_class_name(const pkmn::pkstring& move_name);
        pkmn::pkstring PKMN_API get_move_description(uint16_t move_id, uint16_t version_id);
        pkmn::pkstring PKMN_API get_move_description(const pkmn::pkstring& move_name, const pkmn::pkstring& version_name);
        uint16_t PKMN_API get_move_pp(uint16_t move_id);
        uint16_t PKMN_API get_move_pp(const pkmn::pkstring& move_name);

        uint16_t PKMN_API get_nature_id(const pkmn::pkstring& nature_name);
        pkmn::pkstring PKMN_API get_nature_name(uint16_t nature_id);
        double PKMN_API get_nature_stat_mod(uint16_t nature_id, uint16_t stat_id);
        double PKMN_API get_nature_stat_mod(const pkmn::pkstring& nature_name, const pkmn::pkstring& stat_name);

        pkmn::pkstring PKMN_API get_pokedex_entry(uint16_t species_id, uint16_t version_id);
        pkmn::pkstring PKMN_API get_pokedex_entry(const pkmn::pkstring& species_name, const pkmn::pkstring& version_name);

        uint16_t PKMN_API get_pokemon_id(uint16_t game_index, uint16_t version_id);
        uint16_t PKMN_API get_pokemon_id(uint16_t game_index, const pkmn::pkstring& version_name);
        uint16_t PKMN_API get_pokemon_id(uint16_t form_id);
        uint16_t PKMN_API get_pokemon_game_index(uint16_t pokemon_id, uint16_t version_id);
        uint16_t PKMN_API get_pokemon_game_index(uint16_t pokemon_id, const pkmn::pkstring& version_name);

        uint16_t PKMN_API get_species_id(const pkmn::pkstring& species_name);
        uint16_t PKMN_API get_species_id(uint16_t pokemon_id);
        pkmn::pkstring PKMN_API get_species_name(uint16_t species_id);

        uint16_t PKMN_API get_stat_id(const pkmn::pkstring& stat_name);
        pkmn::pkstring PKMN_API get_stat_name(uint16_t stat_id);

        uint16_t PKMN_API get_type_id(const pkmn::pkstring& type_name);
        pkmn::pkstring PKMN_API get_type_name(uint16_t type_id);

        uint16_t PKMN_API get_version_id(const pkmn::pkstring& version_name);
        pkmn::pkstring PKMN_API get_version_name(uint16_t version_id);

        uint16_t PKMN_API get_version_game_index(const pkmn::pkstring& version_name);
        uint16_t PKMN_API get_version_game_index(uint16_t version_id);
        uint16_t PKMN_API get_version_id(uint16_t game_index);

        uint16_t PKMN_API get_version_group_id(uint16_t version_id);
        uint16_t PKMN_API get_version_group_id(const pkmn::pkstring& version_name);
        pkmn::pkstring PKMN_API get_version_group_name(uint16_t version_group_id);
    }
}

#endif /* INCLUDED_PKMN_DATABASE_HPP */
