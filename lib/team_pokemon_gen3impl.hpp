/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TEAM_POKEMON_GEN3IMPL_HPP
#define INCLUDED_TEAM_POKEMON_GEN3IMPL_HPP

#include "team_pokemon_impl.hpp"
#include "Signal.h"

#include "conversions/structs/pokemon.hpp"

namespace pkmn
{
    class team_pokemon_gen3impl: public team_pokemon_impl
    {
        public:

            team_pokemon_gen3impl(base_pokemon::sptr base, uint8_t level,
                                  uint8_t move1, uint8_t move2,
                                  uint8_t move3, uint8_t move4);
            team_pokemon_gen3impl(const team_pokemon_gen3impl &other);
            team_pokemon_gen3impl& operator=(const team_pokemon_gen3impl &other);
            ~team_pokemon_gen3impl() {};

            //Getting Trainer Info
            pkmn::pkstring get_nickname() const;
            pkmn::pkstring get_trainer_name() const;
            pkmn::pkstring get_trainer_gender() const;
            unsigned int get_trainer_id() const;
            unsigned short get_trainer_public_id() const;
            unsigned short get_trainer_secret_id() const;
            pkmn::pkstring get_ball() const;
            unsigned int get_met_level() const;

            //Setting Trainer Info
            void set_original_game(unsigned int game);
            void set_nickname(const pkmn::pkstring &nickname);
            void set_trainer_name(const pkmn::pkstring &trainer_name);
            void set_trainer_gender(const pkmn::pkstring &gender);
            void set_trainer_id(unsigned int id);
            void set_trainer_public_id(unsigned short id);
            void set_trainer_secret_id(unsigned short id);
            void set_ball(const pkmn::pkstring &ball);
            void set_met_level(unsigned int level);

            //Getting Individual Stat Info
            unsigned int get_personality() const;
            unsigned int get_level() const;
            unsigned int get_experience() const;
            pkmn::pkstring get_gender() const;
            pkmn::nature_t get_nature() const;
            pkmn::pkstring get_ability() const;
            bool using_hidden_ability() const;
            bool is_shiny() const;
            pkmn::dict<pkmn::pkstring, unsigned int> get_stats() const;
            pkmn::dict<pkmn::pkstring, unsigned int> get_EVs() const;
            pkmn::dict<pkmn::pkstring, unsigned int> get_IVs() const;
            
            //Setting Individual Stat Info
            void set_personality(unsigned int personality);
            void set_level(unsigned int level);
            void set_experience(unsigned int experience);
            void set_nature(const pkmn::pkstring &nature_name);
            void set_ability(const pkmn::pkstring &ability);
            void set_using_hidden_ability(bool value);
            void set_EV(const pkmn::pkstring &stat_name, unsigned int stat);
            void set_IV(const pkmn::pkstring &stat_name, unsigned int stat);

            //Battle Stat Info
            pkmn::pkstring get_status() const;
            item::sptr get_held_item() const;
            void set_status(const pkmn::pkstring &status);
            void set_held_item(const pkmn::pkstring &item_name);

            //Getting Move Info
            move::sptr get_move(unsigned int pos) const;
            void get_moves(pkmn::moveset_t &moves) const;
            unsigned int get_move_PP(unsigned int pos) const;
            void get_move_PPs(std::vector<unsigned int> &move_PPs) const;

            //Setting Move Info
            void set_move(const pkmn::pkstring &move_name, unsigned int pos);
            void set_move_PP(unsigned int PP, unsigned int pos);

            //Misc
            pkmn::markings get_markings() const;
            void set_markings(const pkmn::markings &mark);
            pkmn::ribbons get_ribbons() const;
            void set_ribbons(const pkmn::ribbons &rib);

            //Database Info
            unsigned int get_original_game_id() const;
            unsigned int get_ability_id() const;
            unsigned int get_item_id() const;
            unsigned int get_nature_id() const;

            void* get_native();

        private:

            friend class base_pokemon_modernimpl;

            gen3_party_pokemon_t    _raw;
            gen3_pokemon_growth_t*  _growth;
            gen3_pokemon_attacks_t* _attacks;
            gen3_pokemon_effort_t*  _effort;
            gen3_pokemon_misc_t*    _misc;

            void _set_experience(const uint32_t exp);
            void _set_level(const uint8_t level);
            void _set_stats();
    };
}

#endif /* INCLUDED_TEAM_POKEMON_GEN3IMPL_HPP */
