/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_JAVA_LISTWRAPPER_HPP
#define INCLUDED_JAVA_LISTWRAPPER_HPP

#include <string>
#include <vector>

#include <boost/foreach.hpp>

#include <pkmn/config.hpp>
#include <pkmn/lists.hpp>

void PKMN_INLINE getGameList(std::vector<std::wstring> &game_vec)
{
    game_vec.clear();

    std::vector<pkmn::pkstring> game_pkvec;
    pkmn::get_game_list(game_pkvec);

    BOOST_FOREACH(const pkmn::pkstring &pk, game_pkvec)
    {
        game_vec.push_back(pk.std_wstring());
    }
}

void PKMN_INLINE getGameGroupList(std::vector<std::wstring> &game_group_vec)
{
    game_group_vec.clear();

    std::vector<pkmn::pkstring> game_group_pkvec;
    pkmn::get_game_group_list(game_group_pkvec);

    BOOST_FOREACH(const pkmn::pkstring &pk, game_group_pkvec)
    {
        game_group_vec.push_back(pk.std_wstring());
    }
}

void PKMN_INLINE getItemList(std::vector<std::wstring> &item_vec, int game)
{
    item_vec.clear();

    std::vector<pkmn::pkstring> item_pkvec;
    pkmn::get_item_list(item_pkvec, game);

    BOOST_FOREACH(const pkmn::pkstring &pk, item_pkvec)
    {
        item_vec.push_back(pk.std_wstring());
    }
}

//No wstring/pkstring conversion here, but ints are more convenient for Java
typedef std::pair<unsigned int, unsigned int> uint_pair;
void PKMN_INLINE getPokedexOrder(std::vector<std::pair<int, int> > &entry_vec, int pokedex_id)
{
    entry_vec.clear();

    std::vector<std::pair<unsigned int, unsigned int> > entry_uint_vec;
    pkmn::get_pokedex_order(entry_uint_vec, pokedex_id);
    BOOST_FOREACH(const uint_pair &uints, entry_uint_vec)
    {
        entry_vec.push_back(std::make_pair(int(uints.first), int(uints.second)));
    }
}

void PKMN_INLINE getPokemonList(std::vector<std::wstring> &pokemon_vec, int game)
{
    pokemon_vec.clear();

    std::vector<pkmn::pkstring> pokemon_pkvec;
    pkmn::get_pokemon_list(pokemon_pkvec, game);

    BOOST_FOREACH(const pkmn::pkstring &pk, pokemon_pkvec)
    {
        pokemon_vec.push_back(pk.std_wstring());
    }
}

void PKMN_INLINE getTypeList(std::vector<std::wstring> &type_vec, int game)
{
    type_vec.clear();

    std::vector<pkmn::pkstring> type_pkvec;
    pkmn::get_type_list(type_pkvec, game);

    BOOST_FOREACH(const pkmn::pkstring &pk, type_pkvec)
    {
        type_vec.push_back(pk.std_wstring());
    }
}

void PKMN_INLINE getAbilityList(std::vector<std::wstring> &ability_vec, int gen)
{
    ability_vec.clear();

    std::vector<pkmn::pkstring> ability_pkvec;
    pkmn::get_ability_list(ability_pkvec, gen);

    BOOST_FOREACH(const pkmn::pkstring &pk, ability_pkvec)
    {
        ability_vec.push_back(pk.std_wstring());
    }
}

void PKMN_INLINE getNatureList(std::vector<std::wstring> &nature_vec)
{
    nature_vec.clear();

    std::vector<pkmn::pkstring> nature_pkvec;
    pkmn::get_nature_list(nature_pkvec);

    BOOST_FOREACH(const pkmn::pkstring &pk, nature_pkvec)
    {
        nature_vec.push_back(pk.std_wstring());
    }
}

void PKMN_INLINE getPokemonOfType(pkmn::base_pokemon_vector &pkmn_vector, std::wstring type1, std::wstring type2, int gen, bool lax)
{
    pkmn::get_pokemon_of_type(pkmn_vector, pkmn::pkstring(type1), pkmn::pkstring(type2), (unsigned int)(gen), lax);
}

#endif /* INCLUDED_JAVA_LISTWRAPPER_HPP */
