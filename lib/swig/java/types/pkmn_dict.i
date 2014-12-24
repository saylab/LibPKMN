/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

/*
 * Macro for pkmn::dict templates
 */
%define JAVA_PKMN_DICT(dict_name, ctype1, ctype2, javatype1, javatype2)
    %rename(get) pkmn::dict<ctype1, ctype2>::at;
    %rename(put) pkmn::dict<ctype1, ctype2>::insert;
    %rename(containsKey) pkmn::dict<ctype1, ctype2>::has_key;
    %rename(remove) pkmn::dict<ctype1, ctype2>::erase;
    %rename(KeySet) pkmn::dict<ctype1, ctype2>::keys;
    %rename(values) pkmn::dict<ctype1, ctype2>::vals;

    %template(dict_name) pkmn::dict<ctype1, ctype2>;
%enddef

%{
    #include "pkmn/types/dict.hpp"
%}

%include "pkmn/types/dict.hpp"
