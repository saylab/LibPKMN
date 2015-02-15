/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
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
    /*!
     * This set of booleans represents the <a href="http://bulbapedia.bulbagarden.net/wiki/Ribbon">ribbons</a>
     * available in the Hoenn region (Generation III, present in Generation III-V native representations).
     */
    struct PKMN_API hoenn_ribbons_t
    {
        hoenn_ribbons_t() {};

        //! Create an instance using the in-game representation.
        hoenn_ribbons_t(uint32_t ribbonint);

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

        //! Implicit conversion to in-game representation
        operator uint32_t();
    };

    /*!
     * This set of booleans represents the first set of
     * <a href="http://bulbapedia.bulbagarden.net/wiki/Ribbon">ribbons</a> available
     * in the Sinnoh region. This separation corresponds to how the ribbons are
     * stored in-game (Generations IV-VI).
     */
    struct PKMN_API sinnoh_ribbons1_t
    {
        sinnoh_ribbons1_t() {};

        //! Create an instance using the in-game representation.
        sinnoh_ribbons1_t(uint16_t ribbonint);

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

        //! Implicit conversion to in-game representation
        operator uint16_t();
    };

    /*!
     * This set of booleans represents the second set of
     * <a href="http://bulbapedia.bulbagarden.net/wiki/Ribbon">ribbons</a> available
     * in the Sinnoh region. This separation corresponds to how the ribbons are
     * stored in-game (Generations IV).
     *
     * NOTE: In Generations V-VI, this is replaced by the Unova ribbons.
     */
    struct PKMN_API sinnoh_ribbons2_t
    {
        sinnoh_ribbons2_t() {};

        //! Create an instance using the in-game representation.
        sinnoh_ribbons2_t(uint16_t ribbonint);

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

        //! Implicit conversion to in-game representation
        operator uint16_t();
    };

    /*!
     * This set of booleans represents the third set of
     * <a href="http://bulbapedia.bulbagarden.net/wiki/Ribbon">ribbons</a> available
     * in the Sinnoh region. This separation corresponds to how the ribbons are
     * stored in-game (Generations IV-VI).
     */
    struct PKMN_API sinnoh_ribbons3_t
    {
        sinnoh_ribbons3_t() {};

        //! Create an instance using the in-game representation.
        sinnoh_ribbons3_t(uint32_t ribbonint);

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

        //! Implicit conversion to in-game representation
        operator uint32_t();
    };

    /*!
     * This struct holds all of a Pokémon's Sinnoh ribbons.
     */
    struct sinnoh_ribbons_t
    {
        struct sinnoh_ribbons1_t ribbons1;
        struct sinnoh_ribbons2_t ribbons2;
        struct sinnoh_ribbons3_t ribbons3;
    };

    /*!
     * This set of booleans the set of ribbons available in the Unova
     * region.
     */
    struct PKMN_API unova_ribbons_t
    {
        unova_ribbons_t() {};

        //! Create an instance using the in-game representation.
        unova_ribbons_t(uint16_t ribbonint);

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

        //! Implicit conversion to in-game representation
        operator uint16_t();
    };

    /*!
     * This set of booleans the set of ribbons available in the Kalos
     * region.
     */
    struct PKMN_API kalos_ribbons_t
    {
        kalos_ribbons_t() {};

        //! Create an instance using the in-game representation.
        kalos_ribbons_t(uint64_t ribbonint);

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

        //! Implicit conversion to in-game representation
        operator uint64_t();
    };

    /*!
     * This struct holds all of a Pokémon's possible ribbons (accurate up
     * to Generation VI).
     */
    struct ribbons_t
    {
        hoenn_ribbons_t  hoenn;
        sinnoh_ribbons_t sinnoh;
        unova_ribbons_t  unova;
        kalos_ribbons_t  kalos;
    };
}

#endif /* INCLUDED_PKMN_TYPES_RIBBONS_HPP */
