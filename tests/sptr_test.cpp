/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include <pkmn/bag.hpp>
#include <pkmn/base_pokemon.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/item.hpp>
#include <pkmn/pocket.hpp>
#include <pkmn/team_pokemon.hpp>
#include <pkmn/trainer.hpp>
#include <pkmn/types/prng.hpp>

#include "copy_sptrs.hpp"

/*
 * Copying a bag involves copying a pocket as well, so make sure only the pointers
 * that are supposed to be equal are equal.
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
 * Make sure all three base_pokemon implementations copy correctly.
 */
BOOST_AUTO_TEST_CASE(copy_base_pokemon_test)
{
    pkmn::base_pokemon::sptr bp1_1 = pkmn::base_pokemon::make("Bulbasaur", "Red");  // base_pokemon_gen1impl
    pkmn::base_pokemon::sptr bp2_1 = pkmn::base_pokemon::make("Bulbasaur", "Gold"); // base_pokemon_gen2impl
    pkmn::base_pokemon::sptr bpm_1 = pkmn::base_pokemon::make("Bulbasaur", "Ruby"); // base_pokemon_modernimpl

    pkmn::base_pokemon::sptr bp1_2 = bp1_1;
    pkmn::base_pokemon::sptr bp2_2 = bp2_1;
    pkmn::base_pokemon::sptr bpm_2 = bpm_1;

    pkmn::base_pokemon::sptr bp1_3 = pkmn::copy_base_pokemon(bp1_1);
    pkmn::base_pokemon::sptr bp2_3 = pkmn::copy_base_pokemon(bp2_1);
    pkmn::base_pokemon::sptr bpm_3 = pkmn::copy_base_pokemon(bpm_1);

    // These should all match
    BOOST_CHECK_EQUAL(bp1_1, bp1_2);
    BOOST_CHECK_EQUAL(bp2_1, bp2_2);
    BOOST_CHECK_EQUAL(bpm_1, bpm_2);

    // These should all be different
    BOOST_CHECK(bp1_1 != bp1_3);
    BOOST_CHECK(bp2_1 != bp2_3);
    BOOST_CHECK(bpm_1 != bpm_3);
}

/*
 * Make sure both item implementations copy correctly.
 */
BOOST_AUTO_TEST_CASE(copy_item_test)
{
    pkmn::item::sptr i1 = pkmn::item::make("Potion", "Diamond"); // item_impl
    pkmn::item::sptr im1 = pkmn::item::make("TM01", "Diamond");  // item_machineimpl

    pkmn::item::sptr i2 = i1;
    pkmn::item::sptr im2 = im1;

    pkmn::item::sptr i3 = pkmn::copy_item(i1);
    pkmn::item::sptr im3 = pkmn::copy_item(im1);

    // These should both match
    BOOST_CHECK_EQUAL(i1, i2);
    BOOST_CHECK_EQUAL(im1, im2);

    // These should both be different
    BOOST_CHECK(i1 != i3);
    BOOST_CHECK(im1 != im3);
}

/*
 * Make sure moves copy correctly.
 */
BOOST_AUTO_TEST_CASE(copy_move_test)
{
    pkmn::move::sptr move1 = pkmn::move::make("Tackle", "Diamond");
    pkmn::move::sptr move2 = move1;
    pkmn::move::sptr move3 = pkmn::copy_move(move1);

    // This should match
    BOOST_CHECK_EQUAL(move1, move2);

    // This should not match
    BOOST_CHECK(move1 != move3);
}

/*
 * Make sure all three team_pokemon implementations copy correctly.
 * Make sure team_pokemon_modernimpl's signals/slots don't copy.
 */
BOOST_AUTO_TEST_CASE(copy_team_pokemon_test)
{
    pkmn::team_pokemon::sptr tp1_1 = pkmn::team_pokemon::make("Bulbasaur", "Red", 50,
                                                              "None", "None", "None", "None"); // team_pokemon_gen1impl
    pkmn::team_pokemon::sptr tp2_1 = pkmn::team_pokemon::make("Bulbasaur", "Gold", 50,
                                                              "None", "None", "None", "None"); // team_pokemon_gen2impl
    pkmn::team_pokemon::sptr tpm_1 = pkmn::team_pokemon::make("Bulbasaur", "Ruby", 50,
                                                              "None", "None", "None", "None"); // team_pokemon_modernimpl

    pkmn::team_pokemon::sptr tp1_2 = tp1_1;
    pkmn::team_pokemon::sptr tp2_2 = tp2_1;
    pkmn::team_pokemon::sptr tpm_2 = tpm_1;

    pkmn::team_pokemon::sptr tp1_3 = pkmn::copy_team_pokemon(tp1_1);
    pkmn::team_pokemon::sptr tp2_3 = pkmn::copy_team_pokemon(tp2_1);
    pkmn::team_pokemon::sptr tpm_3 = pkmn::copy_team_pokemon(tpm_1);

    // These should all match
    BOOST_CHECK_EQUAL(tp1_1, tp1_2);
    BOOST_CHECK_EQUAL(tp2_1, tp2_2);
    BOOST_CHECK_EQUAL(tpm_1, tpm_2);

    // These should all be different
    BOOST_CHECK(tp1_1 != tp1_3);
    BOOST_CHECK(tp2_1 != tp2_3);
    BOOST_CHECK(tpm_1 != tpm_3);

    /*
     * team_pokemon_modernimpl uses a signal/slot system to update its stats when
     * its base_pokemon's form is changed outside of itself. This makes sure the signals
     * are properly reinitialized when copied.
     */
    pkmn::team_pokemon::sptr deoxys = pkmn::team_pokemon::make("Deoxys", "Diamond", 50,
                                                               "None", "None", "None", "None");
    pkmn::dict<pkmn::pkstring, unsigned int> deoxys_stats1 = deoxys->get_stats();

    pkmn::base_pokemon::sptr deoxysbase1 = deoxys->get_base_pokemon(false); // Same as from deoxys
    pkmn::base_pokemon::sptr deoxysbase2 = deoxys->get_base_pokemon(true);  // Copied from deoxys

    deoxysbase1->set_form("Attack");
    deoxysbase2->set_form("Defense");

    pkmn::dict<pkmn::pkstring, unsigned int> deoxys_stats2 = deoxys->get_stats();

    BOOST_CHECK(deoxysbase1 != deoxysbase2);
    BOOST_CHECK_EQUAL(deoxys->get_form_id(), pkmn::Forms::Deoxys::ATTACK);
    BOOST_CHECK_EQUAL(deoxysbase1->get_form_id(), pkmn::Forms::Deoxys::ATTACK);
    BOOST_CHECK_EQUAL(deoxys->get_form_id(), deoxysbase1->get_form_id());
    BOOST_CHECK_EQUAL(deoxysbase2->get_form_id(), pkmn::Forms::Deoxys::DEFENSE);
    BOOST_CHECK(deoxys_stats1["Attack"] != deoxys_stats2["Attack"]);
    BOOST_CHECK(deoxys_stats1["Defense"] != deoxys_stats2["Defense"]);
    BOOST_CHECK(deoxys_stats1["Special Attack"] != deoxys_stats2["Special Attack"]);
    BOOST_CHECK(deoxys_stats1["Special Defense"] != deoxys_stats2["Special Defense"]);
}

/*
 * Make sure trainers copy correctly.
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

/*
 * Make sure PRNG's copy correctly.
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