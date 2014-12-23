/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "pkmn_pkstring.i"

%extend pkmn::nature_t {
    float at(const pkmn::pkstring &key) {return (*self)[key];}
    float at(unsigned int key) {return (*self)[key];}
};
%typemap(cscode) pkmn::nature_t %{
    public double this[string key] {
        get {
            return at(key);
        }
    }

    public double this[uint key] {
        get {
            return at(key);
        }
    }
%}

%{
    #include "pkmn/types/nature.hpp"
%}

%include "pkmn/types/nature.hpp"
