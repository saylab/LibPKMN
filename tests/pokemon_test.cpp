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
    // Start with SQLite ID's
    BOOST_CHECK_EQUAL(pkmn1->get_species_id(), pkmn2->get_species_id());
    BOOST_CHECK_EQUAL(pkmn1->get_game_id(), pkmn2->get_game_id());
    BOOST_CHECK_EQUAL(pkmn1->get_original_game_id(), pkmn2->get_original_game_id());
    BOOST_CHECK_EQUAL(pkmn1->get_pokemon_id(), pkmn2->get_pokemon_id());
    BOOST_CHECK_EQUAL(pkmn1->get_ability_id(), pkmn2->get_ability_id());
    BOOST_CHECK_EQUAL(pkmn1->get_item_id(), pkmn2->get_item_id());
    BOOST_CHECK_EQUAL(pkmn1->get_nature_id(), pkmn2->get_nature_id());
    BOOST_CHECK_EQUAL(pkmn1->get_form_id(), pkmn2->get_form_id());

    // Name storage is implementations-specific, so use LibPKMN function
    BOOST_CHECK(pkmn1->get_nickname() == pkmn2->get_nickname());
    BOOST_CHECK(pkmn1->get_trainer_name() == pkmn2->get_trainer_name());

    //

    BOOST_CHECK_EQUAL(uint8_t(pkmn1->get_markings()), uint8_t(pkmn2->get_markings()));
}

BOOST_AUTO_TEST_CASE(gen1_pksql_test)
{
    srand((unsigned int)time(NULL));

    pkmn::pokemon::sptr pkmn1 = pkmn::pokemon::make("Bulbasaur", "Red", 50,
                                                    "Tackle", "Vine Whip",
                                                    "Sleep Powder", "Razor Leaf");

    std::string filename = str(boost::format("pksql_gen1_%d.pksql") % rand());
    fs::path filepath = fs::path(fs::path(pkmn::get_tmp_dir()) / filename);

    pkmn::io::export_to_pksql(pkmn1, filepath.string());
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

    pkmn::io::export_to_pksql(pkmn1, filepath.string());
    pkmn::pokemon::sptr pkmn2 = pkmn::pokemon::make(filepath.string());

    pksql_common_equality_check(pkmn1, pkmn2);

    fs::remove(filepath);
}
