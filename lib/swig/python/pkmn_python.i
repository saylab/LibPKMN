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

%include "pkmn_python_docstrings.i"
%include "pkmn.i"

%include "pkmn_datetime.i"
%include "pkmn_dict.i"
%include "pkmn_nature.i"
%include "pkmn_pkstring.i"
%include "pkmn_pokemon.i"
%include "pkmn_shared_ptr.i"

%{
    #include "pkmn/bag.hpp"
    #include "pkmn/game_save.hpp"
    #include "pkmn/lists.hpp"
    #include "pkmn/paths.hpp"
    #include "pkmn/pocket.hpp"
    #include "pkmn/pokedex.hpp"
    #include "pkmn/pokemon.hpp"
    #include "pkmn/trainer.hpp"

    #include "pkmn/types/contest_stats.hpp"
    #include "pkmn/types/markings.hpp"
    #include "pkmn/types/pokerus.hpp"
    #include "pkmn/types/prng.hpp"
    #include "pkmn/types/ribbons.hpp"
    #include "pkmn/types/super_training_medals.hpp"
    #include "pkmn/pokedex/pokemon_entry.hpp"
    #include "pkmn/pokedex/move_entry.hpp"
    #include "pkmn/pokedex/item_entry.hpp"
%}

%include "pkmn/types/markings.hpp"
%include "pkmn/types/pokerus.hpp"
%include "pkmn/types/prng.hpp"
%include "pkmn/types/ribbons.hpp"
%include "pkmn/types/super_training_medals.hpp"
%include "pkmn/pokedex/pokemon_entry.hpp"
%include "pkmn/pokedex/move_entry.hpp"
%include "pkmn/pokedex/item_entry.hpp"

%include "pkmn/pocket.hpp"
%include "pkmn/bag.hpp"

%include "pkmn/pokedex.hpp"
%include "pkmn/pokemon.hpp"
%include "pkmn/trainer.hpp"
%include "pkmn/game_save.hpp"

%include "pkmn/lists.hpp"
%include "pkmn/paths.hpp"

/*
 * std::pair templates
 */
%template(uchar_pair)          std::pair<uint8_t, uint8_t>;
%template(ushort_pair)         std::pair<uint16_t, uint16_t>;
%template(string_pair)         std::pair<pkmn::pkstring, pkmn::pkstring>;

/*
 * std::vector templates
 */
%template(byte_vector)          std::vector<uint8_t>;
%template(int_vector)           std::vector<int>;
%template(ushort_vector)        std::vector<uint16_t>;
%template(item_list)            std::vector<pkmn::bag_slot_t>;
%template(moveset)              std::vector<pkmn::move_entry_t>;
%template(pocket_vector)        std::vector<pkmn::pocket::sptr>;
%template(pokemon_entry_vector) std::vector<pkmn::pokemon_entry_t>;
%template(pokemon_team)         std::vector<pkmn::pokemon::sptr>;
%template(string_vector)        std::vector<pkmn::pkstring>;

/*
 * pkmn::dict templates
 */
PYTHON_PKMN_DICT(string_int_dict,    pkmn::pkstring, int)
PYTHON_PKMN_DICT(string_ushort_dict, pkmn::pkstring, uint16_t)
PYTHON_PKMN_DICT(string_string_dict, pkmn::pkstring, pkmn::pkstring)
PYTHON_PKMN_DICT(pocket_dict,        pkmn::pkstring, pkmn::pocket::sptr)

/*
 * pkmn::shared_ptr templates
 */
PYTHON_PKMN_SPTR(bag)
PYTHON_PKMN_SPTR(game_save)
PYTHON_PKMN_SPTR(pocket)
PYTHON_PKMN_SPTR(pokedex)
PYTHON_PKMN_SPTR(pokemon)
PYTHON_PKMN_SPTR(prng)
PYTHON_PKMN_SPTR(trainer)
