/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_COPY_SPTRS_HPP
#define INCLUDED_COPY_SPTRS_HPP

#include <pkmn/config.hpp>

#include <pkmn/bag.hpp>
#include <pkmn/base_pokemon.hpp>
#include <pkmn/item.hpp>
#include <pkmn/move.hpp>
#include <pkmn/pocket.hpp>
#include <pkmn/trainer.hpp>

namespace pkmn
{
    bag::sptr PKMN_API copy_bag(bag::sptr in);

    base_pokemon::sptr PKMN_API copy_base_pokemon(base_pokemon::sptr in);

    item::sptr PKMN_API copy_item(item::sptr in);

    move::sptr PKMN_API copy_move(move::sptr in);

    pocket::sptr PKMN_API copy_pocket(pocket::sptr in);

    team_pokemon::sptr PKMN_API copy_team_pokemon(team_pokemon::sptr in);

    trainer::sptr PKMN_API copy_trainer(trainer::sptr in);
}

#endif /* INCLUDED_COPY_SPTRS_HPP */
