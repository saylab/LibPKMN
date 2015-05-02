/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/date_time.hpp>

#include <pkmn/types/datetime.hpp>

namespace greg = boost::gregorian;
namespace pt   = boost::posix_time;

namespace pkmn
{
    // Default to now
    datetime_t::datetime_t()
    {
        pt::ptime now = pt::second_clock::local_time();

        year = now.date().year();
        month = now.date().month();
        day = now.date().day();
        hours = now.time_of_day().hours();
        minutes = now.time_of_day().minutes();
        seconds = now.time_of_day().seconds();
    }

    datetime_t::datetime_t(uint32_t time_since_2000)
    {
        pt::ptime _2k(greg::date(2000, 1, 1), pt::time_duration(0,0,0));
        pt::time_duration pt_time(0,0,time_since_2000);
        pt::ptime date = _2k + pt_time;

        year = date.date().year();
        month = date.date().month();
        day = date.date().day();
        hours = date.time_of_day().hours();
        minutes = date.time_of_day().minutes();
        seconds = date.time_of_day().seconds();
    }

    datetime_t::datetime_t(const native::gen1_time_t &gen1_time):
        year(0), month(0), day(0),
        hours(gen1_time.hours), minutes(gen1_time.minutes), seconds(gen1_time.seconds)
    {}

    datetime_t::datetime_t(const native::gen2_time_t &gen2_time):
        year(0), month(0), day(0),
        hours(gen2_time.hours), minutes(gen2_time.minutes), seconds(gen2_time.seconds)
    {}

    datetime_t::datetime_t(const native::gen3_time_t &gen3_time):
        year(0), month(0), day(0),
        hours(gen3_time.hours), minutes(gen3_time.minutes), seconds(gen3_time.seconds)
    {}

    datetime_t::datetime_t(const native::nds_time_t &nds_time):
        year(0), month(0), day(0),
        hours(nds_time.hours), minutes(nds_time.minutes), seconds(nds_time.seconds)
    {}

    datetime_t::operator uint32_t()
    {
        pt::ptime _2k(greg::date(2000, 1, 1), pt::time_duration(0,0,0));
        pt::ptime time(greg::date(year, month, day), pt::time_duration(hours, minutes, seconds));
        pt::time_duration duration = time - _2k;

        return duration.total_seconds();
    }

    datetime_t::operator native::gen1_time_t()
    {
        native::gen1_time_t gen1_time;
        gen1_time.hours   = hours;
        gen1_time.minutes = minutes;
        gen1_time.seconds = seconds;

        return gen1_time;
    }

    datetime_t::operator native::gen2_time_t()
    {
        native::gen2_time_t gen2_time;
        gen2_time.hours   = hours;
        gen2_time.minutes = minutes;
        gen2_time.seconds = seconds;
        gen2_time.frames  = 0;

        return gen2_time;
    }

    datetime_t::operator native::gen3_time_t()
    {
        native::gen3_time_t gen3_time;
        gen3_time.hours   = hours;
        gen3_time.minutes = minutes;
        gen3_time.seconds = seconds;
        gen3_time.frames  = 0;

        return gen3_time;
    }

    datetime_t::operator native::nds_time_t()
    {
        native::nds_time_t nds_time;
        nds_time.hours   = hours;
        nds_time.minutes = minutes;
        nds_time.seconds = seconds;

        return nds_time;
    }
} /* namespace pkmn */
