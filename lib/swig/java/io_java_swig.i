/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "exception.i"
%include "CamelCase.i"
%import "libpkmn_java_swig.i"

%{
    #include "pkmn/io.hpp"
%}

%include "pkmn/io.hpp"
