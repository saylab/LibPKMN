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

    rng->seed_arng(1234567);
    rng->arng();
    uint32_t arng_val = rng->arng();

    rng->seed_arng(1234567);
    rng->arng();
    BOOST_CHECK_EQUAL(rng->arng(), arng_val);
}

/*
 * Make sure the LCRNG returns the same result from the same seed.
 */
BOOST_AUTO_TEST_CASE(lcrng_test)
{
    pkmn::prng::sptr rng = pkmn::prng::make(4);

    rng->seed_lcrng(1234567);
    rng->lcrng();
    uint16_t lcrng_val = rng->lcrng();

    rng->seed_lcrng(1234567);
    rng->lcrng();
    BOOST_CHECK_EQUAL(rng->lcrng(), lcrng_val);
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
