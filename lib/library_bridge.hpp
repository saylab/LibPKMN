/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_LIBRARY_BRIDGE_HPP
#define INCLUDED_LIBRARY_BRIDGE_HPP

#include <cstdint>

#include <boost/assign.hpp>

#include <pkmn/config.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/pkstring.hpp>

#include <pkmds/pkmds_g5.h>

namespace pkmn
{
    static const pkmn::dict<uint8_t, pkstring> ball_dict = boost::assign::map_list_of
        (Balls::UNKNOWN, "Poke Ball")
        (Balls::POKE_BALL, "Poke Ball")
        (Balls::GREAT_BALL, "Great Ball")
        (Balls::ULTRA_BALL, "Ultra Ball")
        (Balls::MASTER_BALL, "Master Ball")
        (Balls::SAFARI_BALL, "Safari Ball")
        (Balls::LEVEL_BALL, "Level Ball")
        (Balls::LURE_BALL, "Lure Ball")
        (Balls::MOON_BALL, "Moon Ball")
        (Balls::FRIEND_BALL, "Friend Ball")
        (Balls::LOVE_BALL, "Love Ball")
        (Balls::HEAVY_BALL, "Heavy Ball")
        (Balls::FAST_BALL, "Fast Ball")
        (Balls::SPORT_BALL, "Sport Ball")
        (Balls::PREMIER_BALL, "Premier Ball")
        (Balls::REPEAT_BALL, "Repeat Ball")
        (Balls::TIMER_BALL, "Timer Ball")
        (Balls::NEST_BALL, "Nest Ball")
        (Balls::NET_BALL, "Net Ball")
        (Balls::DIVE_BALL, "Dive Ball")
        (Balls::LUXURY_BALL, "Luxury Ball")
        (Balls::HEAL_BALL, "Heal Ball")
        (Balls::QUICK_BALL, "Quick Ball")
        (Balls::DUSK_BALL, "Dusk Ball")
        (Balls::CHERISH_BALL, "Cherish Ball")
        (Balls::PARK_BALL, "Park Ball")
        (Balls::DREAM_BALL, "Dream Ball")
    ;

    static const pkmn::dict<pkstring, uint8_t> reverse_ball_dict = boost::assign::map_list_of
        ("None", Balls::POKE_BALL)
        ("Poke Ball", Balls::POKE_BALL)
        ("Great Ball", Balls::GREAT_BALL)
        ("Ultra Ball", Balls::ULTRA_BALL)
        ("Master Ball", Balls::MASTER_BALL)
        ("Safari Ball", Balls::SAFARI_BALL)
        ("Level Ball", Balls::LEVEL_BALL)
        ("Lure Ball", Balls::LURE_BALL)
        ("Moon Ball", Balls::MOON_BALL)
        ("Friend Ball", Balls::FRIEND_BALL)
        ("Love Ball", Balls::LOVE_BALL)
        ("Heavy Ball", Balls::HEAVY_BALL)
        ("Fast Ball", Balls::FAST_BALL)
        ("Sport Ball", Balls::SPORT_BALL)
        ("Premier Ball", Balls::PREMIER_BALL)
        ("Repeat Ball", Balls::REPEAT_BALL)
        ("Timer Ball", Balls::TIMER_BALL)
        ("Nest Ball", Balls::NEST_BALL)
        ("Net Ball", Balls::NET_BALL)
        ("Dive Ball", Balls::DIVE_BALL)
        ("Luxury Ball", Balls::LUXURY_BALL)
        ("Heal Ball", Balls::HEAL_BALL)
        ("Quick Ball", Balls::QUICK_BALL)
        ("Dusk Ball", Balls::DUSK_BALL)
        ("Cherish Ball", Balls::CHERISH_BALL)
        ("Park Ball", Balls::PARK_BALL)
        ("Dream Ball", Balls::DREAM_BALL)
    ;

    uint8_t modern_get_IV(uint32_t* IVint, uint8_t IV);

    void modern_set_IV(uint32_t* IVint, uint8_t IV, uint8_t val);

    bool get_marking(uint8_t* markint, uint8_t mark);

    void set_marking(uint8_t* markint, uint8_t mark, bool val);

    bool get_ribbon(uint32_t* ribbonint, uint8_t ribbon);

    void set_ribbon(uint32_t* ribbonint, uint8_t ribbon, bool val);

    uint8_t get_gen3_ball(uint16_t* metlevelint);

    void set_gen3_ball(uint16_t* metlevelint, uint8_t ball);

    uint8_t get_gen3_met_level(uint16_t* metlevelint);

    void set_gen3_met_level(uint16_t* metlevelint, uint8_t level);

    uint8_t get_gen_456_met_level(uint8_t* metlevelint);

    void set_gen_456_met_level(uint8_t* metlevelint, uint8_t level);

    bool get_gen3_otgender(uint16_t* metlevelint);

    void set_gen3_otgender(uint16_t* metlevelint, bool is_female);

    bool get_gen_456_otgender(uint8_t* metlevelint);

    void set_gen_456_otgender(uint8_t* metlevelint, bool is_female);

    uint8_t libpkmn_getpkmstat(pkmds::pokemon_obj* pkm);

    void libpkmn_pctoparty(pkmds::party_pkm* p_pkm, pkmds::pokemon_obj* pkm);

    uint8_t libpkmn_game_to_hometown(uint8_t game);

    uint8_t hometown_to_libpkmn_game(uint8_t hometown);

    uint8_t libpkmn_ball_to_game_ball(uint8_t ball);

    uint8_t game_ball_to_libpkmn_ball(uint8_t game_ball);

    namespace Hometowns
    {
        enum hometowns
        {
            COLOSSEUM_BONUS,
            SAPPHIRE,
            RUBY,
            EMERALD,
            FIRERED,
            LEAFGREEN,
            HEARTGOLD = 7,
            SOULSILVER,
            DIAMOND = 10,
            PEARL,
            PLATINUM,
            COLOSSEUM_XD = 15,
            WHITE = 20,
            BLACK,
            WHITE_2,
            BLACK_2,
            X,
            Y
        };
    }
}

#endif /* INCLUDED_LIBRARY_BRIDGE_HPP */
