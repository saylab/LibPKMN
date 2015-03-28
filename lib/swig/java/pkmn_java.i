/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
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
%include "pkmn_javadocs.i"

%include "stdint.i"
%include "std_pair.i"
%include "std_vector.i"

%include "pkmn.i"

%include "pkmn_dict.i"
%include "pkmn_pkstring.i"
%include "pkmn_nature.i"
%include "pkmn_pokedex.i"
%include "pkmn_pokerus.i"
%include "pkmn_shared_ptr.i"

%ignore pkmn::trainer::LIBPKMN_PUBLIC_ID;
%ignore pkmn::trainer::LIBPKMN_SECRET_ID;
%ignore pkmn::trainer::LIBPKMN_TRAINER_ID;

%{
    #include "pkmn/bag.hpp"
    #include "pkmn/game_save.hpp"
    #include "pkmn/pocket.hpp"
    #include "pkmn/pokemon.hpp"
    #include "pkmn/trainer.hpp"

    #include "pkmn/types/contest_stats.hpp"
    #include "pkmn/types/markings.hpp"
    #include "pkmn/types/pkstring.hpp"
    #include "pkmn/types/prng.hpp"
    #include "pkmn/types/ribbons.hpp"
    #include "pkmn/types/super_training_medals.hpp"
%}

%include "pkmn/types/markings.hpp"
%include "pkmn/types/prng.hpp"
%include "pkmn/types/ribbons.hpp"
%include "pkmn/types/super_training_medals.hpp"

%include "pkmn/pocket.hpp"
%include "pkmn/bag.hpp"

%include "pkmn/pokemon.hpp"
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
%template(UShortPair)        std::pair<uint16_t, uint16_t>;
%template(StringPair)        std::pair<pkmn::pkstring, pkmn::pkstring>;

/*
 * std::vector templates
 */
%template(ByteVector)         std::vector<uint8_t>;
%template(IntVector)          std::vector<int>;
%template(UShortVector)       std::vector<uint16_t>;
%template(PocketVector)       std::vector<pkmn::pocket::sptr>;
%template(PokemonTeam)        std::vector<pkmn::pokemon::sptr>;
%template(StringVector)       std::vector<pkmn::pkstring>;

/*
 * pkmn::dict templates
 */
JAVA_PKMN_DICT(StringIntDict, pkmn::pkstring, int, string, int)
JAVA_PKMN_DICT(StringUShortDict, pkmn::pkstring, uint16_t, string, int)
JAVA_PKMN_DICT(StringStringDict, pkmn::pkstring, pkmn::pkstring, string, string)
JAVA_PKMN_DICT(PocketDict, pkmn::pkstring, pkmn::pocket::sptr, string, PocketSPtr)

/*
 * pkmn::shared_ptr templates
 */
JAVA_PKMN_SPTR(bag,          BagSPtr)
JAVA_PKMN_SPTR(game_save,    GameSaveSPtr)
JAVA_PKMN_SPTR(pocket,       PocketSPtr)
JAVA_PKMN_SPTR(pokedex,      PokedexSPtr)
JAVA_PKMN_SPTR(pokemon,      PokemonSPtr)
JAVA_PKMN_SPTR(prng,         PRNGSPtr)
JAVA_PKMN_SPTR(trainer,      TrainerSPtr)
