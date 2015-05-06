/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifdef _MSC_VER
#pragma warning(disable: 4800) // forcing value to bool 'true' or 'false'
#endif

#include <pkmn/types/ribbons.hpp>

#include <cstdint>

namespace pkmn
{
    hoenn_ribbons_t::hoenn_ribbons_t(const uint32_t ribbonint)
    {
        cool          = ribbonint & 0x1;
        cool_super    = ribbonint & 0x2;
        cool_hyper    = ribbonint & 0x4;
        cool_master   = ribbonint & 0x8;
        beauty        = ribbonint & 0x10;
        beauty_super  = ribbonint & 0x20;
        beauty_hyper  = ribbonint & 0x40;
        beauty_master = ribbonint & 0x80;
        cute          = ribbonint & 0x100;
        cute_super    = ribbonint & 0x200;
        cute_hyper    = ribbonint & 0x400;
        cute_master   = ribbonint & 0x800;
        smart         = ribbonint & 0x1000;
        smart_super   = ribbonint & 0x2000;
        smart_hyper   = ribbonint & 0x4000;
        smart_master  = ribbonint & 0x8000;
        tough         = ribbonint & 0x10000;
        tough_super   = ribbonint & 0x20000;
        tough_hyper   = ribbonint & 0x40000;
        tough_master  = ribbonint & 0x80000;
        champion      = ribbonint & 0x100000;
        winning       = ribbonint & 0x200000;
        victory       = ribbonint & 0x400000;
        artist        = ribbonint & 0x800000;
        effort        = ribbonint & 0x1000000;
        marine        = ribbonint & 0x2000000;
        land          = ribbonint & 0x4000000;
        sky           = ribbonint & 0x8000000;
        country       = ribbonint & 0x10000000;
        national      = ribbonint & 0x20000000;
        earth         = ribbonint & 0x40000000;
        world         = ribbonint & 0x80000000;
    }

    hoenn_ribbons_t::operator uint32_t()
    {
        uint32_t ribbonint = 0;

        if(cool)          ribbonint |= 0x1;
        if(cool_super)    ribbonint |= 0x2;
        if(cool_hyper)    ribbonint |= 0x4;
        if(cool_master)   ribbonint |= 0x8;
        if(beauty)        ribbonint |= 0x10;
        if(beauty_super)  ribbonint |= 0x20;
        if(beauty_hyper)  ribbonint |= 0x40;
        if(beauty_master) ribbonint |= 0x80;
        if(cute)          ribbonint |= 0x100;
        if(cute_super)    ribbonint |= 0x200;
        if(cute_hyper)    ribbonint |= 0x400;
        if(cute_master)   ribbonint |= 0x800;
        if(smart)         ribbonint |= 0x1000;
        if(smart_super)   ribbonint |= 0x2000;
        if(smart_hyper)   ribbonint |= 0x4000;
        if(smart_master)  ribbonint |= 0x8000;
        if(tough)         ribbonint |= 0x10000;
        if(tough_super)   ribbonint |= 0x20000;
        if(tough_hyper)   ribbonint |= 0x40000;
        if(tough_master)  ribbonint |= 0x80000;
        if(champion)      ribbonint |= 0x100000;
        if(winning)       ribbonint |= 0x200000;
        if(victory)       ribbonint |= 0x400000;
        if(artist)        ribbonint |= 0x800000;
        if(effort)        ribbonint |= 0x1000000;
        if(marine)        ribbonint |= 0x2000000;
        if(land)          ribbonint |= 0x4000000;
        if(sky)           ribbonint |= 0x8000000;
        if(country)       ribbonint |= 0x10000000;
        if(national)      ribbonint |= 0x20000000;
        if(earth)         ribbonint |= 0x40000000;
        if(world)         ribbonint |= 0x80000000;

        return ribbonint;
    }

    sinnoh_ribbons1_t::sinnoh_ribbons1_t(const uint16_t ribbonint)
    {
        sinnoh_champ   = ribbonint & 0x1;
        ability        = ribbonint & 0x2;
        great_ability  = ribbonint & 0x4;
        double_ability = ribbonint & 0x8;
        multi_ability  = ribbonint & 0x10;
        pair_ability   = ribbonint & 0x20;
        world_ability  = ribbonint & 0x40;
        alert          = ribbonint & 0x80;
        shock          = ribbonint & 0x100;
        downcast       = ribbonint & 0x200;
        careless       = ribbonint & 0x400;
        relax          = ribbonint & 0x800;
        snooze         = ribbonint & 0x1000;
        smile          = ribbonint & 0x2000;
        gorgeous       = ribbonint & 0x4000;
        royal          = ribbonint & 0x8000;
    }

    sinnoh_ribbons1_t::operator uint16_t()
    {
        uint16_t ribbonint = 0;

        if(sinnoh_champ)   ribbonint |= 0x1;
        if(ability)        ribbonint |= 0x2;
        if(great_ability)  ribbonint |= 0x4;
        if(double_ability) ribbonint |= 0x8;
        if(multi_ability)  ribbonint |= 0x10;
        if(pair_ability)   ribbonint |= 0x20;
        if(world_ability)  ribbonint |= 0x40;
        if(alert)          ribbonint |= 0x80;
        if(shock)          ribbonint |= 0x100;
        if(downcast)       ribbonint |= 0x200;
        if(careless)       ribbonint |= 0x400;
        if(relax)          ribbonint |= 0x800;
        if(snooze)         ribbonint |= 0x1000;
        if(smile)          ribbonint |= 0x2000;
        if(gorgeous)       ribbonint |= 0x4000;
        if(royal)          ribbonint |= 0x8000;

        return ribbonint;
    }

    sinnoh_ribbons2_t::sinnoh_ribbons2_t(const uint16_t ribbonint)
    {
        gorgeous_royal = ribbonint & 0x1;
        footprint      = ribbonint & 0x2;
        record         = ribbonint & 0x4;
        history        = ribbonint & 0x8;
        legend         = ribbonint & 0x10;
        red            = ribbonint & 0x20;
        green          = ribbonint & 0x40;
        blue           = ribbonint & 0x80;
        festival       = ribbonint & 0x100;
        carnival       = ribbonint & 0x200;
        classic        = ribbonint & 0x400;
        premier        = ribbonint & 0x800;
    }

    sinnoh_ribbons2_t::operator uint16_t()
    {
        uint16_t ribbonint = 0;

        if(gorgeous_royal) ribbonint |= 0x1;
        if(footprint)      ribbonint |= 0x2;
        if(record)         ribbonint |= 0x4;
        if(history)        ribbonint |= 0x8;
        if(legend)         ribbonint |= 0x10;
        if(red)            ribbonint |= 0x20;
        if(green)          ribbonint |= 0x40;
        if(blue)           ribbonint |= 0x80;
        if(festival)       ribbonint |= 0x100;
        if(carnival)       ribbonint |= 0x200;
        if(classic)        ribbonint |= 0x400;
        if(premier)        ribbonint |= 0x800;

        return ribbonint;
    }

    sinnoh_ribbons3_t::sinnoh_ribbons3_t(const uint32_t ribbonint)
    {
        cool          = ribbonint & 0x1;
        cool_super    = ribbonint & 0x2;
        cool_hyper    = ribbonint & 0x4;
        cool_master   = ribbonint & 0x8;
        beauty        = ribbonint & 0x10;
        beauty_super  = ribbonint & 0x20;
        beauty_hyper  = ribbonint & 0x40;
        beauty_master = ribbonint & 0x80;
        cute          = ribbonint & 0x100;
        cute_super    = ribbonint & 0x200;
        cute_hyper    = ribbonint & 0x400;
        cute_master   = ribbonint & 0x800;
        smart         = ribbonint & 0x1000;
        smart_super   = ribbonint & 0x2000;
        smart_hyper   = ribbonint & 0x4000;
        smart_master  = ribbonint & 0x8000;
        tough         = ribbonint & 0x10000;
        tough_super   = ribbonint & 0x20000;
        tough_hyper   = ribbonint & 0x40000;
        tough_master  = ribbonint & 0x80000;
    }

    sinnoh_ribbons3_t::operator uint32_t()
    {
        uint32_t ribbonint = 0;

        if(cool)          ribbonint |= 0x1;
        if(cool_super)    ribbonint |= 0x2;
        if(cool_hyper)    ribbonint |= 0x4;
        if(cool_master)   ribbonint |= 0x8;
        if(beauty)        ribbonint |= 0x10;
        if(beauty_super)  ribbonint |= 0x20;
        if(beauty_hyper)  ribbonint |= 0x40;
        if(beauty_master) ribbonint |= 0x80;
        if(cute)          ribbonint |= 0x100;
        if(cute_super)    ribbonint |= 0x200;
        if(cute_hyper)    ribbonint |= 0x400;
        if(cute_master)   ribbonint |= 0x800;
        if(smart)         ribbonint |= 0x1000;
        if(smart_super)   ribbonint |= 0x2000;
        if(smart_hyper)   ribbonint |= 0x4000;
        if(smart_master)  ribbonint |= 0x8000;
        if(tough)         ribbonint |= 0x10000;
        if(tough_super)   ribbonint |= 0x20000;
        if(tough_hyper)   ribbonint |= 0x40000;
        if(tough_master)  ribbonint |= 0x80000;

        return ribbonint;
    }

    unova_ribbons_t::unova_ribbons_t(const uint16_t ribbonint)
    {
        gorgeous_royal = ribbonint & 0x1;
        footprint      = ribbonint & 0x2;
        record         = ribbonint & 0x4;
        event          = ribbonint & 0x8;
        legend         = ribbonint & 0x10;
        world_champion = ribbonint & 0x20;
        birthday       = ribbonint & 0x40;
        special        = ribbonint & 0x80;
        souvenir       = ribbonint & 0x100;
        wishing        = ribbonint & 0x200;
        classic        = ribbonint & 0x400;
        premier        = ribbonint & 0x800;
    }

    unova_ribbons_t::operator uint16_t()
    {
        uint16_t ribbonint = 0;

        if(gorgeous_royal) ribbonint |= 0x1;
        if(footprint)      ribbonint |= 0x2;
        if(record)         ribbonint |= 0x4;
        if(event)          ribbonint |= 0x8;
        if(legend)         ribbonint |= 0x10;
        if(world_champion) ribbonint |= 0x20;
        if(birthday)       ribbonint |= 0x40;
        if(special)        ribbonint |= 0x80;
        if(souvenir)       ribbonint |= 0x100;
        if(wishing)        ribbonint |= 0x200;
        if(classic)        ribbonint |= 0x400;
        if(premier)        ribbonint |= 0x800;

        return ribbonint;
    }

    kalos_ribbons_t::kalos_ribbons_t(const uint64_t ribbonint)
    {
        kalos_champ       = ribbonint & 0x1;
        champion          = ribbonint & 0x2;
        sinnoh_champ      = ribbonint & 0x4;
        best_friends      = ribbonint & 0x8;
        training          = ribbonint & 0x10;
        skillful_battler  = ribbonint & 0x20;
        expert_battler    = ribbonint & 0x40;
        effort            = ribbonint & 0x80;
        alert             = ribbonint & 0x100;
        shock             = ribbonint & 0x200;
        downcast          = ribbonint & 0x400;
        careless          = ribbonint & 0x800;
        relax             = ribbonint & 0x1000;
        snooze            = ribbonint & 0x2000;
        smile             = ribbonint & 0x4000;
        gorgeous          = ribbonint & 0x8000;
        royal             = ribbonint & 0x10000;
        gorgeous_royal    = ribbonint & 0x20000;
        artist            = ribbonint & 0x40000;
        footprint         = ribbonint & 0x80000;
        record            = ribbonint & 0x100000;
        legend            = ribbonint & 0x200000;
        country           = ribbonint & 0x400000;
        national          = ribbonint & 0x800000;
        earth             = ribbonint & 0x1000000;
        world             = ribbonint & 0x2000000;
        classic           = ribbonint & 0x4000000;
        premier           = ribbonint & 0x8000000;
        event             = ribbonint & 0x10000000;
        birthday          = ribbonint & 0x20000000;
        special           = ribbonint & 0x40000000;
        souvenir          = ribbonint & 0x80000000;
        wishing           = ribbonint & 0x100000000;
        battle_champion   = ribbonint & 0x200000000;
        regional_champion = ribbonint & 0x400000000;
        national_champion = ribbonint & 0x1000000000;
        world_champion    = ribbonint & 0x2000000000;
        hoenn_champion    = ribbonint & 0x10000000000;
        contest_star      = ribbonint & 0x20000000000;
        cool_master       = ribbonint & 0x40000000000;
        beauty_master     = ribbonint & 0x80000000000;
        cute_master       = ribbonint & 0x100000000000;
        clever_master     = ribbonint & 0x200000000000;
        tough_master      = ribbonint & 0x400000000000;
    }

    kalos_ribbons_t::operator uint64_t()
    {
        uint64_t ribbonint = 0;

        if(kalos_champ)       ribbonint |= 0x1;
        if(champion)          ribbonint |= 0x2;
        if(sinnoh_champ)      ribbonint |= 0x4;
        if(best_friends)      ribbonint |= 0x8;
        if(training)          ribbonint |= 0x10;
        if(skillful_battler)  ribbonint |= 0x20;
        if(expert_battler)    ribbonint |= 0x40;
        if(effort)            ribbonint |= 0x80;
        if(alert)             ribbonint |= 0x100;
        if(shock)             ribbonint |= 0x200;
        if(downcast)          ribbonint |= 0x400;
        if(careless)          ribbonint |= 0x800;
        if(relax)             ribbonint |= 0x1000;
        if(snooze)            ribbonint |= 0x2000;
        if(smile)             ribbonint |= 0x4000;
        if(gorgeous)          ribbonint |= 0x8000;
        if(royal)             ribbonint |= 0x10000;
        if(gorgeous_royal)    ribbonint |= 0x20000;
        if(artist)            ribbonint |= 0x40000;
        if(footprint)         ribbonint |= 0x80000;
        if(record)            ribbonint |= 0x100000;
        if(legend)            ribbonint |= 0x200000;
        if(country)           ribbonint |= 0x400000;
        if(national)          ribbonint |= 0x800000;
        if(earth)             ribbonint |= 0x1000000;
        if(world)             ribbonint |= 0x2000000;
        if(classic)           ribbonint |= 0x4000000;
        if(premier)           ribbonint |= 0x8000000;
        if(event)             ribbonint |= 0x10000000;
        if(birthday)          ribbonint |= 0x20000000;
        if(special)           ribbonint |= 0x40000000;
        if(souvenir)          ribbonint |= 0x80000000;
        if(wishing)           ribbonint |= 0x100000000;
        if(battle_champion)   ribbonint |= 0x200000000;
        if(regional_champion) ribbonint |= 0x400000000;
        if(national_champion) ribbonint |= 0x1000000000;
        if(world_champion)    ribbonint |= 0x2000000000;
        if(hoenn_champion)    ribbonint |= 0x10000000000;
        if(contest_star)      ribbonint |= 0x20000000000;
        if(cool_master)       ribbonint |= 0x40000000000;
        if(beauty_master)     ribbonint |= 0x80000000000;
        if(cute_master)       ribbonint |= 0x100000000000;
        if(clever_master)     ribbonint |= 0x200000000000;
        if(tough_master)      ribbonint |= 0x400000000000;

        return ribbonint;
    }
}
