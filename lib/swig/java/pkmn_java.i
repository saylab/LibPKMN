/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("pkmn_java");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Failed to load pkmn_java with the following error:\n" + e); 
      System.exit(1);
    }   
  }
%}

%include "CamelCase.i"

%include "pkmn.i"

%include "pkmn_dict.i"
%include "pkmn_pkstring.i"
%include "pkmn_nature.i"
%include "pkmn_shared_ptr.i"

%include "stdint.i"
%include "std_pair.i"
%include "std_vector.i"

%{
    #include "pkmn/bag.hpp"
    #include "pkmn/base_pokemon.hpp"
    #include "pkmn/game_save.hpp"
    #include "pkmn/move.hpp"
    #include "pkmn/pocket.hpp"
    #include "pkmn/team_pokemon.hpp"
    #include "pkmn/trainer.hpp"

    #include "pkmn/types/markings.hpp"
    #include "pkmn/types/pkstring.hpp"
    #include "pkmn/types/prng.hpp"
%}

%include "pkmn/types/markings.hpp"
%include "pkmn/types/prng.hpp"

%include "pkmn/item.hpp"
%include "pkmn/pocket.hpp"
%include "pkmn/bag.hpp"

%include "pkmn/move.hpp"
%include "pkmn/base_pokemon.hpp"
%include "pkmn/team_pokemon.hpp"
%include "pkmn/trainer.hpp"
%include "pkmn/game_save.hpp"

namespace pkmn
{
    class pkstring;
}

/*
 * std::pair templates
 */
%template(BytePair)          std::pair<uint8_t, uint8_t>;
%template(LongPair)          std::pair<unsigned int, unsigned int>;
%template(BagSlot)           std::pair<pkmn::item::sptr, unsigned int>;
%template(StringPair)        std::pair<pkmn::pkstring, pkmn::pkstring>;

/*
 * std::vector templates
 */
%template(IntVector)         std::vector<int>;
%template(LongVector)        std::vector<unsigned int>;
%template(ItemList)          std::vector<std::pair<pkmn::item::sptr, unsigned int> >;
%template(LongPairVector)    std::vector<std::pair<unsigned int, unsigned int> >;
%template(BasePokemonVector) std::vector<pkmn::base_pokemon::sptr>;
%template(Moveset)           std::vector<pkmn::move::sptr>;
%template(PocketVector)      std::vector<pkmn::pocket::sptr>;
%template(PokemonTeam)       std::vector<pkmn::team_pokemon::sptr>;
%template(StringVector)      std::vector<pkmn::pkstring>;

/*
 * pkmn::dict templates
 */
JAVA_PKMN_DICT(StringIntDict, pkmn::pkstring, int, string, int)
JAVA_PKMN_DICT(StringLongDict, pkmn::pkstring, unsigned int, string, long)
JAVA_PKMN_DICT(StringStringDict, pkmn::pkstring, pkmn::pkstring, string, string)
JAVA_PKMN_DICT(PocketDict, pkmn::pkstring, pkmn::pocket::sptr, string, PocketSPtr)

/*
 * pkmn::shared_ptr templates
 */
JAVA_PKMN_SPTR(bag,          BagSPtr)
JAVA_PKMN_SPTR(base_pokemon, BasePokemonSPtr)
JAVA_PKMN_SPTR(game_save,    GameSaveSPtr)
JAVA_PKMN_SPTR(item,         ItemSPtr)
JAVA_PKMN_SPTR(move,         MoveSPtr)
JAVA_PKMN_SPTR(pocket,       PocketSPtr)
JAVA_PKMN_SPTR(team_pokemon, TeamPokemonSPtr)
JAVA_PKMN_SPTR(prng,         PRNGSPtr)
JAVA_PKMN_SPTR(trainer,      TrainerSPtr)
