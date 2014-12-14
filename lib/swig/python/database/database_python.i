/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%feature("autodoc", "1");

%include "std_vector.i"
%import "pkmn_python.i"

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
    #include "pkmn/database.hpp"
%} 

%include "pkmn/database.hpp"