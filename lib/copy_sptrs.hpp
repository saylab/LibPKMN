/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_COPY_SPTRS_HPP
#define INCLUDED_COPY_SPTRS_HPP

#include <pkmn/config.hpp>

#include <pkmn/pocket.hpp>
#include <pkmn/pokedex.hpp>
#include <pkmn/trainer.hpp>
#include <pkmn/types/prng.hpp>

namespace pkmn
{
    bag::sptr PKMN_API copy_bag(bag::sptr in);

    pocket::sptr PKMN_API copy_pocket(pocket::sptr in);

    pokedex::sptr PKMN_API copy_pokedex(pokedex::sptr in);

    pokemon::sptr PKMN_API copy_pokemon(pokemon::sptr in);

    trainer::sptr PKMN_API copy_trainer(trainer::sptr in);

    prng::sptr PKMN_API copy_prng(prng::sptr in);
}

#endif /* INCLUDED_COPY_SPTRS_HPP */
