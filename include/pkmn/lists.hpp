/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_LISTS_HPP
#define INCLUDED_PKMN_LISTS_HPP

#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <pkmn/config.hpp>
#include <pkmn/pokedex/pokemon_entry.hpp>
#include <pkmn/types/pkstring.hpp>

namespace pkmn
{
    void PKMN_API get_version_list(std::vector<pkmn::pkstring>& game_vec);

    void PKMN_API get_version_group_list(std::vector<pkmn::pkstring>& game_group_vec);

    void PKMN_API get_item_list(std::vector<pkmn::pkstring>& item_vec, uint16_t game);

    void PKMN_API get_pokemon_list(std::vector<pkmn::pkstring>& pokemon_vec, uint16_t game);

    void PKMN_API get_type_list(std::vector<pkmn::pkstring>& type_vec, uint16_t gen);

    void PKMN_API get_ability_list(std::vector<pkmn::pkstring>& ability_vec, uint16_t gen);

    void PKMN_API get_nature_list(std::vector<pkmn::pkstring>& nature_vec);

    void PKMN_API get_pokemon_of_type(pokemon_entry_vector_t& pkmn_vector,
                                      const pkmn::pkstring& type1,
                                      const pkmn::pkstring& type2,
                                      uint16_t generation,
                                      bool lax);
}

#endif /* INCLUDED_PKMN_LISTS_HPP */
