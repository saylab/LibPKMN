/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%feature("autodoc", "1");

%include "exception.i"

%{
    #include "pokemon.hpp"
    #include "text.hpp"
%} 

%import "pkmn_python.i"

%include "pokemon.hpp"
%include "text.hpp"
