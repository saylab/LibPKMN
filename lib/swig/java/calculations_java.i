/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("calculations_java");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Failed to load calculations_java with the following error:\n" + e);
      System.exit(1);
    }
  }
%}

%include "exception.i"
%include "stdint.i"
%include "CamelCase.i"

%import "pkmn_java.i"

std::pair<int, int> getStatRange(pkmn::java::PokemonEntry &entry,
                                 const pkmn::pkstring &game,
                                 const pkmn::pkstring &statName,
                                 int level);

bool isStatPossible(pkmn::java::PokemonEntry &entry, const pkmn::pkstring &game,
                    const pkmn::pkstring &statName, int statValue,
                    int level);

%{
    #include "pkmn/calculations.hpp"
    #include "entry_wrappers.hpp"

    std::pair<int, int> getStatRange(pkmn::java::PokemonEntry &entry,
                                     const pkmn::pkstring &game,
                                     const pkmn::pkstring &statName,
                                     int level){

        pkmn::pokemon_entry_t native_entry(game, entry.getSpeciesName(), entry.getForm());
        return pkmn::calculations::get_stat_range(native_entry, game,
                                                  statName, level);
    }   

    bool isStatPossible(pkmn::java::PokemonEntry &entry, const pkmn::pkstring &game,
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

%{
    #include "pkmn/calculations.hpp"
%}

%include "pkmn/calculations.hpp"
