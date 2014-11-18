/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%typemap(cscode) pkmn::markings %{
    public static implicit operator byte(Markings input) {
        byte markint = 0;

        if(input.circle)   markint |= (1 << 7); 
        if(input.triangle) markint |= (1 << 6); 
        if(input.square)   markint |= (1 << 5); 
        if(input.heart)    markint |= (1 << 4); 
        if(input.star)     markint |= (1 << 3); 
        if(input.diamond)  markint |= (1 << 2); 

        return markint;
    }

    public static implicit operator Markings(byte markint) {
        Markings mark = new Markings();

        mark.circle =   ( (markint >> 7)      > 0) ? true : false;
        mark.triangle = (((markint >> 6) & 1) > 0) ? true : false;
        mark.square =   (((markint >> 5) & 1) > 0) ? true : false;
        mark.heart =    (((markint >> 4) & 1) > 0) ? true : false;
        mark.star =     (((markint >> 3) & 1) > 0) ? true : false;
        mark.diamond =  (((markint >> 2) & 1) > 0) ? true : false;

        return mark;
    }
%}

%{
    #include "pkmn/types/markings.hpp"
%}

%include "pkmn/types/markings.hpp"
