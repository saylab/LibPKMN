/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

namespace pkmn
{

struct datetime_t {
    int year;
    int month;
    int day;

    int hours;
    int minutes;
    int seconds;
};

} 

%{
    #include "pkmn/types/datetime.hpp"
%}
