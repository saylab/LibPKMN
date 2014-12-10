/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_CONVERSIONS_POKEMON_HPP
#define INCLUDED_PKMN_CONVERSIONS_POKEMON_HPP

#include <pkmn/config.hpp>
#include <pkmn/team_pokemon.hpp>
#include <pkmn/types/dict.hpp>

#include "text.hpp"

#include "structs/pokemon.hpp"

#include <PokeLib/PokeLib.h>
#include <pkmds/pkmds_g5.h>

namespace pkmn
{
    namespace conversions
    {
        /*
         * Generation I
         */
        team_pokemon::sptr import_gen1_pokemon(const gen1_pc_pokemon_t &pkmn,
                                               const uint8_t* nickname_buffer,
                                               const uint8_t* otname_buffer);
        void export_gen1_pokemon(team_pokemon::sptr t_pkmn, gen1_pc_pokemon_t &pkmn,
                                 uint8_t* nickname_buf, uint8_t* otname_buf);

        team_pokemon::sptr import_gen1_pokemon(const gen1_party_pokemon_t &pkmn,
                                               const uint8_t* nickname_buf,
                                               const uint8_t* otname_buf);
        void export_gen1_pokemon(team_pokemon::sptr t_pkmn, gen1_party_pokemon_t &pkmn,
                                 uint8_t* nickname_buffer, uint8_t* otname_buffer);

        /*
         * Generation II
         */
        team_pokemon::sptr import_gen2_pokemon(const gen2_pc_pokemon_t &pkmn,
                                               const uint8_t* nickname_buffer,
                                               const uint8_t* otname_buffer);
        void export_gen2_pokemon(team_pokemon::sptr t_pkmn, gen2_pc_pokemon_t &pkmn,
                                 uint8_t* nickname_buf, uint8_t* otname_buf);

        team_pokemon::sptr import_gen2_pokemon(const gen2_party_pokemon_t &pkmn,
                                               const uint8_t* nickname_buf,
                                               const uint8_t* otname_buf);
        void export_gen2_pokemon(team_pokemon::sptr t_pkmn, gen2_party_pokemon_t &pkmn,
                                 uint8_t* nickname_buffer, uint8_t* otname_buffer);

        /*
         * Generation III
         */
        team_pokemon::sptr import_gen3_pokemon(const gen3_pc_pokemon_t &pkmn,
                                               unsigned int version_id,
                                               bool is_encrypted = true);
        void export_gen3_pokemon(team_pokemon::sptr t_pkmn, gen3_pc_pokemon_t &pkmn,
                                 bool encrypt = true);

        team_pokemon::sptr import_gen3_pokemon(const gen3_party_pokemon_t &pkmn,
                                               unsigned int version_id,
                                               bool is_encrypted = true);
        void export_gen3_pokemon(team_pokemon::sptr t_pkmn, gen3_party_pokemon_t &pkmn,
                                 bool encrypt = true);

        /*
         * Generation IV
         */
        team_pokemon::sptr import_gen4_pokemon(const PokeLib::Pokemon &pokelib_pkmn);
        PokeLib::Pokemon export_gen4_pokemon(team_pokemon::sptr t_pkmn);

        /*
         * Generation V
         */
        team_pokemon::sptr import_gen5_pokemon(pkmds::party_pkm* p_pkm);
        void export_gen5_pokemon(team_pokemon::sptr t_pkmn, pkmds::party_pkm* p_pkm);
    }
}

#endif /* INCLUDED_PKMN_CONVERSIONS_POKEMON_HPP */
