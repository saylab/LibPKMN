/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "stdint.i"
%include "exception.i"
%include "CamelCase.i"

%include "pkmn_pkstring.i"

%import "pkmn_cs.i"

%{
    #include "pkmn/database.hpp"
%}

%include "pkmn/database.hpp"
