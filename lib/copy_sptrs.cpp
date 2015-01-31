/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/enums.hpp>
#include <pkmn/database.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include "copy_sptrs.hpp"

#include "bag_impl.hpp"
#include "pocket_impl.hpp"
#include "pokedex_impl.hpp"
#include "pokemon_gen1impl.hpp"
#include "pokemon_gen2impl.hpp"
#include "pokemon_gen3impl.hpp"
#include "pokemon_ndsimpl.hpp"
#include "trainer_impl.hpp"
#include "types/prng_impl.hpp"

namespace pkmn
{
    bag::sptr copy_bag(bag::sptr in)
    {
        bag_impl actual = *pkmn::dynamic_pointer_cast<bag_impl>(in);
        return pkmn::dynamic_pointer_cast<bag>(pkmn::make_shared<bag_impl>(actual));
    }

    pocket::sptr copy_pocket(pocket::sptr in)
    {
        pocket_impl actual = *pkmn::dynamic_pointer_cast<pocket_impl>(in);
        return pkmn::make_shared<pocket_impl>(actual);
    }

    pokedex::sptr copy_pokedex(pokedex::sptr in)
    {
        pokedex_impl actual = *pkmn::dynamic_pointer_cast<pokedex_impl>(in);
        return pkmn::make_shared<pokedex_impl>(actual);
    }

    pokemon::sptr copy_pokemon(pokemon::sptr in)
    {
        switch(in->get_generation())
        {
            case 1:
            {
                pokemon_gen1impl actual = *pkmn::dynamic_pointer_cast<pokemon_gen1impl>(in);
                return pkmn::make_shared<pokemon_gen1impl>(actual);
                break;
            }

            case 2:
            {
                pokemon_gen2impl actual = *pkmn::dynamic_pointer_cast<pokemon_gen2impl>(in);
                return pkmn::make_shared<pokemon_gen2impl>(actual);
                break;
            }

            case 3:
            {
                pokemon_gen3impl actual = *pkmn::dynamic_pointer_cast<pokemon_gen3impl>(in);
                return pkmn::make_shared<pokemon_gen3impl>(actual);
                break;
            }

            default:
            {
                pokemon_ndsimpl actual = *pkmn::dynamic_pointer_cast<pokemon_ndsimpl>(in);
                return pkmn::make_shared<pokemon_ndsimpl>(actual);
                break;
            }
        }
    }

    trainer::sptr copy_trainer(trainer::sptr in)
    {
        trainer_impl actual = *pkmn::dynamic_pointer_cast<trainer_impl>(in);
        return pkmn::make_shared<trainer_impl>(actual);
    }

    prng::sptr copy_prng(prng::sptr in)
    {
        prng_impl actual = *pkmn::dynamic_pointer_cast<prng_impl>(in);
        return pkmn::make_shared<prng_impl>(actual);
    }
}
