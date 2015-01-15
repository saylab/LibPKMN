/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_DATABASE_HPP
#define INCLUDED_PKMN_DATABASE_HPP

#include <pkmn/config.hpp>

#include <pkmn/types/pkstring.hpp>

namespace pkmn
{
    namespace database
    {
        unsigned int PKMN_API get_ability_id(const pkmn::pkstring &ability_name);
        pkmn::pkstring PKMN_API get_ability_name(const unsigned int ability_id);
        pkmn::pkstring PKMN_API get_ability_description(const unsigned int ability_id, const unsigned int version_id);
        pkmn::pkstring PKMN_API get_ability_description(const pkmn::pkstring &ability_name, const pkmn::pkstring &version_name);

        unsigned int PKMN_API get_ball_id(const pkmn::pkstring &ball_name);
        pkmn::pkstring PKMN_API get_ball_name(const unsigned int ball_id);

        unsigned int PKMN_API get_egg_group_id(const pkmn::pkstring &egg_group_name);
        pkmn::pkstring PKMN_API get_egg_group_name(const unsigned int egg_group_id);

        unsigned int PKMN_API get_form_id(const pkmn::pkstring &species_name, const pkmn::pkstring &form_name);
        pkmn::pkstring PKMN_API get_form_name(const unsigned int form_id);

        unsigned int PKMN_API get_experience(const unsigned int species_id, const unsigned int level);
        unsigned int PKMN_API get_experience(const pkmn::pkstring &species_name, const unsigned int level);
        unsigned int PKMN_API get_level(const unsigned int species_id, const unsigned int experience);
        unsigned int PKMN_API get_level(const pkmn::pkstring &species_name, const unsigned int experience);

        unsigned int PKMN_API get_generation(const unsigned int version_id);
        unsigned int PKMN_API get_generation(const pkmn::pkstring &version_name);

        unsigned int PKMN_API get_item_id(const pkmn::pkstring &item_name);
        unsigned int PKMN_API get_item_id(const unsigned int game_index, const unsigned int version_id);
        unsigned int PKMN_API get_item_id(const unsigned int game_index, const pkmn::pkstring &version_name);
        pkmn::pkstring PKMN_API get_item_name(const unsigned int item_id);
        pkmn::pkstring PKMN_API get_item_name(const unsigned int game_index, const unsigned int version_id);
        pkmn::pkstring PKMN_API get_item_name(const unsigned int game_index, const pkmn::pkstring &version_name);
        unsigned int PKMN_API get_item_category_id(const unsigned int item_id);
        unsigned int PKMN_API get_item_category_id(const pkmn::pkstring &item_name);
        pkmn::pkstring PKMN_API get_item_category_name(const unsigned int item_id);
        pkmn::pkstring PKMN_API get_item_category_name(const pkmn::pkstring &item_name);
        pkmn::pkstring PKMN_API get_item_description(const unsigned int item_id, const unsigned int version_id);
        pkmn::pkstring PKMN_API get_item_description(const pkmn::pkstring &item_name, const pkmn::pkstring &version_name);
        unsigned int PKMN_API get_item_game_index(const unsigned int item_id, const unsigned int version_id);
        unsigned int PKMN_API get_item_game_index(const pkmn::pkstring &item_name, const pkmn::pkstring &version_name);

        unsigned int PKMN_API get_move_id(const pkmn::pkstring &move_name);
        pkmn::pkstring PKMN_API get_move_name(const unsigned int move_id);
        unsigned int PKMN_API get_move_damage_class_id(const unsigned int move_id);
        unsigned int PKMN_API get_move_damage_class_id(const pkmn::pkstring &move_name);
        pkmn::pkstring PKMN_API get_move_damage_class_name(const unsigned int move_id);
        pkmn::pkstring PKMN_API get_move_damage_class_name(const pkmn::pkstring &move_name);
        pkmn::pkstring PKMN_API get_move_description(const unsigned int move_id, const unsigned int version_id);
        pkmn::pkstring PKMN_API get_move_description(const pkmn::pkstring &move_name, const pkmn::pkstring &version_name);
        unsigned int PKMN_API get_move_pp(const unsigned int move_id);
        unsigned int PKMN_API get_move_pp(const pkmn::pkstring &move_name);

        unsigned int PKMN_API get_nature_id(const pkmn::pkstring &nature_name);
        pkmn::pkstring PKMN_API get_nature_name(const unsigned int nature_id);
        double PKMN_API get_nature_stat_mod(const unsigned int nature_id, const unsigned int stat_id);
        double PKMN_API get_nature_stat_mod(const pkmn::pkstring &nature_name, const pkmn::pkstring &stat_name);

        pkmn::pkstring PKMN_API get_pokedex_entry(const unsigned int species_id, const unsigned int version_id);
        pkmn::pkstring PKMN_API get_pokedex_entry(const pkmn::pkstring &species_name, const pkmn::pkstring &version_name);

        unsigned int PKMN_API get_pokemon_id(const unsigned int game_index, const unsigned int version_id);
        unsigned int PKMN_API get_pokemon_id(const unsigned int game_index, const pkmn::pkstring &version_name);
        unsigned int PKMN_API get_pokemon_id(const unsigned int form_id);
        unsigned int PKMN_API get_pokemon_game_index(const unsigned int pokemon_id, const unsigned int version_id);
        unsigned int PKMN_API get_pokemon_game_index(const unsigned int pokemon_id, const pkmn::pkstring &version_name);

        unsigned int PKMN_API get_species_id(const pkmn::pkstring &species_name);
        unsigned int PKMN_API get_species_id(const unsigned int pokemon_id);
        pkmn::pkstring PKMN_API get_species_name(const unsigned int species_id);

        unsigned int PKMN_API get_stat_id(const pkmn::pkstring &stat_name);
        pkmn::pkstring PKMN_API get_stat_name(const unsigned int stat_id);

        unsigned int PKMN_API get_type_id(const pkmn::pkstring &type_name);
        pkmn::pkstring PKMN_API get_type_name(const unsigned int type_id);

        unsigned int PKMN_API get_version_id(const pkmn::pkstring &version_name);
        pkmn::pkstring PKMN_API get_version_name(const unsigned int version_id);

        uint16_t PKMN_API get_version_game_index(const pkmn::pkstring& version_name);
        uint16_t PKMN_API get_version_game_index(uint16_t version_id);
        uint16_t PKMN_API get_version_id(uint16_t game_index);

        unsigned int PKMN_API get_version_group_id(const unsigned int version_id);
        unsigned int PKMN_API get_version_group_id(const pkmn::pkstring &version_name);
        pkmn::pkstring PKMN_API get_version_group_name(const unsigned int version_group_id);
    }
}

#endif /* INCLUDED_PKMN_DATABASE_HPP */
