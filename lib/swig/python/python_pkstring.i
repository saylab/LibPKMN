/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "typemaps.i"

%typecheck(SWIG_TYPECHECK_STRING) pkmn::pkstring* %{
    if(PyString_Check($input)) $1 = 1;
    else
    {
        $1 = 0;
        PyErr_Clear();
    }
%}

%typecheck(SWIG_TYPECHECK_STRING) const pkmn::pkstring* %{
    if(PyString_Check($input)) $1 = 1;
    else
    {
        $1 = 0;
        PyErr_Clear();
    }
%}

%typecheck(SWIG_TYPECHECK_STRING) pkmn::pkstring& %{
    if(PyString_Check($input)) $1 = 1;
    else
    {
        $1 = 0;
        PyErr_Clear();
    }
%}

%typecheck(SWIG_TYPECHECK_STRING) const pkmn::pkstring& %{
    if(PyString_Check($input)) $1 = 1;
    else
    {
        $1 = 0;
        PyErr_Clear();
    }
%}

%typemap(in) pkmn::pkstring (std::string temp) %{
    temp = PyString_AsString($input);
    $1 = pkmn::pkstring(temp);
%}

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

%ignore pkmn::pkstring::const_char;
%rename(__eq__) pkmn::operator==(const pkmn::pkstring&, const pkmn::pkstring&);
%rename(__len__) pkmn::pkstring::length;
%extend pkmn::pkstring {
    char* __repr__() {return (char*)((*self).const_char());}
    char* __str__() {return (char*)((*self).const_char());}
};
