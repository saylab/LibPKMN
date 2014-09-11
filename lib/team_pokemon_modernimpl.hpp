/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TEAM_POKEMON_MODERNIMPL_HPP
#define INCLUDED_TEAM_POKEMON_MODERNIMPL_HPP

#include "team_pokemon_impl.hpp"
#include "Signal.h"

namespace pkmn
{
    class team_pokemon_modernimpl: public team_pokemon_impl
    {
        public:

            team_pokemon_modernimpl();
            team_pokemon_modernimpl(base_pokemon::sptr base, unsigned int game, unsigned int lvl,
                                    unsigned int move1, unsigned int move2,
                                    unsigned int move3, unsigned int move4);
            team_pokemon_modernimpl(const team_pokemon_modernimpl &other);
            team_pokemon_modernimpl& operator=(const team_pokemon_modernimpl &other);
            ~team_pokemon_modernimpl() {};

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
            void set_gender(const pkmn::pkstring &gender);
            void set_nature(const pkmn::pkstring &nature_name);
            void set_ability(const pkmn::pkstring &ability);
            virtual void set_using_hidden_ability(bool value);
            void set_EV(const pkmn::pkstring &stat_name, unsigned int stat);
            void set_IV(const pkmn::pkstring &stat_name, unsigned int stat);

            //Misc
            void set_form(const pkmn::pkstring &form);
            void set_form(unsigned int form);

        private:

            unsigned int _SATK, _SDEF;
            unsigned int _evSATK, _evSDEF;
            unsigned int _ivSATK, _ivSDEF;
            pkmn::nature _nature;

            unsigned int _get_hp() const;
            unsigned int _get_stat(const pkmn::pkstring &stat, unsigned int EV, unsigned int IV) const;
            void _set_stats();

            void _determine_ability();
            pkmn::pkstring _determine_gender() const;
            pkmn::nature _determine_nature() const;
    };
}

#endif /* INCLUDED_TEAM_POKEMON_MODERNIMPL_HPP */
