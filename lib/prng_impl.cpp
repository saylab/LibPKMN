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

        srand(time(NULL));
        seed_lcrng(rand());
        seed_arng(rand());
    }

    uint64_t prng_impl::lcrng()
    {
        if(_gen < 5)
        {
            _lcrng_seed = ((0x41C64E6D * _lcrng_seed) + 0x6073) % 4294967295;
            return _lcrng_seed;
        }
        else
        {
            _lcrng_seed = (0x5D588B656C078965 * _lcrng_seed) + 0x0000000000269EC3;
            return _lcrng_seed;
        }
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
