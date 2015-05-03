/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

/*
 * This version of the pkmn::pkstring Python typemap is meant to be used with
 * Python 3.x.
 */

%{
    #include <boost/locale/encoding_utf.hpp>
%}

%typecheck(SWIG_TYPECHECK_STRING) pkmn::pkstring, const pkmn::pkstring& %{
    if(PyUnicode_Check($input)) $1 = 1;
    else
    {
        $1 = 0;
        PyErr_Clear();
    }
%}

/*
 * NOTE: PyUnicode_AS_UNICODE is deprecated and will be removed in Python 4.
 */

%typemap(in) pkmn::pkstring %{
    $1 = pkmn::pkstring(boost::locale::conv::utf_to_utf<uint16_t>(PyUnicode_AS_UNICODE($input)));
%}

%typemap(in) const pkmn::pkstring& %{
    $1 = new pkmn::pkstring(boost::locale::conv::utf_to_utf<uint16_t>(PyUnicode_AS_UNICODE($input)));
%}

%typemap(out) pkmn::pkstring (std::wstring temp) %{
    temp = $1;
    #if __SIZEOF_WCHAR_T__ == 4
    $result = PyUnicode_FromKindAndData(PyUnicode_4BYTE_KIND, temp.c_str(), $1.length());
    #else
    $result = PyUnicode_FromKindAndData(PyUnicode_2BYTE_KIND, temp.c_str(), $1.length());
    #endif
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
