/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_BASE_POKEMON_GEN2IMPL_HPP
#define INCLUDED_BASE_POKEMON_GEN2IMPL_HPP

#include <string>

#include "base_pokemon_impl.hpp"

namespace pkmn
{
    class base_pokemon_gen2impl: public base_pokemon_impl
    {
        public:
            base_pokemon_gen2impl(unsigned int id, unsigned int game_id);

            //Non-Battle Attributes
            bool has_gender_differences() const;
            pkmn::pkstring_pair_t get_abilities() const;
            pkmn::pkstring get_hidden_ability() const;

            //Battle Stats
            pkmn::dict<pkmn::pkstring, unsigned int> get_base_stats() const;
            pkmn::dict<pkmn::pkstring, unsigned int> get_ev_yields() const;

            void set_form(unsigned int form);
            void set_form(const pkmn::pkstring &form);

            std::string get_icon_path(bool is_male) const;
            std::string get_sprite_path(bool is_male, bool is_shiny) const;
    };
}

#endif /* INCLUDED_BASE_POKEMON_GEN2IMPL_HPP */
