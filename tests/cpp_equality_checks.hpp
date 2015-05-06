/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __INCLUDED_CPP_EQUALITY_CHECKS_HPP__
#define __INCLUDED_CPP_EQUALITY_CHECKS_HPP__

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/pokemon.hpp>
#include <pkmn/trainer.hpp>

static int native_sizes[7] = {0,44,48,100,380,380,500};

void pokemon_equality_check(pkmn::pokemon::sptr pkmn1, pkmn::pokemon::sptr pkmn2)
{
    BOOST_CHECK_EQUAL(pkmn1->get_species_id(), pkmn2->get_species_id());
    BOOST_CHECK_EQUAL(pkmn1->get_game_id(), pkmn2->get_game_id());

    if(pkmn1->get_species_id() == pkmn::Species::NONE)
        return;
    else if(pkmn1->get_species_id() == pkmn::Species::INVALID)
    {
        BOOST_CHECK(!memcmp(pkmn1->get_native(), pkmn2->get_native(), native_sizes[pkmn1->get_generation()]));
        return;
    }

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

void trainer_equality_check(pkmn::trainer::sptr trainer1, pkmn::trainer::sptr trainer2)
{
    BOOST_CHECK_EQUAL(trainer1->get_game_id(), trainer2->get_game_id());
    BOOST_CHECK(trainer1->get_name() == trainer2->get_name());
    BOOST_CHECK(trainer1->get_gender() == trainer2->get_gender());
    BOOST_CHECK_EQUAL(trainer1->get_id(), trainer2->get_id());
    BOOST_CHECK_EQUAL(trainer1->get_money(), trainer2->get_money());

    pkmn::pokemon_team_t party1 = trainer1->get_party();
    pkmn::pokemon_team_t party2 = trainer2->get_party();

    BOOST_CHECK_EQUAL(party1.size(), party2.size());
    for(size_t i = 0; i < party1.size(); i++)
        pokemon_equality_check(party1[i], party2[i]);
}

#endif /* __INCLUDED_CPP_EQUALITY_CHECKS_HPP__ */
