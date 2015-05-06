/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_POKEMON_HPP
#define INCLUDED_PKMN_POKEMON_HPP

#include <vector>

#include <pkmn/config.hpp>
#include <pkmn/pokedex.hpp>

#include <pkmn/types/contest_stats.hpp>
#include <pkmn/types/datetime.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/markings.hpp>
#include <pkmn/types/nature.hpp>
#include <pkmn/types/pkstring.hpp>
#include <pkmn/types/pokerus.hpp>
#include <pkmn/types/ribbons.hpp>
#include <pkmn/types/shared_ptr.hpp>
#include <pkmn/types/super_training_medals.hpp>

namespace pkmn
{
    class PKMN_API pokemon
    {
        public:

            typedef pkmn::shared_ptr<pokemon> sptr;

            static sptr make(int species, int version,
                             int level,
                             int move1, int move2,
                             int move3, int move4);
            static sptr make(const pkmn::pkstring &species, const pkmn::pkstring &version,
                             int level,
                             const pkmn::pkstring &move1, const pkmn::pkstring &move2,
                             const pkmn::pkstring &move3, const pkmn::pkstring &move4);
            static sptr make(const pkmn::pkstring &filename);

            pokemon() {};
            virtual ~pokemon() {};

            static void export_to(sptr pkmn, const pkmn::pkstring &filename);

            //! Return the Pokédex entry associated with this species+form
            virtual pkmn::pokemon_entry_t get_pokedex_entry() const = 0;

            //! Return this Pokémon's contest stats
            /*!
             * For Generation I-II Pokémon, this will return an empty instance
             */
            virtual pkmn::contest_stats_t get_contest_stats() const = 0;

            //! Return this Pokémon's markings
            /*!
             *
             */
            virtual pkmn::markings_t get_markings() const = 0;
            virtual pkmn::ribbons_t get_ribbons() const = 0;
            virtual pkmn::super_training_medals_t get_super_training_medals() const = 0;
            virtual pkmn::pokerus_t get_pokerus() const = 0;
            virtual pkmn::datetime_t get_met_date(bool as_egg = false) const = 0;

            // Setting Non-battle Info
            virtual void set_contest_stats(const pkmn::contest_stats_t &contest_stats) = 0;
            virtual void set_markings(const pkmn::markings_t &markings) = 0;
            virtual void set_ribbons(const pkmn::ribbons_t &ribbons) = 0;
            virtual void set_super_training_medals(const pkmn::super_training_medals_t &super_training_medals) = 0;
            virtual void set_pokerus(const pkmn::pokerus_t &pokerus) = 0;
            virtual void set_met_date(pkmn::datetime_t &date, bool as_egg = false) = 0;

            // Getting Trainer Info
            virtual pkmn::pkstring get_nickname() const = 0;
            virtual pkmn::pkstring get_trainer_name() const = 0;
            virtual pkmn::pkstring get_trainer_gender() const = 0;
            virtual uint32_t get_trainer_id() const = 0;
            virtual uint16_t get_trainer_public_id() const = 0;
            virtual uint16_t get_trainer_secret_id() const = 0;
            virtual pkmn::pkstring get_ball() const = 0;
            virtual pkmn::pkstring get_original_game() const = 0;
            virtual int get_met_level() const = 0;

            // Setting Trainer Info
            virtual void set_nickname(const pkmn::pkstring &nickname) = 0;
            virtual void set_trainer_name(const pkmn::pkstring &trainer_name) = 0;
            virtual void set_trainer_gender(const pkmn::pkstring &gender) = 0;
            virtual void set_trainer_id(uint32_t id) = 0;
            virtual void set_trainer_public_id(uint16_t id) = 0;
            virtual void set_trainer_secret_id(uint16_t id) = 0;
            virtual void set_ball(const pkmn::pkstring &ball) = 0;
            virtual void set_original_game(const pkmn::pkstring &game) = 0;
            virtual void set_met_level(int level) = 0;

            // Getting Individual Stat Info
            virtual uint32_t get_personality() const = 0;
            virtual int get_friendship() const = 0;
            virtual int get_level() const = 0;
            virtual uint32_t get_experience() const = 0;
            virtual pkmn::pkstring get_gender() const = 0;
            virtual pkmn::nature_t get_nature() const = 0;
            virtual pkmn::pkstring get_ability() const = 0;
            virtual pkmn::pkstring get_form() const = 0;
            virtual bool is_shiny() const = 0;
            virtual pkmn::dict<pkmn::pkstring, int> get_stats() const = 0;
            virtual pkmn::dict<pkmn::pkstring, int> get_EVs() const = 0;
            virtual pkmn::dict<pkmn::pkstring, int> get_IVs() const = 0;
            
            // Setting Individual Stat Info
            virtual void set_personality(uint32_t personality) = 0;
            virtual void set_friendship(int friendship) = 0;
            virtual void set_level(int level) = 0;
            virtual void set_experience(uint32_t experience) = 0;
            virtual void set_gender(const pkmn::pkstring &gender) = 0;
            virtual void set_nature(const pkmn::pkstring &nature_name) = 0;
            virtual void set_ability(const pkmn::pkstring &ability) = 0;
            virtual void set_form(const pkmn::pkstring &form) = 0;
            virtual void set_shiny(bool value) = 0;
            virtual void set_EV(const pkmn::pkstring &stat, int value) = 0;
            virtual void set_IV(const pkmn::pkstring &stat, int value) = 0;

            // Battle Stat Info
            virtual pkmn::pkstring get_status() const = 0;
            virtual pkmn::item_entry_t get_held_item() const = 0;
            virtual void set_status(const pkmn::pkstring &status) = 0;
            virtual void set_held_item(const pkmn::pkstring &item_name) = 0;

            // Getting Move Info
            virtual pkmn::move_entry_t get_move(int pos) const = 0;
            virtual void get_moves(pkmn::moveset_t &moves) const = 0;
            virtual int get_move_PP(int pos) const = 0;
            virtual void get_move_PPs(std::vector<int> &move_PPs) const = 0;

            // Setting Move Info
            virtual void set_move(const pkmn::pkstring &move_name, int pos) = 0;
            virtual void set_move_PP(int PP, int pos) = 0;

            // Getting LibPKMN info
            virtual pkmn::pkstring get_game() const = 0;
            virtual int get_generation() const = 0;
            virtual int get_attribute(const pkmn::pkstring &attribute) const = 0;
            virtual pkmn::dict<pkmn::pkstring, int> get_attributes() const = 0;
            virtual bool has_attribute(const pkmn::pkstring &attribute) const = 0;
			virtual void set_attribute(const pkmn::pkstring &attribute, int value) = 0;
            virtual pkmn::pkstring get_icon_path() const = 0;
            virtual pkmn::pkstring get_sprite_path() const = 0;

            // Database Info
            virtual int get_species_id() const = 0;
            virtual int get_game_id() const = 0;
            virtual int get_original_game_id() const = 0;
            virtual int get_pokemon_id() const = 0;
            virtual int get_ability_id() const = 0;
            virtual int get_item_id() const = 0;
            virtual int get_nature_id() const = 0;
            virtual int get_form_id() const = 0;

            virtual const void* get_native() = 0;
    };

    // Related typedefs
    typedef std::vector<pokemon::sptr> pokemon_team_t;
}

#endif /* INCLUDED_PKMN_POKEMON_HPP */
