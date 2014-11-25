/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/test/unit_test.hpp>

#include <pkmn/types/prng.hpp>

/*
 * Make sure the ARNG returns the same result from the same seed.
 */
BOOST_AUTO_TEST_CASE(arng_test)
{
    pkmn::prng::sptr rng = pkmn::prng::make(3);

    uint32_t arng_val1 = rng->arng();
    rng->arng();
    uint32_t arng_val2 = rng->arng();

    rng->seed_arng(arng_val1);
    rng->arng();
    BOOST_CHECK_EQUAL(rng->arng(), arng_val2);
}

/*
 * Make sure Generation IV LCRNG values are 32-bit numbers.
 * Make sure the LCRNG returns the same result from the same seed.
 */
BOOST_AUTO_TEST_CASE(lcrng_test)
{
    pkmn::prng::sptr rng4 = pkmn::prng::make(4);
    pkmn::prng::sptr rng5 = pkmn::prng::make(5);

    uint64_t lcrng4_val1 = rng4->lcrng();
    uint64_t lcrng5_val1 = rng5->lcrng();

    // Make sure the Gen IV LCRNG seed is a valid 32-bit value
    BOOST_CHECK(lcrng4_val1 < 4294967295);

    rng4->lcrng();
    rng5->lcrng();

    uint64_t lcrng4_val2 = rng4->lcrng();
    uint64_t lcrng5_val2 = rng5->lcrng();

    // Run the check again
    BOOST_CHECK(lcrng4_val2 < 4294967295);

    rng4->seed_lcrng(lcrng4_val1);
    rng5->seed_lcrng(lcrng5_val1);

    rng4->lcrng();
    rng5->lcrng();

    // Make sure PRNG recreates value with same seed
    BOOST_CHECK_EQUAL(rng4->lcrng(), lcrng4_val2);
    BOOST_CHECK_EQUAL(rng5->lcrng(), lcrng5_val2);
}

/*
 * Make sure the MTRNG array resets when it should.
 */
BOOST_AUTO_TEST_CASE(mtrng_test)
{
    pkmn::prng::sptr rng = pkmn::prng::make(3);

    uint32_t mtrng_val = rng->mtrng();
    for(size_t i = 0; i < 623; i++) rng->mtrng();
    BOOST_CHECK(rng->mtrng() != mtrng_val);
}