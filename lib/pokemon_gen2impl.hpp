/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_POKEMON_GEN2IMPL_HPP
#define INCLUDED_POKEMON_GEN2IMPL_HPP

#include <pkmn/native/pokemon.hpp>

#include "pokemon_impl.hpp"

namespace pkmn
{
    class pokemon_gen2impl: public pokemon_impl
    {
        public:

            pokemon_gen2impl(int species, int version,
                             int level,
                             int move1, int move2,
                             int move3, int move4);
            pokemon_gen2impl(const pkmn::native::gen2_pc_pokemon_t &raw,
                             int version);
            pokemon_gen2impl(const pkmn::native::gen2_pc_pokemon_t &raw,
                             const pkmn::pkstring &nickname,
                             const pkmn::pkstring &otname,
                             int version);
            pokemon_gen2impl(const pkmn::native::gen2_party_pokemon_t &raw,
                             int version);
            pokemon_gen2impl(const pkmn::native::gen2_party_pokemon_t &raw,
                             const pkmn::pkstring &nickname,
                             const pkmn::pkstring &otname,
                             int version);
            pokemon_gen2impl(const pokemon_gen2impl &other);
            pokemon_gen2impl &operator=(const pokemon_gen2impl &other);
            ~pokemon_gen2impl() {};

            // Getting Non-battle Info
            pkmn::contest_stats_t get_contest_stats() const;
            pkmn::markings_t get_markings() const;
            pkmn::ribbons_t get_ribbons() const;
            pkmn::super_training_medals_t get_super_training_medals() const;
            pkmn::pokerus_t get_pokerus() const;
            pkmn::datetime_t get_met_date(bool as_egg) const;

            // Setting Non-battle Info
            void set_contest_stats(const pkmn::contest_stats_t &contest_stats);
            void set_markings(const pkmn::markings_t &markings);
            void set_ribbons(const pkmn::ribbons_t &ribbons);
            void set_super_training_medals(const pkmn::super_training_medals_t &super_training_medals);
            void set_pokerus(const pkmn::pokerus_t &pokerus);
            void set_met_date(pkmn::datetime_t &date, bool as_egg);

            // Getting Trainer Info
            pkmn::pkstring get_nickname() const;
            pkmn::pkstring get_trainer_name() const;
            pkmn::pkstring get_trainer_gender() const;
            uint32_t get_trainer_id() const;
            uint16_t get_trainer_public_id() const;
            uint16_t get_trainer_secret_id() const;
            pkmn::pkstring get_ball() const;
            pkmn::pkstring get_original_game() const;
            int get_met_level() const;

            // Setting Trainer Info
            void set_nickname(const pkmn::pkstring &nickname);
            void set_trainer_name(const pkmn::pkstring &trainer_name);
            void set_trainer_gender(const pkmn::pkstring &gender);
            void set_trainer_id(uint32_t id);
            void set_trainer_public_id(uint16_t id);
            void set_trainer_secret_id(uint16_t id);
            void set_ball(const pkmn::pkstring &ball);
            void set_original_game(const pkmn::pkstring &game);
            void set_met_level(int level);

            // Getting Individual Stat Info
            uint32_t get_personality() const;
            int get_friendship() const;
            int get_level() const;
            uint32_t get_experience() const;
            pkmn::pkstring get_gender() const;
            pkmn::nature_t get_nature() const;
            pkmn::pkstring get_ability() const;
            bool is_shiny() const;
            pkmn::dict<pkmn::pkstring, int> get_stats() const;
            pkmn::dict<pkmn::pkstring, int> get_EVs() const;
            pkmn::dict<pkmn::pkstring, int> get_IVs() const;
            
            // Setting Individual Stat Info
            void set_personality(uint32_t personality);
            void set_friendship(int friendship);
            void set_level(int level);
            void set_experience(uint32_t experience);
            void set_gender(const pkmn::pkstring &gender);
            void set_nature(const pkmn::pkstring &nature_name);
            void set_ability(const pkmn::pkstring &ability);
            void set_form(const pkmn::pkstring &form);
            void set_shiny(bool value);
            void set_EV(const pkmn::pkstring &stat, int value);
            void set_IV(const pkmn::pkstring &stat, int value);

            // Battle Stat Info
            pkmn::pkstring get_status() const;
            pkmn::item_entry_t get_held_item() const;
            void set_status(const pkmn::pkstring &status);
            void set_held_item(const pkmn::pkstring &item_name);

            // Getting Move Info
            pkmn::move_entry_t get_move(int pos) const;
            void get_moves(pkmn::moveset_t &moves) const;
            int get_move_PP(int pos) const;
            void get_move_PPs(std::vector<int> &move_PPs) const;

            // Setting Move Info
            void set_move(const pkmn::pkstring &move_name, int pos);
            void set_move_PP(int PP, int pos);

            // Database Info
            int get_original_game_id() const;
            int get_ability_id() const;
            int get_item_id() const;
            int get_nature_id() const;

            const void* get_native();

        private:

            pkmn::native::gen2_party_pokemon_t _raw;

            pkmn::pkstring _nickname, _otname;

            void _set_experience(uint32_t exp);
            void _set_level(int level);
            void _set_stats();
            void _reset_caught_data();
    };
}

#endif /* INCLUDED_POKEMON_GEN2IMPL_HPP */
