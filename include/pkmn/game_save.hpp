/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_GAME_SAVE_HPP
#define INCLUDED_PKMN_GAME_SAVE_HPP

#include <vector>

#include <pkmn/config.hpp>
#include <pkmn/trainer.hpp>
#include <pkmn/types/shared_ptr.hpp>

namespace pkmn
{
    class PKMN_API game_save
    {
        public:

            typedef pkmn::shared_ptr<game_save> sptr;
            static sptr make(const pkmn::pkstring &filename);

            //Class constructors (should never be called directly)
            game_save() {};
            virtual ~game_save() {};

            virtual pkmn::pkstring get_game() const = 0;
            virtual uint16_t get_generation() const = 0;

            virtual void load() = 0;
            virtual void save() = 0;
            virtual void save_as(const pkmn::pkstring &filename) = 0;
            //virtual bool check() = 0;

            virtual trainer::sptr get_trainer() const = 0;

            virtual const pokemon_team_t& get_trainer_party() const = 0;
            virtual void set_trainer_party(const pokemon_team_t &party) = 0;

            virtual pkmn::pkstring get_trainer_name() const = 0;
            virtual void set_trainer_name(const pkmn::pkstring &trainer_name) = 0;

            virtual uint16_t get_game_id() const = 0;            
    };
}
#endif /* INCLUDED_PKMN_GAME_SAVE_HPP */

