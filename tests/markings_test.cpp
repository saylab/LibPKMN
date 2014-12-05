/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>

#include <boost/test/unit_test.hpp>

#include <pkmn/types/markings.hpp>

BOOST_AUTO_TEST_CASE(from_markings_test)
{
    pkmn::markings mark;
    mark.circle = true;
    mark.triangle = false;
    mark.square = true;
    mark.heart = false;
    mark.star = true;
    mark.diamond = false;

    uint8_t mark_int = mark;
    BOOST_CHECK_EQUAL(mark_int, 21); // 0b010101
}

BOOST_AUTO_TEST_CASE(to_markings_test)
{
    uint8_t mark_int = 21; // 0b010101
    pkmn::markings mark = mark_int;
    BOOST_CHECK(mark.circle);
    BOOST_CHECK(!mark.triangle);
    BOOST_CHECK(mark.square);
    BOOST_CHECK(!mark.heart);
    BOOST_CHECK(mark.star);
    BOOST_CHECK(!mark.diamond);
}
