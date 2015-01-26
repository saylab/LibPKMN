/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "exception.i"
%include "CamelCase.i"

#if SWIG_VERSION < 0x030000
%include "pkmn_pkstring.i"
#else
%include "pkmn_pkstring3.i"
#endif

%import "pkmn_cs.i"

void getPokemonOfType(std::vector<PokemonEntry>& PKMNVector, const pkmn::pkstring& type1,
                      const pkmn::pkstring& type2, uint16_t generation, bool lax);

%{
    #include <boost/foreach.hpp>

    #include "pkmn/enums.hpp"
    #include "pkmn/lists.hpp"
    #include "entry_wrappers.hpp"

    void getPokemonOfType(std::vector<PokemonEntry>& PKMNVector, const pkmn::pkstring& type1,
                          const pkmn::pkstring& type2, uint16_t generation, bool lax){

        pkmn::pkstring version_from_gen[] = {"Yellow","Crystal",
                                             "Fire Red","Heart Gold",
                                             "Black 2","X"};

        PKMNVector.clear();
        pkmn::pokemon_entry_vector_t pkmn_vector;
        get_pokemon_of_type(pkmn_vector, type1, type2, generation, lax);

        BOOST_FOREACH(const pkmn::pokemon_entry_t& entry, pkmn_vector){
            PKMNVector.push_back(PokemonEntry(version_from_gen[generation],
                                              entry.species_name,
                                              entry.form));
        }
    }
%}

%ignore pkmn::get_pokemon_of_type;

%include "pkmn/lists.hpp"
