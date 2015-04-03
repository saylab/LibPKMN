/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <random>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/test/unit_test.hpp>

#include <pkmn/io.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/native/pokemon.hpp>

namespace fs = boost::filesystem;

static void pksql_common_equality_check(pkmn::pokemon::sptr pkmn1, pkmn::pokemon::sptr pkmn2)
{
    BOOST_CHECK_EQUAL(pkmn1->get_species_id(), pkmn2->get_species_id());
    BOOST_CHECK_EQUAL(pkmn1->get_game_id(), pkmn2->get_game_id());
    BOOST_CHECK_EQUAL(pkmn1->get_original_game_id(), pkmn2->get_original_game_id());
    BOOST_CHECK_EQUAL(pkmn1->get_pokemon_id(), pkmn2->get_pokemon_id());
    BOOST_CHECK_EQUAL(pkmn1->get_ability_id(), pkmn2->get_ability_id());
    BOOST_CHECK_EQUAL(pkmn1->get_item_id(), pkmn2->get_item_id());
    BOOST_CHECK_EQUAL(pkmn1->get_nature_id(), pkmn2->get_nature_id());
    BOOST_CHECK_EQUAL(pkmn1->get_form_id(), pkmn2->get_form_id());
    BOOST_CHECK(pkmn1->get_markings() == pkmn2->get_markings());
    BOOST_CHECK(pkmn1->get_ribbons().hoenn == pkmn2->get_ribbons().hoenn);
    BOOST_CHECK(pkmn1->get_ribbons().sinnoh.ribbons1 == pkmn2->get_ribbons().sinnoh.ribbons1);
    BOOST_CHECK(pkmn1->get_ribbons().sinnoh.ribbons2 == pkmn2->get_ribbons().sinnoh.ribbons2);
    BOOST_CHECK(pkmn1->get_ribbons().sinnoh.ribbons3 == pkmn2->get_ribbons().sinnoh.ribbons3);
    BOOST_CHECK(pkmn1->get_ribbons().unova == pkmn2->get_ribbons().unova);
    BOOST_CHECK(pkmn1->get_ribbons().kalos == pkmn2->get_ribbons().kalos);
    BOOST_CHECK(pkmn1->get_super_training_medals() == pkmn2->get_super_training_medals());
    BOOST_CHECK(pkmn1->get_pokerus() == pkmn2->get_pokerus());
    BOOST_CHECK(pkmn1->get_nickname() == pkmn2->get_nickname());
    BOOST_CHECK(pkmn1->get_trainer_name() == pkmn2->get_trainer_name());
    BOOST_CHECK(pkmn1->get_trainer_gender() == pkmn2->get_trainer_gender());
    BOOST_CHECK_EQUAL(pkmn1->get_trainer_id(), pkmn2->get_trainer_id());
    BOOST_CHECK(pkmn1->get_ball() == pkmn2->get_ball());
    BOOST_CHECK_EQUAL(pkmn1->get_met_level(), pkmn2->get_met_level());
    BOOST_CHECK_EQUAL(pkmn1->get_personality(), pkmn2->get_personality());
    BOOST_CHECK_EQUAL(pkmn1->get_friendship(), pkmn2->get_friendship());
    BOOST_CHECK_EQUAL(pkmn1->get_level(), pkmn2->get_level());
    BOOST_CHECK_EQUAL(pkmn1->get_experience(), pkmn2->get_experience());
    BOOST_CHECK(pkmn1->get_gender() == pkmn2->get_gender());
    BOOST_CHECK_EQUAL(pkmn1->is_shiny(), pkmn2->is_shiny());
}

BOOST_AUTO_TEST_CASE(gen1_pksql_test)
{
    srand((unsigned int)time(NULL));

    pkmn::pokemon::sptr pkmn1 = pkmn::pokemon::make("Bulbasaur", "Red", 50,
                                                    "Tackle", "Vine Whip",
                                                    "Sleep Powder", "Razor Leaf");

    std::string filename = str(boost::format("pksql_gen1_%d.pksql") % rand());
    fs::path filepath = fs::path(fs::path(pkmn::get_tmp_dir()) / filename);

    pkmn::pokemon::export_to(pkmn1, filepath.string());
    pkmn::pokemon::sptr pkmn2 = pkmn::pokemon::make(filepath.string());

    pksql_common_equality_check(pkmn1, pkmn2);

    fs::remove(filepath);
}

BOOST_AUTO_TEST_CASE(gen2_pksql_test)
{
    pkmn::pokemon::sptr pkmn1 = pkmn::pokemon::make("Chikorita", "Gold", 50,
                                                    "Tackle", "Vine Whip",
                                                    "Sleep Powder", "Razor Leaf");

    std::string filename = str(boost::format("pksql_gen2_%d.pksql") % rand());
    fs::path filepath = fs::path(fs::path(pkmn::get_tmp_dir()) / filename);

    pkmn::pokemon::export_to(pkmn1, filepath.string());
    pkmn::pokemon::sptr pkmn2 = pkmn::pokemon::make(filepath.string());

    pksql_common_equality_check(pkmn1, pkmn2);

    fs::remove(filepath);
}
