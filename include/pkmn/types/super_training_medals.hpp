/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TYPES_SUPER_TRAINING_MEDALS_HPP
#define INCLUDED_PKMN_TYPES_SUPER_TRAINING_MEDALS_HPP

#include <cstdint>

#include <pkmn/config.hpp>

namespace pkmn
{
    struct PKMN_API super_training_medals_t
    {
        super_training_medals_t() {};

        //! Create an instance using the in-game representation.
        super_training_medals_t(uint32_t medalint);

        bool spatk_level1;
        bool hp_level1;
        bool atk_level1;
        bool spdef_level1;
        bool spd_level1;
        bool def_level1;

        bool spatk_level2;
        bool hp_level2;
        bool atk_level2;
        bool spdef_level2;
        bool spd_level2;
        bool def_level2;

        bool spatk_level3;
        bool hp_level3;
        bool atk_level3;
        bool spdef_level3;
        bool spd_level3;
        bool def_level3;

        bool secret_troubles;

        bool secret_leafstone;
        bool secret_firestone;
        bool secret_waterstone;
        bool secret_followgoals;

        bool secret_watchout;
        bool secret_lightningquick;
        bool secret_longshots;

        bool secret_scatterbug;
        bool secret_bitbots;
        bool secret_hydreigon;

        bool secret_battlebest;

        //! Implicit conversion to in-game representation
        operator uint32_t();
    };
}

#endif /* INCLUDED_PKMN_TYPES_SUPER_TRAINING_MEDALS_HPP */
