/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

/*
 * This workaround is necessary because of a bug in SWIG in which typemaps
 * are not properly propogated into std::pair templates. Supposedly, this
 * bug is only present in versions 2.0.5-3.0.0, but I have encountered it
 * in 3.0.1, so this workaround is necessary.
 *
 * At least for now, the strings used in pairs have no Unicode, so there
 * should be no problem in using std::string, since std::wstring has its
 * own set of problems.
 */

%extend pkmn::base_pokemon{
    std::pair<std::string, std::string> getAbilities(){
        pkmn::pkstring_pair_t abilities = self->get_abilities();

        std::pair<std::string, std::string> abilities2;
        abilities2.first = abilities.first;
        abilities2.second = abilities.second;

        return abilities2;
    }

    std::pair<std::string, std::string> getTypes(){
        pkmn::pkstring_pair_t types = self->get_types();

        std::pair<std::string, std::string> types2;
        types2.first = types.first;
        types2.second = types.second;

        return types2;
    }
}

%extend pkmn::team_pokemon{
    std::pair<std::string, std::string> getTypes(){
        pkmn::pkstring_pair_t types = self->get_types();

        std::pair<std::string, std::string> types2;
        types2.first = types.first;
        types2.second = types.second;

        return types2;
    }
}

%ignore pkmn::base_pokemon::get_abilities;
%ignore pkmn::base_pokemon::get_types;
%ignore pkmn::team_pokemon::get_types;
