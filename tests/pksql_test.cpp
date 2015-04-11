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

#include "cpp_equality_checks.hpp"

namespace fs = boost::filesystem;

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

    pokemon_equality_check(pkmn1, pkmn2);

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

    pokemon_equality_check(pkmn1, pkmn2);

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

    pokemon_equality_check(pkmn1, pkmn2);

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

    pokemon_equality_check(pkmn1, pkmn2);

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

    pokemon_equality_check(pkmn1, pkmn2);

    fs::remove(filepath);
}
