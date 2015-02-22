/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_POKEMON_GEN3IMPL_HPP
#define INCLUDED_POKEMON_GEN3IMPL_HPP

#include <pkmn/native/pokemon.hpp>

#include "pokemon_impl.hpp"

namespace pkmn
{
    class pokemon_gen3impl: public pokemon_impl
    {
        public:

            pokemon_gen3impl(uint16_t species, uint16_t version,
                             uint8_t level,
                             uint8_t move1, uint8_t move2,
                             uint8_t move3, uint8_t move4);
            pokemon_gen3impl(const pkmn::native::gen3_pc_pokemon_t& raw,
                             uint8_t version);
            pokemon_gen3impl(const pkmn::native::gen3_party_pokemon_t& raw,
                             uint8_t version);
            pokemon_gen3impl(const pokemon_gen3impl& other);
            pokemon_gen3impl& operator=(const pokemon_gen3impl& other);
            ~pokemon_gen3impl() {};

            // Getting Non-battle Info
            pkmn::contest_stats_t get_contest_stats() const;
            pkmn::markings_t get_markings() const;
            pkmn::ribbons_t get_ribbons() const;
            pkmn::super_training_medals_t get_super_training_medals() const;

            // Setting Non-battle Info
            void set_contest_stats(const pkmn::contest_stats_t& contest_stats);
            void set_markings(const pkmn::markings_t& markings);
            void set_ribbons(const pkmn::ribbons_t& ribbons);
            void set_super_training_medals(const pkmn::super_training_medals_t& super_training_medals);

            // Getting Trainer Info
            pkmn::pkstring get_nickname() const;
            pkmn::pkstring get_trainer_name() const;
            pkmn::pkstring get_trainer_gender() const;
            uint32_t get_trainer_id() const;
            uint16_t get_trainer_public_id() const;
            uint16_t get_trainer_secret_id() const;
            pkmn::pkstring get_ball() const;
            pkmn::pkstring get_original_game() const;
            uint8_t get_met_level() const;

            // Setting Trainer Info
            void set_nickname(const pkmn::pkstring& nickname);
            void set_trainer_name(const pkmn::pkstring& trainer_name);
            void set_trainer_gender(const pkmn::pkstring& gender);
            void set_trainer_id(uint32_t id);
            void set_trainer_public_id(uint16_t id);
            void set_trainer_secret_id(uint16_t id);
            void set_ball(const pkmn::pkstring& ball);
            void set_original_game(const pkmn::pkstring& game);
            void set_met_level(uint8_t level);

            // Getting Individual Stat Info
            uint32_t get_personality() const;
            uint8_t get_friendship() const;
            uint8_t get_level() const;
            uint32_t get_experience() const;
            pkmn::pkstring get_gender() const;
            pkmn::nature_t get_nature() const;
            pkmn::pkstring get_ability() const;
            bool is_shiny() const;
            pkmn::dict<pkmn::pkstring, uint16_t> get_stats() const;
            pkmn::dict<pkmn::pkstring, uint16_t> get_EVs() const;
            pkmn::dict<pkmn::pkstring, uint16_t> get_IVs() const;
            
            // Setting Individual Stat Info
            void set_personality(uint32_t personality);
            void set_friendship(uint8_t friendship);
            void set_level(uint8_t level);
            void set_experience(uint32_t experience);
            void set_gender(const pkmn::pkstring& gender);
            void set_nature(const pkmn::pkstring& nature_name);
            void set_ability(const pkmn::pkstring& ability);
            void set_form(const pkmn::pkstring& form);
            void set_shiny(bool value);
            void set_EV(const pkmn::pkstring& stat, uint16_t value);
            void set_IV(const pkmn::pkstring& stat, uint16_t value);

            // Battle Stat Info
            pkmn::pkstring get_status() const;
            pkmn::item_entry_t get_held_item() const;
            void set_status(const pkmn::pkstring& status);
            void set_held_item(const pkmn::pkstring& item_name);

            // Getting Move Info
            pkmn::move_entry_t get_move(uint8_t pos) const;
            void get_moves(pkmn::moveset_t& moves) const;
            uint8_t get_move_PP(uint8_t pos) const;
            void get_move_PPs(std::vector<uint8_t>& move_PPs) const;

            // Setting Move Info
            void set_move(const pkmn::pkstring& move_name, uint8_t pos);
            void set_move_PP(uint8_t PP, uint8_t pos);

            // Database Info
            uint16_t get_original_game_id() const;
            uint16_t get_ability_id() const;
            uint16_t get_item_id() const;
            uint16_t get_nature_id() const;

            const void* get_native();

        private:

            pkmn::native::gen3_party_pokemon_t    _raw;
            pkmn::native::gen3_pokemon_growth_t  *_growth;
            pkmn::native::gen3_pokemon_attacks_t *_attacks;
            pkmn::native::gen3_pokemon_effort_t  *_effort;
            pkmn::native::gen3_pokemon_misc_t    *_misc;

            void _set_experience(uint32_t exp);
            void _set_level(uint8_t level);
            void _set_stats();
            void _set_form();
    };
}

#endif /* INCLUDED_POKEMON_GEN3IMPL_HPP */
