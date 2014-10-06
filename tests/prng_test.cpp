/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/test/unit_test.hpp>

#include <pkmn/types/prng.hpp>

BOOST_AUTO_TEST_CASE(check_lcrng)
{
    pkmn::prng::sptr rng4 = pkmn::prng::make(4);
    pkmn::prng::sptr rng5 = pkmn::prng::make(5);

    uint64_t lcrng4_val1 = rng4->lcrng();
    uint64_t lcrng5_val1 = rng5->lcrng();

    //Make sure the Gen IV LCRNG seed is a valid 32-bit value
    BOOST_CHECK(lcrng4_val1 < 4294967295);

    rng4->lcrng();
    rng5->lcrng();

    uint64_t lcrng4_val2 = rng4->lcrng();
    uint64_t lcrng5_val2 = rng5->lcrng();

    //Run the check again
    BOOST_CHECK(lcrng4_val2 < 4294967295);

    rng4->seed_lcrng(lcrng4_val1);
    rng5->seed_lcrng(lcrng5_val1);

    rng4->lcrng();
    rng5->lcrng();

    //Make sure PRNG recreates value with same seed
    BOOST_CHECK_EQUAL(rng4->lcrng(), lcrng4_val2);
    BOOST_CHECK_EQUAL(rng5->lcrng(), lcrng5_val2);
}
