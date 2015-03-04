/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PRNG_IMPL_HPP
#define INCLUDED_PRNG_IMPL_HPP

#include <climits>
#include <cstdint>
#include <ctime>
#include <iostream>

#include <pkmn/config.hpp>

#include <pkmn/types/prng.hpp>

namespace pkmn
{
    struct mtrng
    {   
        mtrng()
        {
            srand((unsigned int)time(NULL));
            for(size_t i = 0; i < 624; i++) nums[i] = uint32_t(rand());
            pos = 0;
        }

        uint16_t pos;
        uint32_t nums[624];
    };

    class prng_impl: public prng
    {
        public:

            prng_impl();
            prng_impl(uint16_t gen);
            ~prng_impl() {};

            void PKMN_INLINE seed_lcrng(const uint32_t seed)
            {
                _lcrng_seed = seed;
            }

            uint16_t lcrng();

            void PKMN_INLINE seed_arng(const uint32_t seed)
            {
                _arng_seed = seed;
            }

            uint32_t arng();

            void reset_mtrng();
            uint32_t mtrng();

        private:

            uint32_t _gen;
            uint32_t _lcrng_seed;
            uint32_t _arng_seed;
            struct mtrng _mtrng;
    };
}

#endif /* INCLUDED_PRNG_IMPL_HPP */
