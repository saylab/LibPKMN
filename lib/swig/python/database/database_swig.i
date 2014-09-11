/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%feature("autodoc", "1");

%include "std_vector.i"
%import "libpkmn_python_swig.i"

%typemap(in) pkmn::pkstring* (std::string temp) %{
    temp = PyString_AsString($input);
    $1 = new pkmn::pkstring(temp);
%}

%typemap(in) const pkmn::pkstring* (std::string temp) %{
    temp = PyString_AsString($input);
    $1 = new pkmn::pkstring(temp);
%}

%typemap(in) pkmn::pkstring& (std::string temp) %{
    temp = PyString_AsString($input);
    $1 = new pkmn::pkstring(temp);
%}

%typemap(in) const pkmn::pkstring& (std::string temp) %{
    temp = PyString_AsString($input);
    $1 = new pkmn::pkstring(temp);
%}

%typemap(out) pkmn::pkstring %{
    $result = PyString_FromString($1.const_char());
%}

%{
    #include "pkmn/database/lists.hpp"
    #include "pkmn/database/queries.hpp"
%} 

%include "pkmn/database/lists.hpp"
%include "pkmn/database/queries.hpp"
