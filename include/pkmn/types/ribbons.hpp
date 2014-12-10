/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TYPES_RIBBONS_HPP
#define INCLUDED_PKMN_TYPES_RIBBONS_HPP

#include <cstdint>

#include <pkmn/config.hpp>

namespace pkmn
{
    struct PKMN_API hoenn_ribbons
    {
        hoenn_ribbons() {};
        hoenn_ribbons(const uint32_t ribbonint);

        bool cool;
        bool cool_super;
        bool cool_hyper;
        bool cool_master;
        bool beauty;
        bool beauty_super;
        bool beauty_hyper;
        bool beauty_master;
        bool cute;
        bool cute_super;
        bool cute_hyper;
        bool cute_master;
        bool smart;
        bool smart_super;
        bool smart_hyper;
        bool smart_master;
        bool tough;
        bool tough_super;
        bool tough_hyper;
        bool tough_master;
        bool champion;
        bool winning;
        bool victory;
        bool artist;
        bool effort;
        bool marine;
        bool land;
        bool sky;
        bool country;
        bool national;
        bool earth;
        bool world;

        operator uint32_t();
    };

    struct PKMN_API sinnoh_ribbons1
    {
        sinnoh_ribbons1() {};
        sinnoh_ribbons1(const uint16_t ribbonint);

        bool sinnoh_champ;
        bool ability;
        bool great_ability;
        bool double_ability;
        bool multi_ability;
        bool pair_ability;
        bool world_ability;
        bool alert;
        bool shock;
        bool downcast;
        bool careless;
        bool relax;
        bool snooze;
        bool smile;
        bool gorgeous;
        bool royal;

        operator uint16_t();
    };

    struct PKMN_API sinnoh_ribbons2
    {
        sinnoh_ribbons2() {};
        sinnoh_ribbons2(const uint16_t ribbonint);

        bool gorgeous_royal;
        bool footprint;
        bool record;
        bool history;
        bool legend;
        bool red;
        bool green;
        bool blue;
        bool festival;
        bool carnival;
        bool classic;
        bool premier;

        operator uint16_t();
    };

    struct PKMN_API sinnoh_ribbons3
    {
        sinnoh_ribbons3() {};
        sinnoh_ribbons3(const uint32_t ribbonint);

        bool cool;
        bool cool_super;
        bool cool_hyper;
        bool cool_master;
        bool beauty;
        bool beauty_super;
        bool beauty_hyper;
        bool beauty_master;
        bool cute;
        bool cute_super;
        bool cute_hyper;
        bool cute_master;
        bool smart;
        bool smart_super;
        bool smart_hyper;
        bool smart_master;
        bool tough;
        bool tough_super;
        bool tough_hyper;
        bool tough_master;

        operator uint32_t();
    };

    struct sinnoh_ribbons
    {
        struct sinnoh_ribbons1 ribbons1;
        struct sinnoh_ribbons2 ribbons2;
        struct sinnoh_ribbons3 ribbons3;
    };

    struct PKMN_API unova_ribbons
    {
        unova_ribbons() {};
        unova_ribbons(const uint16_t ribbonint);

        bool gorgeous_royal;
        bool footprint;
        bool record;
        bool event;
        bool legend;
        bool world_champion;
        bool birthday;
        bool special;
        bool souvenir;
        bool wishing;
        bool classic;
        bool premier;

        operator uint16_t();
    };

    struct PKMN_API kalos_ribbons
    {
        kalos_ribbons() {};
        kalos_ribbons(const uint64_t ribbonint);

        bool kalos_champ;
        bool champion;
        bool sinnoh_champ;
        bool best_friends;
        bool training;
        bool skillful_battler;
        bool expert_battler;
        bool effort;
        bool alert;
        bool shock;
        bool downcast;
        bool careless;
        bool relax;
        bool snooze;
        bool smile;
        bool gorgeous;
        bool royal;
        bool gorgeous_royal;
        bool artist;
        bool footprint;
        bool record;
        bool legend;
        bool country;
        bool national;
        bool earth;
        bool world;
        bool classic;
        bool premier;
        bool event;
        bool birthday;
        bool special;
        bool souvenir;
        bool wishing;
        bool battle_champion;
        bool regional_champion;
        bool national_champion;
        bool world_champion;
        bool hoenn_champion;
        bool contest_star;
        bool cool_master;
        bool beauty_master;
        bool cute_master;
        bool clever_master;
        bool tough_master;

        operator uint64_t();
    };

    struct ribbons
    {
        struct hoenn_ribbons  hoenn;
        struct sinnoh_ribbons sinnoh;
        struct unova_ribbons  unova;
        struct kalos_ribbons  kalos;
    };
}

#endif /* INCLUDED_PKMN_TYPES_RIBBONS_HPP */
