/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_POKEMON_GEN6IMPL_HPP
#define INCLUDED_POKEMON_GEN6IMPL_HPP

#include <pkmn/native/pokemon.hpp>

#include "pokemon_impl.hpp"

namespace pkmn
{
    class pokemon_gen6impl: public pokemon_impl
    {
        public:

            pokemon_gen6impl(int species, int version,
                             int level,
                             int move1, int move2,
                             int move3, int move4);
            pokemon_gen6impl(const pkmn::native::gen6_pc_pokemon_t &raw,
                             int version);
            pokemon_gen6impl(const pkmn::native::gen6_party_pokemon_t &raw,
                             int version);
            pokemon_gen6impl(const pokemon_gen6impl &other);
            pokemon_gen6impl& operator=(const pokemon_gen6impl &other);
            ~pokemon_gen6impl() {};

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
            pkmn::pkstring get_form() const;
            bool is_shiny() const;
            pkmn::dict<pkmn::pkstring, int> get_stats() const;
            pkmn::dict<pkmn::pkstring, int> get_EVs() const;
            pkmn::dict<pkmn::pkstring, int> get_IVs() const;
            
            // Setting Individual Stat Info
            void set_personality(uint32_t personality);
            void set_friendship(int friegen6hip);
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

            pkmn::native::gen6_party_pokemon_t   _raw;
            pkmn::native::gen6_pokemon_blockA_t *_blockA;
            pkmn::native::gen6_pokemon_blockB_t *_blockB;
            pkmn::native::gen6_pokemon_blockC_t *_blockC;
            pkmn::native::gen6_pokemon_blockD_t *_blockD;

            void _set_experience(uint32_t exp);
            void _set_level(int level);
            void _set_default_gender();
            void _set_stats();
            void _set_form();
    };
}

#endif /* INCLUDED_POKEMON_GEN6IMPL_HPP */
