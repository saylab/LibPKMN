/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "CamelCase.i"

%include "stdint.i"
%include "std_pair.i"
%include "std_wstring.i"
%include "std_vector.i"
%include "stl.i"

%include "pkmn.i"

%include "pkmn_dict.i"
%include "pkmn_markings.i"

#if SWIG_VERSION < 0x030000
%include "pkmn_pkstring.i"
#else
%include "pkmn_pkstring3.i"
#endif

%include "pkmn_nature.i"
%include "pkmn_pokedex.i"
%include "pkmn_shared_ptr.i"

%{
    #include "pkmn/bag.hpp"
    #include "pkmn/game_save.hpp"
    #include "pkmn/pocket.hpp"
    #include "pkmn/pokemon.hpp"
    #include "pkmn/trainer.hpp"

    #include "pkmn/types/contest_stats.hpp"
    #include "pkmn/types/pkstring.hpp"
    #include "pkmn/types/pokerus.hpp"
    #include "pkmn/types/prng.hpp"
    #include "pkmn/types/ribbons.hpp"
    #include "pkmn/types/super_training_medals.hpp"
%}

%include "pkmn/types/pokerus.hpp"
%include "pkmn/types/prng.hpp"
%include "pkmn/types/ribbons.hpp"
%include "pkmn/types/super_training_medals.hpp"

%include "pkmn/pocket.hpp"
%include "pkmn/bag.hpp"

%include "pkmn/pokemon.hpp"
%include "pkmn/trainer.hpp"
%include "pkmn/game_save.hpp"

/*
 * std::pair templates
 */
%template(BytePair)     std::pair<uint8_t, uint8_t>;
%template(UShortPair)   std::pair<uint16_t, uint16_t>;
%template(UIntPair)     std::pair<unsigned int, unsigned int>;
%template(StringPair)   std::pair<std::wstring, std::wstring>;

/*
 * std::vector templates
 */
%template(ByteVector)   std::vector<uint8_t>;
%template(IntVector)    std::vector<int>;
%template(UShortVector) std::vector<uint16_t>;
%template(PocketVector) std::vector<pkmn::pocket::sptr>;
%template(PokemonTeam)  std::vector<pkmn::pokemon::sptr>;
%template(StringVector) std::vector<pkmn::pkstring>;

/*
 * pkmn::dict templates
 */
CS_PKMN_DICT(StringIntDict, pkmn::pkstring, int, string, int, StringVector, IntVector)
CS_PKMN_DICT(StringUShortDict, pkmn::pkstring, uint16_t, string, ushort, StringVector, UShortVector)
CS_PKMN_DICT(StringStringDict, pkmn::pkstring, pkmn::pkstring, string, string, StringVector, StringVector)
CS_PKMN_DICT(PocketDict, pkmn::pkstring, pkmn::pocket::sptr, string, PocketSPtr, StringVector, PocketVector)

/*
 * pkmn::shared_ptr templates
 */
CS_PKMN_SPTR(bag,       BagSPtr)
CS_PKMN_SPTR(game_save, GameSaveSPtr)
CS_PKMN_SPTR(pocket,    PocketSPtr)
CS_PKMN_SPTR(pokedex,   PokedexSPtr)
CS_PKMN_SPTR(pokemon,   PokemonSPtr)
CS_PKMN_SPTR(prng,      PRNGSPtr)
CS_PKMN_SPTR(trainer,   TrainerSPtr)
