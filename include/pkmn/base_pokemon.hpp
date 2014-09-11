/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_BASE_POKEMON_HPP
#define INCLUDED_PKMN_BASE_POKEMON_HPP

#include <string>
#include <vector>

#include <pkmn/config.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/pkstring.hpp>
#include <pkmn/types/shared_ptr.hpp>

namespace pkmn
{
    class PKMN_API base_pokemon
    {
        public:

            typedef pkmn::shared_ptr<base_pokemon> sptr;
            static sptr make(unsigned int species_id, unsigned int game_id);
            static sptr make(const pkmn::pkstring &species, const pkmn::pkstring &game);

			base_pokemon() {};
            virtual ~base_pokemon() {};

            //Game-specific Info
            virtual pkmn::pkstring get_game() const = 0;
            virtual unsigned int get_generation() const = 0;

            //Non-Battle Attributes
            virtual pkmn::pkstring get_name() const = 0;
            virtual pkmn::pkstring get_species() const = 0;
            virtual unsigned int get_pokedex_num() const = 0;
            virtual pkmn::pkstring get_pokedex_entry() const = 0;
            virtual pkmn::pkstring_pair_t get_types() const = 0;
            virtual double get_height() const = 0;
            virtual double get_weight() const = 0;
            virtual void get_egg_groups(pkmn::pkstring_vector_t& egg_group_vec) const = 0;
            virtual bool has_gender_differences() const = 0;
            virtual void get_evolutions(std::vector<sptr>& evolution_vec) const = 0;
			virtual bool is_fully_evolved() const = 0;
            virtual double get_chance_male() const = 0;
            virtual double get_chance_female() const = 0;
            virtual pkmn::pkstring_pair_t get_abilities() const = 0;
            virtual pkmn::pkstring get_hidden_ability() const = 0;

            //Battle Stats
            virtual pkmn::dict<pkmn::pkstring, unsigned int> get_base_stats() const = 0;
            virtual pkmn::dict<pkmn::pkstring, unsigned int> get_ev_yields() const = 0;
            virtual unsigned int get_exp_yield() const = 0;

            //Forms
            virtual pkmn::pkstring get_form() const = 0;
            virtual void set_form(unsigned int form) = 0;
            virtual void set_form(const pkmn::pkstring &form) = 0;

            virtual std::string get_icon_path(bool is_male) const = 0;
            virtual std::string get_sprite_path(bool is_male, bool is_shiny) const = 0;

            //Database Info
            virtual unsigned int get_pokemon_id() const = 0;
            virtual unsigned int get_species_id() const = 0;
            virtual unsigned int get_game_id() const = 0;
            virtual unsigned int get_form_id() const = 0;
            virtual void get_egg_group_ids(std::vector<unsigned int>& egg_group_id_vec) const = 0;
            
    };

    //Related typedefs
    typedef std::vector<base_pokemon::sptr> base_pokemon_vector;
}

#endif /* INCLUDED_PKMN_BASE_POKEMON_HPP */
