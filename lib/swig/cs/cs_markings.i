/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%typemap(cscode) pkmn::markings %{
    public static implicit operator byte(Markings input) {
        byte markint = 0;

        if(input.circle)   markint |= 0x01;
        if(input.triangle) markint |= 0x02;
        if(input.square)   markint |= 0x04;
        if(input.heart)    markint |= 0x08;
        if(input.star)     markint |= 0x10;
        if(input.diamond)  markint |= 0x20;

        return markint;
    }

    public static implicit operator Markings(byte markint) {
        Markings mark = new Markings();

        mark.circle =   (markint & 0x01) > 0;
        mark.triangle = (markint & 0x02) > 0;
        mark.square =   (markint & 0x04) > 0;
        mark.heart =    (markint & 0x08) > 0;
        mark.star =     (markint & 0x10) > 0;
        mark.diamond =  (markint & 0x20) > 0;

        return mark;
    }
%}

%{
    #include "pkmn/types/markings.hpp"
%}

%include "pkmn/types/markings.hpp"
