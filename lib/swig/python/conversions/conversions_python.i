/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%feature("autodoc", "1");

%include "exception.i"
%include "ignores.i"

%{
    #include "pkmn/conversions/pokemon.hpp"
    #include "pokemon.hpp"
%} 

%import "pkmn_python.i"

%include "pkmn/conversions/pokemon.hpp"
%include "pokemon.hpp"
