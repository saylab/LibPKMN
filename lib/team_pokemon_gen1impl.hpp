/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TEAM_POKEMON_GEN1IMPL_HPP
#define INCLUDED_TEAM_POKEMON_GEN1IMPL_HPP

#include "team_pokemon_impl.hpp"

namespace pkmn
{
    class team_pokemon_gen1impl: public team_pokemon_impl
    {
        public:

            team_pokemon_gen1impl();
            team_pokemon_gen1impl(base_pokemon::sptr base, unsigned int game, unsigned int level,
                                  unsigned int move1, unsigned int move2,
                                  unsigned int move3, unsigned int move4);
            team_pokemon_gen1impl(const team_pokemon_gen1impl &other);
            team_pokemon_gen1impl& operator=(const team_pokemon_gen1impl &other);
            ~team_pokemon_gen1impl() {};

            //Getting Individual Stat Info
            pkmn::pkstring get_gender() const;
            nature get_nature() const;
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

            unsigned int _SPCL, _evSPCL;

            unsigned int _get_hp() const;
            unsigned int _get_stat(const pkmn::pkstring &stat, unsigned int EV, unsigned int IV) const;
            void _set_stats();
    };
}

#endif /* INCLUDED_TEAM_POKEMON_GEN1IMPL_HPP */
