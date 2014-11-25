/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/assign/list_of.hpp>
#include <boost/test/unit_test.hpp>

#include <pkmn/types/dict.hpp>

/*
 * Part of LibPKMN's constraints is that Boost cannot be publicly exposed,
 * so an alternative to BOOST_FOREACH is required. MSVC has a foreach method,
 * but it is not portable, so a custom #define is necessary. If this doesn't
 * work, then the Boostless pkmn::dict class will not work.
 */
BOOST_AUTO_TEST_CASE(has_key_test)
{
    pkmn::dict<int, int> d = boost::assign::map_list_of
        (0,-1)
        (2,5)
        (4,57)
    ;
    BOOST_CHECK(d.has_key(0));
    BOOST_CHECK(not d.has_key(3));
}

/*
 * Testing both at() functions
 */
BOOST_AUTO_TEST_CASE(at_test)
{
    pkmn::dict<int, int> d = boost::assign::map_list_of
        (0,-1)
        (2,5)
        (4,57)
    ;
    BOOST_CHECK_EQUAL(d.at(2), 5);
    BOOST_CHECK_EQUAL(d.at(3,10), 10);
}

/*
 * Test getting and setting with [] operator
 */
BOOST_AUTO_TEST_CASE(operator_test)
{
    pkmn::dict<int, int> d;

    // Test set operator
    d[5] = 10;
    BOOST_CHECK_EQUAL(d.at(5,5), 10);

    // Test get operator
    BOOST_CHECK_EQUAL(d[5], 10);
}

/*
 * Testing whether swap() swaps
 */
BOOST_AUTO_TEST_CASE(swap_test)
{
    pkmn::dict<int, int> d = boost::assign::map_list_of
        (0,-1)
        (2,5)
        (4,57)
    ;
    d.swap(0,4);
    BOOST_CHECK_EQUAL(d[0], 57);
    BOOST_CHECK_EQUAL(d[4], -1);
}

/*
 * Test whether erase() erases
 * Test whether empty() properly checks emptiness.
 */
BOOST_AUTO_TEST_CASE(erase_empty_test)
{
    pkmn::dict<int, int> d;
    BOOST_CHECK(d.empty());
    d[5] = 10;
    BOOST_CHECK(not d.empty());
    d.clear();
    BOOST_CHECK(d.empty());
}

/*
 * Test whether keys() and vals() work properly
 */
BOOST_AUTO_TEST_CASE(keys_vals_test)
{
    pkmn::dict<int, int> d = boost::assign::map_list_of
        (0,-1)
        (2,5)
        (4,57)
    ;

    std::vector<int> keys = d.keys();
    BOOST_CHECK_EQUAL(keys.size(), 3);
    BOOST_CHECK_EQUAL(keys[0], 0);
    BOOST_CHECK_EQUAL(keys[1], 2);
    BOOST_CHECK_EQUAL(keys[2], 4);

    std::vector<int> vals = d.vals();
    BOOST_CHECK_EQUAL(vals.size(), 3);
    BOOST_CHECK_EQUAL(vals[0], -1);
    BOOST_CHECK_EQUAL(vals[1], 5);
    BOOST_CHECK_EQUAL(vals[2], 57);
}