/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TEAM_POKEMON_GEN2IMPL_HPP
#define INCLUDED_TEAM_POKEMON_GEN2IMPL_HPP

#include "team_pokemon_impl.hpp"

namespace pkmn
{
    class team_pokemon_gen2impl: public team_pokemon_impl
    {
        public:

            team_pokemon_gen2impl();
            team_pokemon_gen2impl(base_pokemon::sptr base, unsigned int game, unsigned int level,
                                  unsigned int move1, unsigned int move2,
                                  unsigned int move3, unsigned int move4);
            team_pokemon_gen2impl(const team_pokemon_gen2impl &other);
            team_pokemon_gen2impl& operator=(const team_pokemon_gen2impl &other);
            ~team_pokemon_gen2impl() {};

            //Getting Individual Stat Info
            pkmn::pkstring get_gender() const;
            pkmn::nature_t get_nature() const;
            pkmn::pkstring get_ability() const;
            bool is_shiny() const;
            pkmn::dict<pkmn::pkstring, unsigned int> get_stats() const;
            pkmn::dict<pkmn::pkstring, unsigned int> get_EVs() const;
            pkmn::dict<pkmn::pkstring, unsigned int> get_IVs() const;

            //Setting Individual Stat Info
            void set_personality(unsigned int personality);
            void set_nature(const pkmn::pkstring &nature_name);
            void set_ability(const pkmn::pkstring &ability);
            void set_EV(const pkmn::pkstring &stat_name, unsigned int stat);
            void set_IV(const pkmn::pkstring &stat_name, unsigned int stat);

        private:

            uint16_t _iv_data;

            unsigned int _SATK, _SDEF, _evSPCL;

            unsigned int _get_hp() const;
            unsigned int _get_stat(const pkmn::pkstring &stat, unsigned int EV, unsigned int IV) const;
            void _set_stats();

            pkmn::pkstring _determine_gender() const;
    };
}

#endif /* INCLUDED_TEAM_POKEMON_GEN2IMPL_HPP */
