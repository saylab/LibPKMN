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

#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/pokemon.hpp>
#include <pkmn/trainer.hpp>
#include <pkmn/conversions/pokemon.hpp>
#include <pkmn/conversions/text.hpp>
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

    BOOST_CHECK(pkmn1->get_nickname() == "GEN1_PKSQL");
    BOOST_CHECK(pkmn1->get_trainer_name() == "WAT");
    BOOST_CHECK_EQUAL(pkmn1->get_trainer_id(), 01351);

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

    BOOST_CHECK(pkmn1->get_nickname() == "GEN2_PKSQL");
    BOOST_CHECK(pkmn1->get_trainer_name() == "WAT");
    BOOST_CHECK_EQUAL(pkmn1->get_trainer_id(), 01351);

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

BOOST_AUTO_TEST_CASE(trsql_test)
{
    pkmn::pokemon::sptr pkmn1 = pkmn::pokemon::make("Bulbasaur", "Red", 50,
                                                    "Tackle", "Vine Whip",
                                                    "Sleep Powder", "Razor Leaf");

    pkmn::pokemon::sptr pkmn2 = pkmn::pokemon::make("Charmander", "Red", 50,
                                                    "Scratch", "Ember",
                                                    "Tail Whip", "Slash");

    pkmn::pokemon::sptr pkmn3 = pkmn::pokemon::make("Squirtle", "Red", 50,
                                                    "Tackle", "Bubble",
                                                    "Water Gun", "Leech Seed");

    pkmn::pokemon::sptr pkmn4 = pkmn::pokemon::make("Pikachu", "Red", 50,
                                                    "Thunder", "Thunder Wave",
                                                    "Flash", "Tail Whip");

    // Invalid
    pkmn::native::gen1_pc_pokemon_t raw5;
    memset(&raw5, 0x0, sizeof(pkmn::native::gen1_pc_pokemon_t));
    raw5.species = 181; // Missingno.
    raw5.moves[0] = pkmn::Moves::SKY_ATTACK;
    raw5.moves[1] = pkmn::Moves::WATER_GUN;
    raw5.moves[2] = pkmn::Moves::WATER_GUN;

    uint8_t* nickname_buffer = new uint8_t[15];
    pkmn::conversions::export_gen1_text("MISSINGNO.", nickname_buffer, 10);

    uint8_t* otname_buffer = new uint8_t[15];
    pkmn::conversions::export_gen1_text("LIBPKMN", otname_buffer, 10);

    pkmn::pokemon::sptr pkmn5 = pkmn::conversions::import_gen1_pokemon(raw5, nickname_buffer,
                                                                       otname_buffer, "Red");

    delete[] nickname_buffer;
    delete[] otname_buffer;

    pkmn::trainer::sptr trainer1 = pkmn::trainer::make("Red", "LIBPKMN", "Male");
    trainer1->set_pokemon(1, pkmn1);
    trainer1->set_pokemon(2, pkmn2);
    trainer1->set_pokemon(3, pkmn3);
    trainer1->set_pokemon(4, pkmn4);
    trainer1->set_pokemon(5, pkmn5);

    std::string filename = str(boost::format("trsql_%d.trsql") % rand());
    fs::path filepath = fs::path(fs::path(pkmn::get_tmp_dir()) / filename);

    pkmn::trainer::export_to(trainer1, filepath.string());
    pkmn::trainer::sptr trainer2 = pkmn::trainer::make(filepath.string());

    trainer_equality_check(trainer1, trainer2);

    //fs::remove(filepath);
}
