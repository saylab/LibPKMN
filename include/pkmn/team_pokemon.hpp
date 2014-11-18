/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TEAM_POKEMON_HPP
#define INCLUDED_PKMN_TEAM_POKEMON_HPP

#include <string>
#include <vector>

#include <pkmn/base_pokemon.hpp>
#include <pkmn/config.hpp>
#include <pkmn/item.hpp>
#include <pkmn/move.hpp>
#include <pkmn/nature.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/markings.hpp>
#include <pkmn/types/pkstring.hpp>
#include <pkmn/types/shared_ptr.hpp>

namespace pkmn
{
    class PKMN_API team_pokemon
    {
        public:

            typedef pkmn::shared_ptr<team_pokemon> sptr;
            static sptr make(unsigned int id, unsigned int game, unsigned int level,
                             unsigned int move1, unsigned int move2,
                             unsigned int move3, unsigned int move4);
            static sptr make(const pkmn::pkstring &name, const pkmn::pkstring &game, unsigned int level,
                             const pkmn::pkstring &move1, const pkmn::pkstring &move2,
                             const pkmn::pkstring &move3, const pkmn::pkstring &move4);

            team_pokemon() {};
            virtual ~team_pokemon() {};

            //Game-specific Info
            virtual pkmn::pkstring get_game() const = 0;
            virtual unsigned int get_generation() const = 0;

            //Non-battle Stats
            virtual base_pokemon::sptr get_base_pokemon(bool copy = false) const = 0;
            virtual pkmn::pkstring get_species_name() const = 0;
            virtual pkmn::pkstring get_original_game() const = 0;
            virtual pkmn::pkstring get_nickname() const = 0;
            virtual pkmn::pkstring_pair_t get_types() const = 0;
            virtual pkmn::dict<pkmn::pkstring, unsigned int> get_base_stats() const = 0;

            //Getting Trainer Info
            virtual pkmn::pkstring get_trainer_name() const = 0;
            virtual pkmn::pkstring get_trainer_gender() const = 0;
            virtual unsigned int get_trainer_id() const = 0;
            virtual unsigned short get_trainer_public_id() const = 0;
            virtual unsigned short get_trainer_secret_id() const = 0;
            virtual pkmn::pkstring get_ball() const = 0;
            virtual unsigned int get_met_level() const = 0;

            //Setting Trainer Info
            virtual void set_original_game(unsigned int game) = 0;
            virtual void set_original_game(const pkmn::pkstring &game) = 0;
            virtual void set_nickname(const pkmn::pkstring &nickname) = 0;
            virtual void set_trainer_name(const pkmn::pkstring &trainer_name) = 0;
            virtual void set_trainer_gender(const pkmn::pkstring &gender) = 0;
            virtual void set_trainer_id(unsigned int id) = 0;
            virtual void set_trainer_public_id(unsigned short id) = 0;
            virtual void set_trainer_secret_id(unsigned short id) = 0;
            virtual void set_ball(const pkmn::pkstring &ball) = 0;
            virtual void set_met_level(unsigned int level) = 0;

            //Getting Individual Stat Info
            virtual unsigned int get_personality() const = 0;
            virtual unsigned int get_level() const = 0;
            virtual pkmn::pkstring get_gender() const = 0;
            virtual pkmn::nature get_nature() const = 0;
            virtual pkmn::pkstring get_ability() const = 0;
            virtual bool using_hidden_ability() const = 0;
            virtual bool is_shiny() const = 0;
            virtual pkmn::dict<pkmn::pkstring, unsigned int> get_stats() const = 0;
            virtual pkmn::dict<pkmn::pkstring, unsigned int> get_EVs() const = 0;
            virtual pkmn::dict<pkmn::pkstring, unsigned int> get_IVs() const = 0;
            
            //Setting Individual Stat Info
            virtual void set_personality(unsigned int personality) = 0;
            virtual void set_level(unsigned int level) = 0;
            virtual void set_nature(const pkmn::pkstring &nature_name) = 0;
            virtual void set_ability(const pkmn::pkstring &ability) = 0;
            virtual void set_using_hidden_ability(bool value) = 0;
            virtual void set_EV(const pkmn::pkstring &stat_name, unsigned int stat) = 0;
            virtual void set_IV(const pkmn::pkstring &stat_name, unsigned int stat) = 0;

            //Battle Stat Info
            virtual pkmn::pkstring get_status() const = 0;
            virtual item::sptr get_held_item() const = 0;
            virtual void set_status(const pkmn::pkstring &status) = 0;
            virtual void set_held_item(const pkmn::pkstring &item_name) = 0;
            virtual void set_held_item(item::sptr item_sptr) = 0;

            //Getting Move Info
            virtual move::sptr get_move(unsigned int pos) const = 0;
            virtual void get_moves(moveset_t& moves) const = 0;
            virtual unsigned int get_move_PP(unsigned int pos) const = 0;
            virtual void get_move_PPs(std::vector<unsigned int>& move_PPs) const = 0;

            //Setting Move Info
            virtual void set_move(const pkmn::pkstring &move_name, unsigned int pos) = 0;
            virtual void set_move(unsigned int move_id, unsigned int pos) = 0;
            virtual void set_move(move::sptr move_sptr, unsigned int pos) = 0;
            virtual void set_move_PP(unsigned int PP, unsigned int pos) = 0;

            //Misc
            virtual pkmn::markings get_markings() const = 0;
            virtual void set_marking(const uint16_t which, bool value) = 0;
            virtual void set_markings(const pkmn::markings &mark) = 0;
            virtual int get_attribute(const pkmn::pkstring &attribute) const = 0;
            virtual pkmn::dict<pkmn::pkstring, int> get_attributes() const = 0;
            virtual bool has_attribute(const pkmn::pkstring &attribute) const = 0;
			virtual void set_attribute(const pkmn::pkstring &attribute, int value) = 0;
            virtual std::string get_icon_path() const = 0;
            virtual std::string get_sprite_path() const = 0;
            virtual void set_form(const pkmn::pkstring &form) = 0;
            virtual void set_form(unsigned int form) = 0;

            //Database Info
            virtual unsigned int get_form_id() const = 0;
            virtual unsigned int get_game_id() const = 0;
            virtual unsigned int get_original_game_id() const = 0;
            virtual unsigned int get_pokemon_id() const = 0;
            virtual unsigned int get_species_id() const = 0;
            virtual unsigned int get_ability_id() const = 0;
            virtual unsigned int get_item_id() const = 0;
            virtual unsigned int get_nature_id() const = 0;
    };

    //Related typedefs
    typedef std::vector<team_pokemon::sptr> pokemon_team_t;
}

#endif /* INCLUDED_PKMN_TEAM_POKEMON_HPP */
