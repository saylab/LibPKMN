/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

/*
 * Macro for dict templates to avoid using partial classes for every type
 */
%define LIBPKMN_CS_DICT(dict_name, ctype1, ctype2, cstype1, cstype2)
    %typemap(cscode) pkmn::dict<ctype1, ctype2> %{
        /// <returns>
        /// Returns a vector (cstype1) containing the dictionary's keys.
        /// </returns>
        public cstype1 ## _vec Keys {
            get {
                return keys();
            }
        }

        /// <returns>
        /// Returns a vector (cstype2) containing the dictionary's values.
        /// </returns>
        public cstype2 ## _vec Values {
            get {
                return vals();
            }
        }

        /// <summary>
        /// The [] operator gets or sets dictionary entries.
        /// </summary>
        /// <returns>
        /// cstype2 whose position is given in [].
        /// </returns>
        public cstype2 this[cstype1 index] {
            get {
                return at(index);
            }
            set {
                Add(index, value);
            }
        }

        /// <summary>
        /// Allows a dict_name to be cast as a native C# Dictionary<cstype1, cstype2>.
        /// </summary>
        public static implicit operator System.Collections.Generic.Dictionary<cstype1, cstype2>(dict_name input) {
            System.Collections.Generic.Dictionary<cstype1, cstype2> output = new System.Collections.Generic.Dictionary<cstype1, cstype2>();
            cstype1 ## _vec keys = input.keys();
            cstype2 ## _vec vals = input.vals();

            for(int i = 0; i < keys.Count; i++) output.Add(keys[i], vals[i]);

            return output;
        }

        /// <summary>
        /// Allows a native C# Dictionary<cstype1, cstype2> to be cast as a dict_name.
        /// </summary>
        public static implicit operator dict_name(System.Collections.Generic.Dictionary<cstype1, cstype2> input) {
            dict_name output = new dict_name();
            foreach(System.Collections.Generic.KeyValuePair<cstype1, cstype2> pair in input) output.Add(pair.Key, pair.Value);
            return output;
        }
    %}

    %rename(Add) pkmn::dict<ctype1, ctype2>::insert;
    %rename(Clear) pkmn::dict<ctype1, ctype2>::clear;
    %rename(ContainsKey) pkmn::dict<ctype1, ctype2>::has_key;
    %rename(Remove) pkmn::dict<ctype1, ctype2>::erase;

    %template(dict_name) pkmn::dict<ctype1, ctype2>;
%enddef

%extend pkmn::nature {
    double at(std::string key) {return (*self)[key];}
    double at(unsigned int key) {return (*self)[key];}
};
%typemap(cscode) pkmn::nature %{
    public double this[string key] {
        get {
            return at(key);
        }
    }

    public double this[uint key] {
        get {
            return at(key);
        }
    }
%}

%include "CamelCase.i"
%include "cs_pkstring.i"
%include "libpkmn.i"

%include "cs_markings.i"

%{
    #include "pkmn/build_info.hpp"
    #include "pkmn/paths.hpp"

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

%}

%include "pkmn/build_info.hpp"
%include "pkmn/paths.hpp"

%include "pkmn/types/dict.hpp"
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

%template(BagSlot) std::pair<pkmn::item::sptr, unsigned int>;
%template(ItemList) std::vector<std::pair<pkmn::item::sptr, unsigned int> >;

%template(UIntPairVector) std::vector<std::pair<unsigned int, unsigned int> >;
%template(BasePokemonVector) std::vector<pkmn::base_pokemon::sptr>;
%template(Moveset) std::vector<pkmn::move::sptr>;
%template(PocketSPtr_vec) std::vector<pkmn::pocket::sptr>; //Temporary, necessary for dict for now
%template(PokemonTeam) std::vector<pkmn::team_pokemon::sptr>;
%template(string_vec) std::vector<pkmn::pkstring>;

LIBPKMN_CS_DICT(StringIntDict, pkmn::pkstring, int, string, int)
LIBPKMN_CS_DICT(StringStringDict, pkmn::pkstring, pkmn::pkstring, string, string)
LIBPKMN_CS_DICT(StringUIntDict, pkmn::pkstring, unsigned int, string, uint)
LIBPKMN_CS_DICT(PocketDict, pkmn::pkstring, pkmn::pocket::sptr, string, PocketSPtr)

//Factory functions need to specifically be associated with newobject
%newobject pkmn::bag::make;
%newobject pkmn::base_pokemon::make;
%newobject pkmn::game_save::make;
%newobject pkmn::item::make;
%newobject pkmn::move::make;
%newobject pkmn::pocket::make;
%newobject pkmn::prng::make;
%newobject pkmn::team_pokemon::make;
%newobject pkmn::team_pokemon::get_base_pokemon;
%newobject pkmn::trainer::make;

//TODO: make C++ -> C# class name conversion function
%template(BagSPtr)         pkmn::shared_ptr<pkmn::bag>;
%template(BasePokemonSPtr) pkmn::shared_ptr<pkmn::base_pokemon>;
%template(GameSaveSPtr)    pkmn::shared_ptr<pkmn::game_save>;
%template(ItemSPtr)        pkmn::shared_ptr<pkmn::item>;
%template(MoveSPtr)        pkmn::shared_ptr<pkmn::move>;
%template(PocketSPtr)      pkmn::shared_ptr<pkmn::pocket>;
%template(PRNGSPtr)        pkmn::shared_ptr<pkmn::prng>;
%template(TeamPokemonSPtr) pkmn::shared_ptr<pkmn::team_pokemon>;
%template(TrainerSPtr)     pkmn::shared_ptr<pkmn::trainer>;
