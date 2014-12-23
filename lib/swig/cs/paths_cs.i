/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "exception.i"
%include "CamelCase.i"

#if SWIG_VERSION < 0x030000
%include "pkmn_pkstring.i"
#else
%include "pkmn_pkstring3.i"
#endif

%import "pkmn_cs.i"

%{
    #include "pkmn/paths.hpp"
%}

%include "pkmn/paths.hpp"
