/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

/*
 * Macro for pkmn::dict templates
 */
%define PYTHON_PKMN_DICT(python_name, type1, type2)
    %extend pkmn::dict<type1, type2>{
        type2 __getitem__(type1 i) {return (*self)[i];}
        void __setitem__(type1 i, type2 val) {(*self)[i] = val;}
    }
    %template(python_name) pkmn::dict<type1, type2>;
%enddef

%{
    #include "pkmn/types/dict.hpp"
%}

%include "pkmn/types/dict.hpp"
