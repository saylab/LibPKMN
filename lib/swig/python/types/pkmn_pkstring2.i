/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

/*
 * This version of the pkmn::pkstring Python typemap is meant to be used with
 * Python 2.x.
 */

%typecheck(SWIG_TYPECHECK_STRING) pkmn::pkstring, const pkmn::pkstring& %{
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

%typemap(in) const pkmn::pkstring& (std::string temp) %{
    temp = PyString_AsString($input);
    $1 = new pkmn::pkstring(temp);
%}

%typemap(out) pkmn::pkstring %{
    $result = PyString_FromString($1.const_char());
%}

%ignore pkmn::pkstring::const_char;
%rename(__len__) pkmn::pkstring::length;
%extend pkmn::pkstring {
    char* __repr__() {return (char*)((*self).const_char());}
    char* __str__() {return (char*)((*self).const_char());}

    %pythoncode %{
        def __eq__(self, rhs):
            if isinstance(rhs, (int, long, float, complex)):
                return False
            else:
                return (str(self) == str(rhs))

        def __ne__(self, rhs):
            if isinstance(rhs, (int, long, float, complex)):
                return True
            else:
                return (str(self) != str(rhs))
    %}
};

%{
    #include "pkmn/types/pkstring.hpp"
%}

%include "pkmn/types/pkstring.hpp"
