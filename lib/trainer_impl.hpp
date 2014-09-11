/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TRAINER_IMPL_HPP
#define INCLUDED_TRAINER_IMPL_HPP

#include <string>
#include <vector>

#include <pkmn/trainer.hpp>

namespace pkmn
{
    class trainer_impl: public trainer
    {
        public:

            trainer_impl(unsigned int game, const pkmn::pkstring &name, unsigned int gender);

            //Game-specific Info
            pkmn::pkstring get_game() const;
            unsigned int get_generation() const;

            //Get attributes
            pkmn::pkstring get_name() const;
            unsigned int get_money() const;
            pkmn::pkstring get_gender() const;
            unsigned int get_id() const;
            unsigned short get_public_id() const;
            unsigned short get_secret_id() const;

            //Set attributes
            void set_name(const pkmn::pkstring &name);
            void set_money(unsigned int money);
            void set_gender(const pkmn::pkstring &gender);
            void set_id(unsigned int id);
            void set_public_id(unsigned short id);
            void set_secret_id(unsigned short id);

            //Pokemon
            team_pokemon::sptr get_pokemon(unsigned int pos);
            void set_pokemon(unsigned int pos, team_pokemon::sptr t_pkmn);
            void remove_pokemon(unsigned int pos);
            void get_party(pokemon_team_t &party);
            void set_party(pokemon_team_t &party);

            //Bag
            bag::sptr get_bag() const;

            //Database Info
            unsigned int get_game_id() const;

        protected:

            unsigned int _money, _game_id, _gender_id, _generation;

            union
            {
                struct
                {
                    unsigned short public_id;
                    unsigned short secret_id;
                } _tid;
                unsigned int _trainer_id;
            };

            bag::sptr _bag;
            pkmn::pkstring _trainer_name;
            pokemon_team_t _party;

            std::string _sprite_path;
    };
}

#endif /* INCLUDED_TRAINER_IMPL_HPP */
