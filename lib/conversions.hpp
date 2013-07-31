/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_CONVERSIONS_HPP
#define INCLUDED_CONVERSIONS_HPP

#include <pkmnsim/spec_pkmn.hpp>

#include "sqlitecpp/SQLiteCPP.h"

#include <pokehack/pokestructs.h>
#include <pokelib/pokelib.h>
#include <pkmds/pkmds_g5.h>

namespace pkmnsim
{
    class converter
    {
        public:
            converter() {};

            static spec_pkmn::sptr pokehack_pkmn_to_spec_pkmn(belt_pokemon_t* b_pkmn_t,
                                                              pokemon_attacks_t* pkmn_a_t,
                                                              pokemon_effort_t* pkmn_e_t,
                                                              pokemon_misc_t* pkmn_m_t,
                                                              pokemon_growth_t* pkmn_g_t);

            static void spec_pkmn_to_pokehack_pkmn(spec_pkmn::sptr s_pkmn,
                                                   belt_pokemon_t* b_pkmn_t,
                                                   pokemon_attacks_t* pkmn_a_t,
                                                   pokemon_effort_t* pkmn_e_t,
                                                   pokemon_misc_t* pkmn_m_t,
                                                   pokemon_growth_t* pkmn_g_t);

            static spec_pkmn::sptr pokelib_pkmn_to_spec_pkmn(PokeLib::Pokemon pokelib_pkmn);

            static PokeLib::Pokemon spec_pkmn_to_pokelib_pkmn(spec_pkmn::sptr s_pkmn);

            static spec_pkmn::sptr pkmds_pkmn_to_spec_pkmn(party_pkm* p_pkm);

            static void spec_pkmn_to_pkmds_pkmn(spec_pkmn::sptr s_pkmn, party_pkm* p_pkm);

            static PokeLib::Pokemon pokehack_pkmn_to_pokelib_pkmn(belt_pokemon_t* b_pkmn_t,
                                                                  pokemon_attacks_t* pkmn_a_t,
                                                                  pokemon_effort_t* pkmn_e_t,
                                                                  pokemon_misc_t* pkmn_m_t,
                                                                  pokemon_growth_t* pkmn_g_t);

            static void pokehack_pkmn_to_pkmds_pkmn(belt_pokemon_t* b_pkmn_t,
                                                    pokemon_attacks_t* pkmn_a_t,
                                                    pokemon_effort_t* pkmn_e_t,
                                                    pokemon_misc_t* pkmn_m_t,
                                                    pokemon_growth_t* pkmn_g_t,
                                                    pokemon_obj* pkm);

        private:
            static char* pokehack_get_text(unsigned char* raw, bool is_nickname);
    };
}

#endif /* INCLUDED_CONVERSIONS_HPP */
