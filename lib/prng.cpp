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

#include <pkmn/types/dict.hpp>
#include <pkmn/types/prng.hpp>

namespace pkmn
{
    struct mtrng
    {
        mtrng()
        {
            srand(time(NULL));
            for(size_t i = 0; i < 624; i++) nums[i] = rand();
        }

        uint16_t pos;
        uint32_t nums[624];
    };

    static mtrng pkmn_mtrng;

    //TODO: Check how Gen VI PRNG works
    pkmn::dict<uint8_t, prng::sptr> prng::_prngs = boost::assign::map_list_of
        (1, prng::sptr(new prng(1)))
        (2, prng::sptr(new prng(2)))
        (3, prng::sptr(new prng(3)))
        (4, prng::sptr(new prng(4)))
        (5, prng::sptr(new prng(5)))
        (6, prng::sptr(new prng(6)))
    ;

    static pkmn::dict<uint8_t, uint32_t> lcrng_seeds = boost::assign::map_list_of
        (1, rand())
        (2, rand())
        (3, rand())
        (4, rand())
        (5, rand())
        (6, rand())
    ;

    static uint32_t arng_seed = rand();

    prng::sptr prng::get(uint8_t gen)
    {
        if(_prngs.has_key(gen)) return _prngs[gen];
        else throw std::runtime_error("Invalid generation.");
    }

    prng::prng(uint8_t gen)
    {
        _gen = gen;
    }

    uint64_t prng::lcrng()
    {
        if(_gen < 5)
        {
            lcrng_seeds[_gen] = (0x41C64E6D * lcrng_seeds[_gen]) + 0x6073;
            return lcrng_seeds[_gen];
        }
        else
        {
            lcrng_seeds[_gen] = (0x5D588B656C078965 * lcrng_seeds[_gen]) + 0x0000000000269EC3;
            return lcrng_seeds[_gen];
        }
    }

    uint32_t prng::arng()
    {
        arng_seed = (0x6C078965 * arng_seed) + 0x1;
        return arng_seed;
    }

    uint32_t prng::mtrng()
    {
        uint32_t result = pkmn_mtrng.nums[pkmn_mtrng.pos];

        if(pkmn_mtrng.pos == 623)
        {
            for(int i = 0; i < 624; i++) pkmn_mtrng.nums[i] = rand();
            pkmn_mtrng.pos = 0;
        }
        else pkmn_mtrng.pos++;

        return result;
    }
} /* namespace pkmn */
