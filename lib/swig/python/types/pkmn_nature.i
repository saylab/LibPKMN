/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%extend pkmn::nature_t {
    float __getitem__(const pkmn::pkstring &key) {return (*self)[key];}
    float __getitem__(unsigned int key) {return (*self)[key];}
};

%{
    #include "pkmn/types/nature.hpp"
%}

%include "pkmn/types/nature.hpp"
