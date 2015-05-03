/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/test/unit_test.hpp>

#include <pkmn/types/datetime.hpp>

BOOST_AUTO_TEST_CASE(uint32_cast_test)
{
    /*
     * 500000 seconds = 5 days, 18 hours, 53 minutes, 20 seconds
     * 2000-01-06, 18:53:20
     */
    uint32_t seconds1 = 500000;
    pkmn::datetime_t date = seconds1;

    BOOST_CHECK_EQUAL(date.year,    2000);
    BOOST_CHECK_EQUAL(date.month,   1);
    BOOST_CHECK_EQUAL(date.day,     6);
    BOOST_CHECK_EQUAL(date.hours,   18);
    BOOST_CHECK_EQUAL(date.minutes, 53);
    BOOST_CHECK_EQUAL(date.seconds, 20);

    uint32_t seconds2 = date;
    BOOST_CHECK_EQUAL(seconds1, seconds2);
}

BOOST_AUTO_TEST_CASE(native_gen1_time_test)
{
    pkmn::native::gen1_time_t gen1_time1;
    gen1_time1.hours   = 1351;
    gen1_time1.minutes = 38;
    gen1_time1.seconds = 50;
    pkmn::datetime_t date = gen1_time1;

    BOOST_CHECK_EQUAL(date.hours,   gen1_time1.hours);
    BOOST_CHECK_EQUAL(date.minutes, gen1_time1.minutes);
    BOOST_CHECK_EQUAL(date.seconds, gen1_time1.seconds);

    pkmn::native::gen1_time_t gen1_time2 = date;
    BOOST_CHECK_EQUAL(gen1_time1.hours,   gen1_time2.hours);
    BOOST_CHECK_EQUAL(gen1_time1.minutes, gen1_time2.minutes);
    BOOST_CHECK_EQUAL(gen1_time1.seconds, gen1_time2.seconds);
}

BOOST_AUTO_TEST_CASE(native_gen2_time_test)
{
    pkmn::native::gen2_time_t gen2_time1;
    gen2_time1.hours   = 254;
    gen2_time1.minutes = 8;
    gen2_time1.seconds = 50;
    gen2_time1.frames  = 0;
    pkmn::datetime_t date = gen2_time1;

    BOOST_CHECK_EQUAL(date.hours,   gen2_time1.hours);
    BOOST_CHECK_EQUAL(date.minutes, gen2_time1.minutes);
    BOOST_CHECK_EQUAL(date.seconds, gen2_time1.seconds);

    pkmn::native::gen2_time_t gen2_time2 = date;
    BOOST_CHECK_EQUAL(gen2_time1.hours,   gen2_time2.hours);
    BOOST_CHECK_EQUAL(gen2_time1.minutes, gen2_time2.minutes);
    BOOST_CHECK_EQUAL(gen2_time1.seconds, gen2_time2.seconds);
    BOOST_CHECK_EQUAL(gen2_time1.frames,  gen2_time2.frames);
}

BOOST_AUTO_TEST_CASE(native_gen3_time_test)
{
    pkmn::native::gen3_time_t gen3_time1;
    gen3_time1.hours   = 254;
    gen3_time1.minutes = 38;
    gen3_time1.seconds = 50;
    gen3_time1.frames  = 0;
    pkmn::datetime_t date = gen3_time1;

    BOOST_CHECK_EQUAL(date.hours,   gen3_time1.hours);
    BOOST_CHECK_EQUAL(date.minutes, gen3_time1.minutes);
    BOOST_CHECK_EQUAL(date.seconds, gen3_time1.seconds);

    pkmn::native::gen3_time_t gen3_time2 = date;
    BOOST_CHECK_EQUAL(gen3_time1.hours,   gen3_time2.hours);
    BOOST_CHECK_EQUAL(gen3_time1.minutes, gen3_time2.minutes);
    BOOST_CHECK_EQUAL(gen3_time1.seconds, gen3_time2.seconds);
    BOOST_CHECK_EQUAL(gen3_time1.frames, gen3_time2.frames);
}

BOOST_AUTO_TEST_CASE(native_date_test)
{
    pkmn::native::date_t native_date1;
    native_date1.year  = 9; // 2009
    native_date1.month = 2;
    native_date1.day   = 11;
    pkmn::datetime_t date = native_date1;

    BOOST_CHECK_EQUAL(date.year,  (2000+native_date1.year));
    BOOST_CHECK_EQUAL(date.month, native_date1.month);
    BOOST_CHECK_EQUAL(date.day,   native_date1.day);

    pkmn::native::date_t native_date2 = date;
    BOOST_CHECK_EQUAL(native_date1.year,  native_date2.year);
    BOOST_CHECK_EQUAL(native_date1.month, native_date2.month);
    BOOST_CHECK_EQUAL(native_date1.day,   native_date2.day);
}
