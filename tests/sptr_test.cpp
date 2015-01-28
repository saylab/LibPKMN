/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include <pkmn/bag.hpp>
#include <pkmn/pocket.hpp>
#include <pkmn/pokedex.hpp>
#include <pkmn/pokemon.hpp>
#include <pkmn/trainer.hpp>
#include <pkmn/types/prng.hpp>

#include "copy_sptrs.hpp"

/*
 * Copying a bag::sptr involves copying a pocket::sptr as well, so make sure
 * only the pointers that are supposed to be equal are equal.
 */
BOOST_AUTO_TEST_CASE(copy_bag_pocket_test)
{
    pkmn::bag::sptr bag1 = pkmn::bag::make("Gold");
    pkmn::dict<pkmn::pkstring, pkmn::pocket::sptr> bag1_pockets = bag1->get_pockets();

    pkmn::bag::sptr bag2 = bag1;
    pkmn::dict<pkmn::pkstring, pkmn::pocket::sptr> bag2_pockets = bag2->get_pockets();

    pkmn::bag::sptr bag3 = pkmn::copy_bag(bag1);
    pkmn::dict<pkmn::pkstring, pkmn::pocket::sptr> bag3_pockets = bag3->get_pockets();

    // These should all match
    BOOST_CHECK_EQUAL(bag1, bag2);
    BOOST_CHECK_EQUAL(bag1_pockets["Items"], bag2_pockets["Items"]);
    BOOST_CHECK_EQUAL(bag1_pockets["Balls"], bag2_pockets["Balls"]);
    BOOST_CHECK_EQUAL(bag1_pockets["KeyItems"], bag2_pockets["KeyItems"]);
    BOOST_CHECK_EQUAL(bag1_pockets["TM/HM"], bag2_pockets["TM/HM"]);

    // These should all be different
    BOOST_CHECK(bag1 != bag3);
    BOOST_CHECK(bag1_pockets["Items"] != bag3_pockets["Items"]);
    BOOST_CHECK(bag1_pockets["Balls"] != bag3_pockets["Balls"]);
    BOOST_CHECK(bag1_pockets["KeyItems"] != bag3_pockets["KeyItems"]);
    BOOST_CHECK(bag1_pockets["TM/HM"] != bag3_pockets["TM/HM"]);
}

/*
 * Make sure pokedex::sptrs copy correctly.
 */
BOOST_AUTO_TEST_CASE(copy_pokedex_test)
{
    pkmn::pokedex::sptr dex1 = pkmn::pokedex::make("Red");
    pkmn::pokedex::sptr dex2 = dex1;
    pkmn::pokedex::sptr dex3 = pkmn::copy_pokedex(dex1);

    // This should match
    BOOST_CHECK_EQUAL(dex1, dex2);

    // This should not match
    BOOST_CHECK(dex1 != dex3);
}

/*
 * Make sure pokemon::sptrs copy correctly.
 */
BOOST_AUTO_TEST_CASE(copy_pokemon_test)
{
    pkmn::pokemon::sptr pk1 = pkmn::pokemon::make("Houndoom", "X", 50,
                                                  "None", "None",
                                                  "None", "None");
    pkmn::pokemon::sptr pk2 = pk1;
    pkmn::pokemon::sptr pk3 = pkmn::copy_pokemon(pk1);

    // This should match
    BOOST_CHECK_EQUAL(pk1, pk2);

    // This should not match
    BOOST_CHECK(pk1 != pk3);
}

/*
 * Make sure prng::sptrs copy correctly.
 */
BOOST_AUTO_TEST_CASE(copy_prng_test)
{
    pkmn::prng::sptr rng1 = pkmn::prng::make(3);
    pkmn::prng::sptr rng2 = rng1;
    pkmn::prng::sptr rng3 = pkmn::copy_prng(rng1);

    // This should match
    BOOST_CHECK_EQUAL(rng1, rng2);

    // This should not match
    BOOST_CHECK(rng1 != rng3);
}

/*
 * Make sure trainer::sptrs copy correctly.
 */
BOOST_AUTO_TEST_CASE(copy_trainer_test)
{
    pkmn::trainer::sptr trainer1 = pkmn::trainer::make("Red", "Red", "Male");
    pkmn::trainer::sptr trainer2 = trainer1;
    pkmn::trainer::sptr trainer3 = pkmn::copy_trainer(trainer1);

    // This should match
    BOOST_CHECK_EQUAL(trainer1, trainer2);

    // This should not match
    BOOST_CHECK(trainer1 != trainer3);
}
