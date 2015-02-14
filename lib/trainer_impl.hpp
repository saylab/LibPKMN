/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
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

            trainer_impl(uint16_t game, const pkmn::pkstring& name, uint16_t gender);

            //Game-specific Info
            pkmn::pkstring get_game() const;
            uint16_t get_generation() const;

            //Get attributes
            pkmn::pkstring get_name() const;
            uint32_t get_money() const;
            pkmn::pkstring get_gender() const;
            uint32_t get_id() const;
            uint16_t get_public_id() const;
            uint16_t get_secret_id() const;

            //Set attributes
            void set_name(const pkmn::pkstring& name);
            void set_money(uint32_t money);
            void set_gender(const pkmn::pkstring& gender);
            void set_id(uint32_t id);
            void set_public_id(uint16_t id);
            void set_secret_id(uint16_t id);

            //Pokemon
            pokemon::sptr get_pokemon(uint16_t pos);
            void set_pokemon(uint16_t pos, pokemon::sptr pkmn);
            void remove_pokemon(uint16_t pos);
            void get_party(pokemon_team_t& party);
            void set_party(pokemon_team_t& party);

            //Bag
            bag::sptr get_bag() const;

            //Database Info
            uint16_t get_game_id() const;

        protected:

            uint16_t _money, _game_id, _gender_id, _generation;

            union
            {
                struct
                {
                    uint16_t public_id;
                    uint16_t secret_id;
                } _tid;
                uint32_t _trainer_id;
            };

            bag::sptr _bag;
            pkmn::pkstring _trainer_name;
            pokemon_team_t _party;

            std::string _sprite_path;
    };
}

#endif /* INCLUDED_TRAINER_IMPL_HPP */
