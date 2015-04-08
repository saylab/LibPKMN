/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <random>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/test/unit_test.hpp>

#include <pkmn/paths.hpp>
#include <pkmn/pokemon.hpp>
#include <pkmn/native/pokemon.hpp>

namespace fs = boost::filesystem;

static void pksql_equality_check(pkmn::pokemon::sptr pkmn1, pkmn::pokemon::sptr pkmn2)
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

    pkmn::dict<pkmn::pkstring, int> pkmn1_stats = pkmn1->get_stats();
    pkmn::dict<pkmn::pkstring, int> pkmn2_stats = pkmn2->get_stats();
    BOOST_CHECK_EQUAL(pkmn1_stats.at("HP"), pkmn2_stats.at("HP"));
    BOOST_CHECK_EQUAL(pkmn1_stats.at("Attack"), pkmn2_stats.at("Attack"));
    BOOST_CHECK_EQUAL(pkmn1_stats.at("Defense"), pkmn2_stats.at("Defense"));
    BOOST_CHECK_EQUAL(pkmn1_stats.at("Speed"), pkmn2_stats.at("Speed"));

    if(pkmn1->get_generation() == 1)
        BOOST_CHECK_EQUAL(pkmn1_stats.at("Special"), pkmn2_stats.at("Special"));
    else
    {
        BOOST_CHECK_EQUAL(pkmn1_stats.at("Special Attack"), pkmn2_stats.at("Special Attack"));
        BOOST_CHECK_EQUAL(pkmn1_stats.at("Special Defense"), pkmn2_stats.at("Special Defense"));
    }

    pkmn::dict<pkmn::pkstring, int> pkmn1_EVs = pkmn1->get_EVs();
    pkmn::dict<pkmn::pkstring, int> pkmn2_EVs = pkmn2->get_EVs();
    BOOST_CHECK_EQUAL(pkmn1_EVs.at("HP"), pkmn2_EVs.at("HP"));
    BOOST_CHECK_EQUAL(pkmn1_EVs.at("Attack"), pkmn2_EVs.at("Attack"));
    BOOST_CHECK_EQUAL(pkmn1_EVs.at("Defense"), pkmn2_EVs.at("Defense"));
    BOOST_CHECK_EQUAL(pkmn1_EVs.at("Speed"), pkmn2_EVs.at("Speed"));

    if(pkmn1->get_generation() < 3)
        BOOST_CHECK_EQUAL(pkmn1_EVs.at("Special"), pkmn2_EVs.at("Special"));
    else
    {
        BOOST_CHECK_EQUAL(pkmn1_EVs.at("Special Attack"), pkmn2_EVs.at("Special Attack"));
        BOOST_CHECK_EQUAL(pkmn1_EVs.at("Special Defense"), pkmn2_EVs.at("Special Defense"));
    }

    pkmn::dict<pkmn::pkstring, int> pkmn1_IVs = pkmn1->get_IVs();
    pkmn::dict<pkmn::pkstring, int> pkmn2_IVs = pkmn2->get_IVs();
    BOOST_CHECK_EQUAL(pkmn1_IVs.at("HP"), pkmn2_IVs.at("HP"));
    BOOST_CHECK_EQUAL(pkmn1_IVs.at("Attack"), pkmn2_IVs.at("Attack"));
    BOOST_CHECK_EQUAL(pkmn1_IVs.at("Defense"), pkmn2_IVs.at("Defense"));
    BOOST_CHECK_EQUAL(pkmn1_IVs.at("Speed"), pkmn2_IVs.at("Speed"));

    if(pkmn1->get_generation() < 3)
        BOOST_CHECK_EQUAL(pkmn1_IVs.at("Special"), pkmn2_IVs.at("Special"));
    else
    {
        BOOST_CHECK_EQUAL(pkmn1_IVs.at("Special Attack"), pkmn2_IVs.at("Special Attack"));
        BOOST_CHECK_EQUAL(pkmn1_IVs.at("Special Defense"), pkmn2_IVs.at("Special Defense"));
    }

    pkmn::moveset_t moves1, moves2;
    pkmn1->get_moves(moves1);
    pkmn2->get_moves(moves2);
    BOOST_CHECK(moves1[0].name == moves2[0].name);
    BOOST_CHECK(moves1[1].name == moves2[1].name);
    BOOST_CHECK(moves1[2].name == moves2[2].name);
    BOOST_CHECK(moves1[3].name == moves2[3].name);

    pkmn::dict<pkmn::pkstring, int> pkmn1_attributes = pkmn1->get_attributes();
    pkmn::dict<pkmn::pkstring, int> pkmn2_attributes = pkmn2->get_attributes();
    BOOST_CHECK_EQUAL(pkmn1_attributes.size(), pkmn2_attributes.size());
    BOOST_FOREACH(const pkmn::pkstring &key, pkmn1_attributes.keys())
    {
        BOOST_CHECK_EQUAL(pkmn1_attributes.at(key), pkmn1_attributes.at(key,(pkmn1_attributes.at(key)-1)));
    }
}

BOOST_AUTO_TEST_CASE(gen1_pksql_test)
{
    srand((unsigned int)time(NULL));

    pkmn::pokemon::sptr pkmn1 = pkmn::pokemon::make("Bulbasaur", "Red", 50,
                                                    "Tackle", "Vine Whip",
                                                    "Sleep Powder", "Razor Leaf");

    // Set stats to non-default
    pkmn1->set_nickname("GEN1_PKSQL");
    pkmn1->set_trainer_name("WAT");
    pkmn1->set_trainer_id(01351);
    pkmn1->set_level(7);
    pkmn1->set_EV("HP", 692);
    pkmn1->set_EV("Attack", 2135);
    pkmn1->set_EV("Defense", 846);
    pkmn1->set_EV("Speed", 100);
    pkmn1->set_EV("Special", 8);
    pkmn1->set_IV("HP", 10);
    pkmn1->set_IV("Attack", 2);
    pkmn1->set_IV("Defense", 3);
    pkmn1->set_IV("Speed", 4);
    pkmn1->set_IV("Special", 15);
    pkmn1->set_move("Solar Beam", 1);
    pkmn1->set_move("Stun Spore", 4);
    pkmn1->set_move_PP(10, 2);
    pkmn1->set_move_PP(5, 3);

    std::string filename = str(boost::format("pksql_gen1_%d.pksql") % rand());
    fs::path filepath = fs::path(fs::path(pkmn::get_tmp_dir()) / filename);

    pkmn::pokemon::export_to(pkmn1, filepath.string());
    pkmn::pokemon::sptr pkmn2 = pkmn::pokemon::make(filepath.string());

    pksql_equality_check(pkmn1, pkmn2);

    fs::remove(filepath);
}

BOOST_AUTO_TEST_CASE(gen2_pksql_test)
{
    pkmn::pokemon::sptr pkmn1 = pkmn::pokemon::make("Chikorita", "Gold", 50,
                                                    "Tackle", "Vine Whip",
                                                    "Sleep Powder", "Razor Leaf");

    // Set stats to non-default
    pkmn1->set_nickname("GEN2_PKSQL");
    pkmn1->set_trainer_name("WAT");
    pkmn1->set_trainer_id(01351);
    pkmn1->set_level(70);
    pkmn1->set_EV("HP", 692);
    pkmn1->set_EV("Attack", 2135);
    pkmn1->set_EV("Defense", 846);
    pkmn1->set_EV("Speed", 100);
    pkmn1->set_EV("Special", 8);
    pkmn1->set_IV("HP", 10);
    pkmn1->set_IV("Attack", 2);
    pkmn1->set_IV("Defense", 3);
    pkmn1->set_IV("Speed", 4);
    pkmn1->set_IV("Special", 15);
    pkmn1->set_move("Solar Beam", 1);
    pkmn1->set_move("Stun Spore", 4);
    pkmn1->set_move_PP(10, 2);
    pkmn1->set_move_PP(5, 3);
    pkmn1->set_gender("Female");

    std::string filename = str(boost::format("pksql_gen2_%d.pksql") % rand());
    fs::path filepath = fs::path(fs::path(pkmn::get_tmp_dir()) / filename);

    pkmn::pokemon::export_to(pkmn1, filepath.string());
    pkmn::pokemon::sptr pkmn2 = pkmn::pokemon::make(filepath.string());

    pksql_equality_check(pkmn1, pkmn2);

    fs::remove(filepath);
}

BOOST_AUTO_TEST_CASE(gen3_pksql_test)
{
    pkmn::pokemon::sptr pkmn1 = pkmn::pokemon::make("Treecko", "Emerald", 50,
                                                    "Tackle", "Vine Whip",
                                                    "Sleep Powder", "Razor Leaf");

    // Set stats to non-default
    pkmn1->set_nickname("GEN3_PKSQL");
    pkmn1->set_trainer_name("WAT");
    pkmn1->set_trainer_id(13511351);
    pkmn1->set_level(40);
    pkmn1->set_EV("HP", 200);
    pkmn1->set_EV("Attack", 25);
    pkmn1->set_EV("Defense", 46);
    pkmn1->set_EV("Speed", 10);
    pkmn1->set_EV("Special Attack", 38);
    pkmn1->set_EV("Special Defense", 38);
    pkmn1->set_IV("HP", 10);
    pkmn1->set_IV("Attack", 2);
    pkmn1->set_IV("Defense", 3);
    pkmn1->set_IV("Speed", 4);
    pkmn1->set_IV("Special Attack", 15);
    pkmn1->set_IV("Special Defense", 12);
    pkmn1->set_move("Solar Beam", 1);
    pkmn1->set_move("Slash", 4);
    pkmn1->set_move_PP(10, 2);
    pkmn1->set_move_PP(5, 3);
    pkmn1->set_gender("Female");

    std::string filename = str(boost::format("pksql_gen3_%d.pksql") % rand());
    fs::path filepath = fs::path(fs::path(pkmn::get_tmp_dir()) / filename);

    pkmn::pokemon::export_to(pkmn1, filepath.string());
    pkmn::pokemon::sptr pkmn2 = pkmn::pokemon::make(filepath.string());

    pksql_equality_check(pkmn1, pkmn2);

    fs::remove(filepath);
}

BOOST_AUTO_TEST_CASE(gen4_pksql_test)
{
    pkmn::pokemon::sptr pkmn1 = pkmn::pokemon::make("Turtwig", "Platinum", 50,
                                                    "Tackle", "Vine Whip",
                                                    "Sleep Powder", "Razor Leaf");

    // Set stats to non-default
    pkmn1->set_nickname("GEN4_PKSQL");
    pkmn1->set_trainer_name("WAT");
    pkmn1->set_trainer_id(13511351);
    pkmn1->set_level(40);
    pkmn1->set_EV("HP", 200);
    pkmn1->set_EV("Attack", 25);
    pkmn1->set_EV("Defense", 46);
    pkmn1->set_EV("Speed", 10);
    pkmn1->set_EV("Special Attack", 38);
    pkmn1->set_EV("Special Defense", 38);
    pkmn1->set_IV("HP", 10);
    pkmn1->set_IV("Attack", 2);
    pkmn1->set_IV("Defense", 3);
    pkmn1->set_IV("Speed", 4);
    pkmn1->set_IV("Special Attack", 15);
    pkmn1->set_IV("Special Defense", 12);
    pkmn1->set_move("Solar Beam", 1);
    pkmn1->set_move("Earthquake", 4);
    pkmn1->set_move_PP(10, 2);
    pkmn1->set_move_PP(5, 3);
    pkmn1->set_gender("Female");

    std::string filename = str(boost::format("pksql_gen4_%d.pksql") % rand());
    fs::path filepath = fs::path(fs::path(pkmn::get_tmp_dir()) / filename);

    pkmn::pokemon::export_to(pkmn1, filepath.string());
    pkmn::pokemon::sptr pkmn2 = pkmn::pokemon::make(filepath.string());

    pksql_equality_check(pkmn1, pkmn2);

    fs::remove(filepath);
}

BOOST_AUTO_TEST_CASE(gen5_pksql_test)
{
    pkmn::pokemon::sptr pkmn1 = pkmn::pokemon::make("Snivy", "White", 50,
                                                    "Tackle", "Vine Whip",
                                                    "Sleep Powder", "Razor Leaf");

    // Set stats to non-default
    pkmn1->set_nickname("GEN5_PKSQL");
    pkmn1->set_trainer_name("WAT");
    pkmn1->set_trainer_id(13511351);
    pkmn1->set_level(100);
    pkmn1->set_EV("HP", 200);
    pkmn1->set_EV("Attack", 25);
    pkmn1->set_EV("Defense", 46);
    pkmn1->set_EV("Speed", 10);
    pkmn1->set_EV("Special Attack", 38);
    pkmn1->set_EV("Special Defense", 38);
    pkmn1->set_IV("HP", 10);
    pkmn1->set_IV("Attack", 2);
    pkmn1->set_IV("Defense", 3);
    pkmn1->set_IV("Speed", 4);
    pkmn1->set_IV("Special Attack", 15);
    pkmn1->set_IV("Special Defense", 12);
    pkmn1->set_move("Solar Beam", 1);
    pkmn1->set_move("Earthquake", 4);
    pkmn1->set_move_PP(10, 2);
    pkmn1->set_move_PP(5, 3);
    pkmn1->set_gender("Female");

    std::string filename = str(boost::format("pksql_gen5_%d.pksql") % rand());
    fs::path filepath = fs::path(fs::path(pkmn::get_tmp_dir()) / filename);

    pkmn::pokemon::export_to(pkmn1, filepath.string());
    pkmn::pokemon::sptr pkmn2 = pkmn::pokemon::make(filepath.string());

    pksql_equality_check(pkmn1, pkmn2);

    fs::remove(filepath);
}
