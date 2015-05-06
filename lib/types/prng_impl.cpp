/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <stdexcept>

#include <boost/assign.hpp>

#include <pkmn/types/prng.hpp>

#include "prng_impl.hpp"

namespace pkmn
{
    prng::sptr prng::make(uint16_t gen)
    {
        if(gen >= 1 and gen <= 6) return prng::sptr(new prng_impl(gen));
        else throw std::runtime_error("Invalid generation.");
    }

    prng_impl::prng_impl(uint16_t gen)
    {
        _gen = gen;

        srand((unsigned int)time(NULL));
        seed_lcrng(rand());
        seed_arng(rand());
        reset_mtrng();
    }

    uint16_t prng_impl::lcrng()
    {
        _lcrng_seed = ((0x41C64E6D * _lcrng_seed) + 0x6073) & 0xFFFFFFFF;
        return uint16_t(_lcrng_seed >> 16);
    }

    uint32_t prng_impl::arng()
    {
        _arng_seed = (0x6C078965 * _arng_seed) + 0x1;
        return _arng_seed;
    }

    void prng_impl::reset_mtrng()
    {
        for(int i = 0; i < 624; i++) _mtrng.nums[i] = rand();
        _mtrng.pos = 0;
    }

    uint32_t prng_impl::mtrng()
    {
        uint32_t result = _mtrng.nums[_mtrng.pos];

        if(_mtrng.pos == 623) reset_mtrng();
        else _mtrng.pos++;

        return result;
    }
} /* namespace pkmn */
