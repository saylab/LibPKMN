/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#if SWIG_VERSION < 0x030000
%include "pkmn_pkstring.i"
#else
%include "pkmn_pkstring3.i"
#endif

%extend pkmn::nature_t {
    float at(const pkmn::pkstring &key) {return (*self)[key];}
    float at(unsigned int key) {return (*self)[key];}
};
%typemap(cscode) pkmn::nature_t %{
    public float this[string key] {
        get {
            return at(key);
        }
    }

    public float this[uint key] {
        get {
            return at(key);
        }
    }
%}

%{
    #include "pkmn/types/nature.hpp"
%}

%include "pkmn/types/nature.hpp"
