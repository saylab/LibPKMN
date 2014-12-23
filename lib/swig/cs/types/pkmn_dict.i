/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

/*
 * Macro for pkmn::dict templates
 */
%define CS_PKMN_DICT(dict_name, ctype1, ctype2, cstype1, cstype2, keyvec_name, valvec_name)
    %typemap(cscode) pkmn::dict<ctype1, ctype2> %{
        /// <returns>
        /// Returns a vector (cstype1) containing the dictionary's keys.
        /// </returns>
        public keyvec_name Keys {
            get {
                return keys();
            }
        }

        /// <returns>
        /// Returns a vector (cstype2) containing the dictionary's values.
        /// </returns>
        public valvec_name Values {
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
    %}

    %rename(Add) pkmn::dict<ctype1, ctype2>::insert;
    %rename(Clear) pkmn::dict<ctype1, ctype2>::clear;
    %rename(ContainsKey) pkmn::dict<ctype1, ctype2>::has_key;
    %rename(Remove) pkmn::dict<ctype1, ctype2>::erase;

    %template(dict_name) pkmn::dict<ctype1, ctype2>;
%enddef

%{
    #include "pkmn/types/dict.hpp"
%}

%include "pkmn/types/dict.hpp"
