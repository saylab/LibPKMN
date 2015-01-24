/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_LISTS_HPP
#define INCLUDED_PKMN_LISTS_HPP

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <pkmn/config.hpp>
#include <pkmn/types/pkstring.hpp>

namespace pkmn
{
    void PKMN_API get_version_list(std::vector<pkmn::pkstring> &game_vec);

    void PKMN_API get_version_group_list(std::vector<pkmn::pkstring> &game_group_vec);

    void PKMN_API get_item_list(std::vector<pkmn::pkstring> &item_vec, unsigned int game);

    void PKMN_API get_pokedex_order(std::vector<std::pair<unsigned int, unsigned int> > &entry_list, unsigned int pokedex_id);

    void PKMN_API get_pokemon_list(std::vector<pkmn::pkstring> &pokemon_vec, unsigned int game);

    void PKMN_API get_type_list(std::vector<pkmn::pkstring> &type_vec, unsigned int gen);

    void PKMN_API get_ability_list(std::vector<pkmn::pkstring> &ability_vec, unsigned int gen);

    void PKMN_API get_nature_list(std::vector<pkmn::pkstring> &nature_vec);

    //void PKMN_API get_pokemon_of_type(base_pokemon_vector &pkmn_vector, pkmn::pkstring type1, pkmn::pkstring type2, unsigned int gen, bool lax);

}

#endif /* INCLUDED_PKMN_LISTS_HPP */
