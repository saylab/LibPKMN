/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifdef _MSC_VER
#pragma warning(disable:4244)
#endif

#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database/queries.hpp>

#include "pkmds/pkmds_sql.h"

#include "library_bridge.hpp"
#include "SQLiteCpp/SQLiteC++.h"

#define MAX_NICKNAME_LEN 10
#define MAX_TRAINER_NAME_LEN 7

using namespace std;

namespace pkmn
{
    uint8_t modern_get_IV(uint32_t* IVint, uint8_t IV)
    {
        switch(IV)
        {
            case Stats::HP:
                return ((*IVint >> 2) & 31);

            case Stats::ATTACK:
                return ((*IVint >> 7) & 31);

            case Stats::DEFENSE:
                return ((*IVint >> 12) & 31);

            case Stats::SPEED:
                return ((*IVint >> 17) & 31);

            case Stats::SPECIAL_ATTACK:
                return ((*IVint >> 22) & 31);

            case Stats::SPECIAL_DEFENSE:
                return ((*IVint >> 27) & 31);

            default:
                return 0;
        }
    }

    void modern_set_IV(uint32_t* IVint, uint8_t IV, uint8_t val)
    {
        switch(IV)
        {
            case Stats::HP:
                *IVint = ((*IVint & 0xFFFFFF83) | (val << 2));
                break;

            case Stats::ATTACK:
                *IVint = ((*IVint & 0xFFFFF07F) | (val << 7));
                break;

            case Stats::DEFENSE:
                *IVint = ((*IVint & 0xFFFE0FFF) | (val << 12));
                break;

            case Stats::SPEED:
                *IVint = ((*IVint & 0xFFC1FFFF) | (val << 17));
                break;

            case Stats::SPECIAL_ATTACK:
                *IVint = ((*IVint & 0xF83FFFFF) | (val << 22));
                break;

            case Stats::SPECIAL_DEFENSE:
                *IVint = ((*IVint & 0x7FFFFFF) | (val << 27));
                break;
        }
    }

    bool get_marking(uint8_t* markint, uint8_t mark)
    {
        return ((*markint >> mark) & 0x1);
    }

    void set_marking(uint8_t* markint, uint8_t mark, bool val)
    {
        if(val) *markint |= (1 << mark);
        else *markint &= ~(1 << mark);
    }

    bool get_ribbon(uint32_t* ribbonint, uint8_t ribbon)
    {
        return ((*ribbonint >> ribbon) & 0x1);
    }

    void set_ribbon(uint32_t* ribbonint, uint8_t ribbon, bool val)
    {
        if(val) *ribbonint |= (1 << ribbon);
        else *ribbonint &= ~(1 << ribbon);
    }

    uint8_t get_gen3_ball(uint16_t* metlevelint)
    {
        return ((*metlevelint >> 1) & 0x8);
    }

    void set_gen3_ball(uint16_t* metlevelint, uint8_t ball)
    {
        //Ball can only be 4 bits long and must be shifted by one
        ball = (ball & 0xF) << 1;
        *metlevelint = (*metlevelint & 0xFFE0) | ball | (*metlevelint & 0x1);
    }

    uint8_t get_gen3_met_level(uint16_t* metlevelint)
    {
        return (*metlevelint >> 9);
    }

    void set_gen3_met_level(uint16_t* metlevelint, uint8_t level)
    {
        //Level can only be 7 bits long and must be shifted by nine
        uint16_t intermediate_level = (level & 0x7F) << 9;
        *metlevelint = intermediate_level | (*metlevelint & 0x1FF);
    }

    uint8_t get_gen_456_met_level(uint8_t* metlevelint)
    {
        return (*metlevelint >> 1);
    }

    void set_gen_456_met_level(uint8_t* metlevelint, uint8_t level)
    {
        //Level can only be 7 bits long and must be shifted by one
        level = (level & 0x7F) << 1;
        *metlevelint = (*metlevelint & 0x1) | level;
    }

    bool get_gen3_otgender(uint16_t* metlevelint)
    {
        return (*metlevelint & 0x1);
    }

    void set_gen3_otgender(uint16_t* metlevelint, bool is_female)
    {
        *metlevelint = (*metlevelint & 0xFFFE) | ((is_female) ? 1 : 0);
    }

    bool get_gen_456_otgender(uint8_t* metlevelint)
    {
        return (*metlevelint & 0x1);
    }

    void set_gen_456_otgender(uint8_t* metlevelint, bool is_female)
    {
        *metlevelint = (*metlevelint & 0xFE) | ((is_female) ? 1 : 0);
    }

    uint8_t libpkmn_getpkmstat(pkmds::pokemon_obj* pkm, unsigned int stat_id)
    {
        SQLite::Database db(get_database_path().c_str());
        std::string pkmstatsql = pkmds::getpkmstatsql(pkm, pkmds::Stat_IDs::stat_ids(stat_id));
        unsigned int basestat = db.execAndGet(pkmstatsql.c_str());

        std::string pkmlevelsql = pkmds::getpkmlevelsql(int(pkm->species),
                                                        int(pkm->exp));
        unsigned int level = db.execAndGet(pkmlevelsql.c_str());
        unsigned int ev = 0;
        unsigned int iv = 0;

        uint32_t* IVint = reinterpret_cast<uint32_t*>(&(pkm->ppup[3])+1);
        switch(stat_id)
        {
            case Stats::HP:
                ev = pkm->evs.hp;
                iv = modern_get_IV(IVint, Stats::HP);
                return (int)((floor((double)(floor((double)(((iv +
                       (2 * basestat) + floor((double)(ev/4))+100)
                       * level) / 100)) + 10))));

            case Stats::ATTACK:
                ev = pkm->evs.attack;
                break;

            case Stats::DEFENSE:
                ev = pkm->evs.defense;
                break;

            case Stats::SPECIAL_ATTACK:
                ev = pkm->evs.spatk;
                break;

            case Stats::SPECIAL_DEFENSE:
                ev = pkm->evs.spdef;
                break;

            default:
                ev = pkm->evs.speed;
        }
        iv = modern_get_IV(IVint, stat_id);
        double naturemod = database::get_nature_stat_mod(
                           (unsigned int)(pkm->nature), stat_id);
        return (int)((floor((double)(floor((double)(((iv + (2 * basestat)
               + floor((double)(ev/4))) * level) / 100)) + 5)) * naturemod));
    }

    void libpkmn_pctoparty(pkmds::party_pkm* p_pkm, pkmds::pokemon_obj* pkm)
    {
        memcpy(p_pkm, pkm, sizeof(*pkm));
        p_pkm->party_data.maxhp = libpkmn_getpkmstat(pkm, Stats::HP);
        p_pkm->party_data.hp = p_pkm->party_data.maxhp;
        p_pkm->party_data.attack = libpkmn_getpkmstat(pkm, Stats::ATTACK);
        p_pkm->party_data.defense = libpkmn_getpkmstat(pkm, Stats::DEFENSE);
        p_pkm->party_data.spatk = libpkmn_getpkmstat(pkm, Stats::SPECIAL_ATTACK);
        p_pkm->party_data.spdef = libpkmn_getpkmstat(pkm, Stats::SPECIAL_DEFENSE);
        p_pkm->party_data.speed = libpkmn_getpkmstat(pkm, Stats::SPEED);

        SQLite::Database db(get_database_path().c_str());
        std::string pkxlevelsql = pkmds::getpkmlevelsql(int(pkm->species),
                                                        int(pkm->exp));
        p_pkm->party_data.level = int(db.execAndGet(pkxlevelsql.c_str()));
    }

    uint8_t libpkmn_game_to_hometown(uint8_t game)
    {
        switch(game)
        {
            case Versions::RUBY:
                return Hometowns::RUBY;

            case Versions::SAPPHIRE:
                return Hometowns::SAPPHIRE;

            case Versions::EMERALD:
                return Hometowns::EMERALD;

            case Versions::FIRERED:
                return Hometowns::FIRERED;

            case Versions::LEAFGREEN:
                return Hometowns::LEAFGREEN;

            case Versions::COLOSSEUM:
            case Versions::XD:
                return Hometowns::COLOSSEUM_XD;

            case Versions::DIAMOND:
                return Hometowns::DIAMOND;

            case Versions::PEARL:
                return Hometowns::PEARL;

            case Versions::PLATINUM:
                return Hometowns::PLATINUM;

            case Versions::HEARTGOLD:
                return Hometowns::HEARTGOLD;

            case Versions::SOULSILVER:
                return Hometowns::SOULSILVER;

            case Versions::BLACK:
                return Hometowns::BLACK;

            case Versions::WHITE:
                return Hometowns::WHITE;

            case Versions::BLACK_2:
                return Hometowns::BLACK_2;

            case Versions::WHITE_2:
                return Hometowns::WHITE_2;

            default:
                return Hometowns::RUBY;
        }
    }

    uint8_t hometown_to_libpkmn_game(uint8_t hometown)
    {
        switch(hometown)
        {
            case Hometowns::COLOSSEUM_BONUS:
                return Versions::COLOSSEUM;

            case Hometowns::RUBY:
                return Versions::RUBY;

            case Hometowns::SAPPHIRE:
                return Versions::SAPPHIRE;

            case Hometowns::EMERALD:
                return Versions::EMERALD;

            case Hometowns::FIRERED:
                return Versions::FIRERED;

            case Hometowns::LEAFGREEN:
                return Versions::LEAFGREEN;

            case Hometowns::COLOSSEUM_XD:
                return Versions::XD;

            case Hometowns::DIAMOND:
                return Versions::DIAMOND;

            case Hometowns::PEARL:
                return Versions::PEARL;

            case Hometowns::PLATINUM:
                return Versions::PLATINUM;

            case Hometowns::HEARTGOLD:
                return Versions::HEARTGOLD;

            case Hometowns::SOULSILVER:
                return Versions::SOULSILVER;

            case Hometowns::BLACK:
                return Versions::BLACK;

            case Hometowns::WHITE:
                return Versions::WHITE;

            case Hometowns::BLACK_2:
                return Versions::BLACK_2;

            case Hometowns::WHITE_2:
                return Versions::WHITE_2;

            default:
                return Versions::RUBY;
        }
    }

    uint8_t libpkmn_ball_to_game_ball(uint8_t ball)
    {
        switch(ball)
        {
            case PokeBalls::UNKNOWN:
                return pkmds::Balls::pokeball_;

            case PokeBalls::POKE_BALL:
                return pkmds::Balls::pokeball;

            case PokeBalls::GREAT_BALL:
                return pkmds::Balls::greatball;

            case PokeBalls::ULTRA_BALL:
                return pkmds::Balls::ultraball;

            case PokeBalls::MASTER_BALL:
                return pkmds::Balls::masterball;

            case PokeBalls::SAFARI_BALL:
                return pkmds::Balls::safariball;

            case PokeBalls::LEVEL_BALL:
                return pkmds::Balls::levelball;

            case PokeBalls::LURE_BALL:
                return pkmds::Balls::lureball;

            case PokeBalls::MOON_BALL:
                return pkmds::Balls::moonball;

            case PokeBalls::FRIEND_BALL:
                return pkmds::Balls::friendball;

            case PokeBalls::LOVE_BALL:
                return pkmds::Balls::loveball;

            case PokeBalls::HEAVY_BALL:
                return pkmds::Balls::heavyball;

            case PokeBalls::FAST_BALL:
                return pkmds::Balls::fastball;

            case PokeBalls::SPORT_BALL:
                return pkmds::Balls::sportball;

            case PokeBalls::PREMIER_BALL:
                return pkmds::Balls::premierball;

            case PokeBalls::REPEAT_BALL:
                return pkmds::Balls::repeatball;

            case PokeBalls::TIMER_BALL:
                return pkmds::Balls::timerball;

            case PokeBalls::NEST_BALL:
                return pkmds::Balls::nestball;

            case PokeBalls::NET_BALL:
                return pkmds::Balls::netball;

            case PokeBalls::DIVE_BALL:
                return pkmds::Balls::diveball;

            case PokeBalls::LUXURY_BALL:
                return pkmds::Balls::luxuryball;

            case PokeBalls::HEAL_BALL:
                return pkmds::Balls::healball;

            case PokeBalls::QUICK_BALL:
                return pkmds::Balls::quickball;

            case PokeBalls::DUSK_BALL:
                return pkmds::Balls::duskball;

            case PokeBalls::CHERISH_BALL:
                return pkmds::Balls::cherishball;

            case PokeBalls::PARK_BALL:
                return pkmds::Balls::pokeball; //This should never be set, this ball is for Pal Park

            case PokeBalls::DREAM_BALL:
                return pkmds::Balls::dreamball;

            default:
                return pkmds::Balls::pokeball;
        }
    }

    uint8_t game_ball_to_libpkmn_ball(uint8_t game_ball)
    {
        switch(game_ball)
        {
            case pkmds::Balls::pokeball_:
                return PokeBalls::UNKNOWN;

            case pkmds::Balls::masterball:
                return PokeBalls::MASTER_BALL;

            case pkmds::Balls::ultraball:
                return PokeBalls::ULTRA_BALL;

            case pkmds::Balls::greatball:
                return PokeBalls::GREAT_BALL;

            case pkmds::Balls::pokeball:
                return PokeBalls::POKE_BALL;

            case pkmds::Balls::safariball:
                return PokeBalls::SAFARI_BALL;

            case pkmds::Balls::netball:
                return PokeBalls::NET_BALL;

            case pkmds::Balls::diveball:
                return PokeBalls::DIVE_BALL;

            case pkmds::Balls::nestball:
                return PokeBalls::NEST_BALL;

            case pkmds::Balls::repeatball:
                return PokeBalls::REPEAT_BALL;

            case pkmds::Balls::timerball:
                return PokeBalls::TIMER_BALL;

            case pkmds::Balls::luxuryball:
                return PokeBalls::LUXURY_BALL;

            case pkmds::Balls::premierball:
                return PokeBalls::PREMIER_BALL;

            case pkmds::Balls::duskball:
                return PokeBalls::DUSK_BALL;

            case pkmds::Balls::healball:
                return PokeBalls::HEAL_BALL;

            case pkmds::Balls::quickball:
                return PokeBalls::QUICK_BALL;

            case pkmds::Balls::cherishball:
                return PokeBalls::CHERISH_BALL;

            case pkmds::Balls::fastball:
                return PokeBalls::FAST_BALL;

            case pkmds::Balls::levelball:
                return PokeBalls::LEVEL_BALL;

            case pkmds::Balls::lureball:
                return PokeBalls::LURE_BALL;

            case pkmds::Balls::heavyball:
                return PokeBalls::HEAVY_BALL;

            case pkmds::Balls::loveball:
                return PokeBalls::LOVE_BALL;

            case pkmds::Balls::friendball:
                return PokeBalls::FRIEND_BALL;

            case pkmds::Balls::moonball:
                return PokeBalls::MOON_BALL;

            case pkmds::Balls::sportball:
                return PokeBalls::SPORT_BALL;

            case pkmds::Balls::dreamball:
                return PokeBalls::DREAM_BALL;

            default:
                return PokeBalls::POKE_BALL;
        }
    }
} /* namespace pkmn */
