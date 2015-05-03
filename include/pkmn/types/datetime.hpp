/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TYPES_DATETIME_HPP
#define INCLUDED_PKMN_TYPES_DATETIME_HPP

#include <cstdint>
#include <pkmn/config.hpp>
#include <pkmn/native/datetime.hpp>

namespace pkmn
{
    struct PKMN_API datetime_t
    {
        datetime_t();
        datetime_t(uint32_t time_since_2000);
        datetime_t(const native::gen1_time_t &gen1_time);
        datetime_t(const native::gen2_time_t &gen2_time);
        datetime_t(const native::gen3_time_t &gen3_time);
        datetime_t(const native::date_t      &date);

        int year;
        int month;
        int day;

        int hours;
        int minutes;
        int seconds;

        operator uint32_t();
        operator native::gen1_time_t();
        operator native::gen2_time_t();
        operator native::gen3_time_t();
        operator native::date_t();
    };
}

#endif /* INCLUDED_PKMN_TYPES_DATETIME_HPP */
