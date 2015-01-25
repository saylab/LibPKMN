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
        uint8_t PKMN_API get_ability_num(uint32_t personality);

        std::pair<uint8_t, uint8_t> PKMN_API get_hidden_power(uint8_t ivHP, uint8_t ivATK, uint8_t ivDEF,
                                                              uint8_t ivSATK, uint8_t ivSDEF, uint8_t ivSPD);

        uint16_t PKMN_API get_retro_stat(const pkmn::pkstring& stat_name,
                                         uint16_t base_stat_value,
                                         uint8_t level,
                                         uint16_t EV, uint8_t IV);

        uint16_t PKMN_API get_modern_stat(const pkmn::pkstring& stat_name,
                                          uint16_t base_stat_value,
                                          uint8_t level, const pkmn::nature_t& nature,
                                          uint16_t EV, uint8_t IV);

        std::pair<uint16_t, uint16_t> PKMN_API get_stat_range(const pokemon_entry_t& entry,
                                                              const pkmn::pkstring& game,
                                                              const pkmn::pkstring& stat_name,
                                                              uint8_t level);

        bool PKMN_API is_stat_possible(const pokemon_entry_t& entry,
                                       const pkmn::pkstring& game,
                                       const pkmn::pkstring& stat_name,
                                       uint16_t stat_value,
                                       uint8_t level);

        pkmn::nature_t PKMN_API get_nature(uint32_t personality);

        bool PKMN_API get_gen2_shiny(uint8_t ivATK, uint8_t ivDEF,
                                     uint8_t ivSPD, uint8_t ivSPCL);

        bool PKMN_API get_modern_shiny(uint32_t personality, uint16_t secret_tid, uint16_t public_tid);

        pkmn::pkstring PKMN_API get_gen2_unown_form(uint8_t ivATK, uint8_t ivDEF,
                                                    uint8_t ivSPD, uint8_t ivSPCL);

        pkmn::pkstring PKMN_API get_gen3_unown_form(uint32_t personality);

        pkmn::pkstring PKMN_API get_wurmple_evolution(uint32_t personality);

        float PKMN_API get_type_damage_mod(const pkmn::pkstring& attacking_type,
                                           const pkmn::pkstring& defending_type,
                                           uint16_t gen);

        uint16_t PKMN_API get_base_damage(uint8_t level, uint16_t attack,
                                          uint16_t defense, uint16_t base_power);

        uint16_t PKMN_API get_base_damage(pokemon::sptr attacker, pokemon::sptr defender,
                                          const pkmn::pkstring& move);
    }
}

#endif /* INCLUDED_PKMN_CALCULATIONS_HPP */
