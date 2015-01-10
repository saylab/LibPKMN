/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%feature("autodoc", "1");

%include "stdint.i"
%include "std_pair.i"
%include "std_vector.i"

%include "pkmn.i"

%include "pkmn_dict.i"
%include "pkmn_nature.i"
%include "pkmn_pkstring.i"
%include "pkmn_shared_ptr.i"

%{
    #include "pkmn/bag.hpp"
    #include "pkmn/base_pokemon.hpp"
    #include "pkmn/game_save.hpp"
    #include "pkmn/lists.hpp"
    #include "pkmn/move.hpp"
    #include "pkmn/paths.hpp"
    #include "pkmn/pocket.hpp"
    #include "pkmn/pokedex.hpp"
    #include "pkmn/team_pokemon.hpp"
    #include "pkmn/trainer.hpp"

    #include "pkmn/types/markings.hpp"
    #include "pkmn/types/prng.hpp"
    #include "pkmn/pokedex/pokemon_entry.hpp"
    #include "pkmn/pokedex/move_entry.hpp"
%}

%include "pkmn/types/markings.hpp"
%include "pkmn/types/prng.hpp"
%include "pkmn/pokedex/pokemon_entry.hpp"
%include "pkmn/pokedex/move_entry.hpp"

%include "pkmn/item.hpp"
%include "pkmn/pocket.hpp"
%include "pkmn/bag.hpp"

%include "pkmn/pokedex.hpp"
%include "pkmn/move.hpp"
%include "pkmn/base_pokemon.hpp"
%include "pkmn/team_pokemon.hpp"
%include "pkmn/trainer.hpp"
%include "pkmn/game_save.hpp"

%include "pkmn/lists.hpp"
%include "pkmn/paths.hpp"

/*
 * std::pair templates
 */
%template(uchar_pair)          std::pair<uint8_t, uint8_t>;
%template(uint_pair)           std::pair<unsigned int, unsigned int>;
%template(bag_slot)            std::pair<pkmn::item::sptr, unsigned int>;
%template(string_pair)         std::pair<pkmn::pkstring, pkmn::pkstring>;

/*
 * std::vector templates
 */
%template(int_vector)          std::vector<int>;
%template(ushort_vector)       std::vector<uint16_t>;
%template(uint_vector)         std::vector<unsigned int>;
%template(item_list)           std::vector<std::pair<pkmn::item::sptr, unsigned int> >;
%template(uint_pair_vector)    std::vector<std::pair<unsigned int, unsigned int> >;
%template(base_pokemon_vector) std::vector<pkmn::base_pokemon::sptr>;
%template(moveset)             std::vector<pkmn::move::sptr>;
%template(pocket_vector)       std::vector<pkmn::pocket::sptr>;
%template(pokemon_team)        std::vector<pkmn::team_pokemon::sptr>;
%template(string_vector)       std::vector<pkmn::pkstring>;

/*
 * pkmn::dict templates
 */
PYTHON_PKMN_DICT(string_int_dict,    pkmn::pkstring, int)
PYTHON_PKMN_DICT(string_ushort_dict, pkmn::pkstring, uint16_t)
PYTHON_PKMN_DICT(string_uint_dict,   pkmn::pkstring, unsigned int)
PYTHON_PKMN_DICT(string_string_dict, pkmn::pkstring, pkmn::pkstring)
PYTHON_PKMN_DICT(pocket_dict,        pkmn::pkstring, pkmn::pocket::sptr)

/*
 * pkmn::shared_ptr templates
 */
PYTHON_PKMN_SPTR(bag)
PYTHON_PKMN_SPTR(base_pokemon)
PYTHON_PKMN_SPTR(game_save)
PYTHON_PKMN_SPTR(item)
PYTHON_PKMN_SPTR(move)
PYTHON_PKMN_SPTR(pocket)
PYTHON_PKMN_SPTR(pokedex)
PYTHON_PKMN_SPTR(prng)
PYTHON_PKMN_SPTR(team_pokemon)
PYTHON_PKMN_SPTR(trainer)
