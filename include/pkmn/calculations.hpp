/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_CALCULATIONS_HPP
#define INCLUDED_PKMN_CALCULATIONS_HPP

#include <cstdint>
#include <utility>

#include <pkmn/config.hpp>
#include <pkmn/pokemon.hpp>
#include <pkmn/pokedex/pokemon_entry.hpp>
#include <pkmn/types/nature.hpp>
#include <pkmn/types/pkstring.hpp>

namespace pkmn
{
    namespace calculations
    {
        int PKMN_API get_ability_num(uint32_t personality);

        std::pair<int, int> PKMN_API get_hidden_power(int ivHP, int ivATK, int ivDEF,
                                                      int ivSATK, int ivSDEF, int ivSPD);

        int PKMN_API get_retro_stat(const pkmn::pkstring &stat_name,
                                    int base_stat_value,
                                    int level,
                                    int EV, int IV);

        int PKMN_API get_modern_stat(const pkmn::pkstring &stat_name,
                                     int base_stat_value,
                                     int level, const pkmn::nature_t &nature,
                                     int EV, int IV);

        std::pair<int, int> PKMN_API get_stat_range(const pokemon_entry_t &entry,
                                                    const pkmn::pkstring &game,
                                                    const pkmn::pkstring &stat_name,
                                                    int level);

        bool PKMN_API is_stat_possible(const pokemon_entry_t &entry,
                                       const pkmn::pkstring &game,
                                       const pkmn::pkstring &stat_name,
                                       int stat_value,
                                       int level);

        pkmn::nature_t PKMN_API get_nature(uint32_t personality);

        bool PKMN_API get_gen2_shiny(int ivATK, int ivDEF,
                                     int ivSPD, int ivSPCL);

        bool PKMN_API get_modern_shiny(uint32_t personality, uint16_t secret_tid, uint16_t public_tid);

        pkmn::pkstring PKMN_API get_gen2_unown_form(int ivATK, int ivDEF,
                                                    int ivSPD, int ivSPCL);

        pkmn::pkstring PKMN_API get_gen3_unown_form(uint32_t personality);

        pkmn::pkstring PKMN_API get_wurmple_evolution(uint32_t personality);

        float PKMN_API get_type_damage_mod(const pkmn::pkstring &attacking_type,
                                           const pkmn::pkstring &defending_type,
                                           int gen);

        int PKMN_API get_base_damage(int level, int attack,
                                     int defense, int base_power);

        int PKMN_API get_base_damage(pokemon::sptr attacker, pokemon::sptr defender,
                                     const pkmn::pkstring &move);
    }
}

#endif /* INCLUDED_PKMN_CALCULATIONS_HPP */
