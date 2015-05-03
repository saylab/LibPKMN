/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "exception.i"
%include "stdint.i"
%include "std_pair.i"
%include "CamelCase.i"

#if SWIG_VERSION < 0x030000
%include "pkmn_pkstring.i"
#else
%include "pkmn_pkstring3.i"
#endif

%import "pkmn_cs.i"

std::pair<int, int> getStatRange(pkmn::cs::PokemonEntry &entry,
                                 const pkmn::pkstring &game,
                                 const pkmn::pkstring &statName,
                                 int level);

bool isStatPossible(pkmn::cs::PokemonEntry &entry, const pkmn::pkstring &game,
                    const pkmn::pkstring &statName, int statValue,
                    int level);

%{
    #include "pkmn/calculations.hpp"
    #include "entry_wrappers.hpp"

    std::pair<int, int> getStatRange(pkmn::cs::PokemonEntry &entry,
                                     const pkmn::pkstring &game,
                                     const pkmn::pkstring &statName,
                                     int level){

        pkmn::pokemon_entry_t native_entry(game, entry.getSpeciesName(), entry.getForm());
        return pkmn::calculations::get_stat_range(native_entry, game,
                                                  statName, level);
    }

    bool isStatPossible(pkmn::cs::PokemonEntry &entry, const pkmn::pkstring &game,
                        const pkmn::pkstring &statName, int statValue,
                        int level){

        pkmn::pokemon_entry_t native_entry(game, entry.getSpeciesName(), entry.getForm());
        return pkmn::calculations::is_stat_possible(native_entry, game,
                                                    statName, statValue,
                                                    level);
    }
%}

%ignore pkmn::calculations::get_stat_range;
%ignore pkmn::calculations::is_stat_possible;

%include "pkmn/calculations.hpp"
