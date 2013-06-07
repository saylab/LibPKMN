/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_GEN4IMPL_HPP
#define INCLUDED_GAME_SAVE_GEN4IMPL_HPP

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <pkmnsim/game_save.hpp>
#include <pkmnsim/spec_pkmn.hpp>

#include "PokeLib/pokelib.h"

namespace pkmnsim
{
    /*
     * Game Save Class
     */
    class PKMNSIM_API game_save_gen4impl: public game_save
    {
        public:
            game_save_gen4impl(std::string filename);

        private:
            int save_type;
            PokeLib::Save* gen4_save;

            int verify();

            spec_pkmn::sptr PokeLib_Pokemon_to_spec_pkmn(PokeLib::Pokemon pkmn);
    };
}

#endif /* INCLUDED_GAME_SAVE_GEN4IMPL_HPP */
