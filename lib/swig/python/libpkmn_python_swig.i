/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%feature("autodoc", "1");

%include "typemaps.i"

/*
 * An interface template needs to be created for each pkmn::shared_ptr used.
 *
 * To make Python development easier, the make function is rendered unnecessary.
 */

%define LIBPKMN_PYTHON_SPTR(CLASS_NAME)
    %template(CLASS_NAME ## _sptr) pkmn::shared_ptr<pkmn :: ## CLASS_NAME>;
    %pythoncode %{
        CLASS_NAME = CLASS_NAME.make;
    %}
%enddef

/*
 * Macro for pkmn::dict templates
 */
%define LIBPKMN_PYTHON_DICT(python_name, type1, type2)
    %extend pkmn::dict<type1, type2>{
        type2 __getitem__(type1 i) {return (*self)[i];}
        void __setitem__(type1 i, type2 val) {(*self)[i] = val;}
    }
    %template(python_name) pkmn::dict<type1, type2>;
%enddef

%extend pkmn::nature {
    double __getitem__(std::string key) {return (*self)[key];}
    double __getitem__(unsigned int key) {return (*self)[key];}
};

/* Enough to warrant its own file */
%include "python_pkstring.i"

%include "libpkmn.i"

%{
    #include "pkmn/build_info.hpp"

    #include "pkmn/types/dict.hpp"
    #include "pkmn/types/pkstring.hpp"
    #include "pkmn/types/prng.hpp"

    #include "pkmn/nature.hpp"
    #include "pkmn/base_pokemon.hpp"
    #include "pkmn/item.hpp"
    #include "pkmn/pocket.hpp"
    #include "pkmn/bag.hpp"
    #include "pkmn/move.hpp"
    #include "pkmn/team_pokemon.hpp"
    #include "pkmn/trainer.hpp"
    #include "pkmn/game_save.hpp"

    #include "pkmn/lists.hpp"
    #include "pkmn/paths.hpp"
%}

%include "pkmn/build_info.hpp"

%include "pkmn/types/dict.hpp"
%include "pkmn/types/pkstring.hpp"
%include "pkmn/types/prng.hpp"

%include "pkmn/nature.hpp"
%include "pkmn/base_pokemon.hpp"
%include "pkmn/item.hpp"
%include "pkmn/pocket.hpp"
%include "pkmn/bag.hpp"
%include "pkmn/move.hpp"
%include "pkmn/team_pokemon.hpp"
%include "pkmn/trainer.hpp"
%include "pkmn/game_save.hpp"

%include "pkmn/lists.hpp"
%include "pkmn/paths.hpp"

%template(bag_slot) std::pair<pkmn::item::sptr, unsigned int>;
%template(item_list) std::vector<std::pair<pkmn::item::sptr, unsigned int> >;

%template(uint_pair_vec) std::vector<std::pair<unsigned int, unsigned int> >;
%template(base_pokemon_vec) std::vector<pkmn::base_pokemon::sptr>;
%template(moveset) std::vector<pkmn::move::sptr>;
%template(pocket_vec) std::vector<pkmn::pocket::sptr>;
%template(pokemon_team) std::vector<pkmn::team_pokemon::sptr>;
%template(pkstring_vec) std::vector<pkmn::pkstring>;

LIBPKMN_PYTHON_DICT(string_int_dict, std::string, int)
LIBPKMN_PYTHON_DICT(string_string_dict, std::string, std::string)
LIBPKMN_PYTHON_DICT(string_uint_dict, std::string, unsigned int)
LIBPKMN_PYTHON_DICT(pocket_dict, std::string, pkmn::pocket::sptr)

LIBPKMN_PYTHON_SPTR(base_pokemon)
LIBPKMN_PYTHON_SPTR(item)
LIBPKMN_PYTHON_SPTR(pocket)
LIBPKMN_PYTHON_SPTR(bag)
LIBPKMN_PYTHON_SPTR(move)
LIBPKMN_PYTHON_SPTR(team_pokemon)
LIBPKMN_PYTHON_SPTR(trainer)
LIBPKMN_PYTHON_SPTR(game_save)
LIBPKMN_PYTHON_SPTR(prng)
