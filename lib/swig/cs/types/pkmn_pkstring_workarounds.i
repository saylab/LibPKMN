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
 * This bug is theoretically only present in versions 2.0.5-3.0.0, but I
 * have encountered it in 3.0.1, so this workaround is necessary.
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
%ignore pkmn::team_pokemon::get_types;
