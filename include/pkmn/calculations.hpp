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
        //! Return the which ability (0-1, first or second) results from the given personality
        int PKMN_API get_ability_num(uint32_t personality);

        //! Return the type ID and base power of Hidden Power, given the supplied IV's
        std::pair<int, int> PKMN_API get_hidden_power(int ivHP, int ivATK, int ivDEF,
                                                      int ivSATK, int ivSDEF, int ivSPD);

        //! Return the stat (as calculated in Generations I-II), given the supplied stats
        int PKMN_API get_retro_stat(const pkmn::pkstring &stat_name,
                                    int base_stat_value,
                                    int level,
                                    int EV, int IV);

        //! Return the stat (as calculated in Generations III+), given the supplied stats
        int PKMN_API get_modern_stat(const pkmn::pkstring &stat_name,
                                     int base_stat_value,
                                     int level, const pkmn::nature_t &nature,
                                     int EV, int IV);

        //! Return the potential stat range for the given Pokémon and stat
        std::pair<int, int> PKMN_API get_stat_range(const pokemon_entry_t &entry,
                                                    const pkmn::pkstring &game,
                                                    const pkmn::pkstring &stat_name,
                                                    int level);

        //! Check to see if given stat is possible for the given Pokémon
        bool PKMN_API is_stat_possible(const pokemon_entry_t &entry,
                                       const pkmn::pkstring &game,
                                       const pkmn::pkstring &stat_name,
                                       int stat_value,
                                       int level);

        //! Return the nature corresponding to the given personality
        pkmn::nature_t PKMN_API get_nature(uint32_t personality);

        //! Check to see if a Generation II Pokémon with the given IV's will be shiny
        bool PKMN_API get_gen2_shiny(int ivATK, int ivDEF,
                                     int ivSPD, int ivSPCL);

        //! Check to see if a Generation III+ Pokémon with the given personality and trainer ID will be shiny
        bool PKMN_API get_modern_shiny(uint32_t personality, uint16_t secret_tid, uint16_t public_tid);

        //! Return the Generation II Unown form that results from the given IV's
        pkmn::pkstring PKMN_API get_gen2_unown_form(int ivATK, int ivDEF,
                                                    int ivSPD, int ivSPCL);

        //! Return the Generation III Unown form that results from the given personality
        pkmn::pkstring PKMN_API get_gen3_unown_form(uint32_t personality);

        //! Check to see if a Wurmple with the given personality will evolve into a Silcoon or Cascoon
        pkmn::pkstring PKMN_API get_wurmple_evolution(uint32_t personality);

        //! Check the damage multiplier from one type to another
        float PKMN_API get_type_damage_mod(const pkmn::pkstring &attacking_type,
                                           const pkmn::pkstring &defending_type,
                                           int gen);

        //! Check how much damage a move will do to a Pokémon, given the supplied stats
        int PKMN_API get_base_damage(int level, int attack,
                                     int defense, int base_power);

        //! Check how much damage one Pokémon will do to another with the given move
        int PKMN_API get_base_damage(pokemon::sptr attacker, pokemon::sptr defender,
                                     const pkmn::pkstring &move);
    }
}

#endif /* INCLUDED_PKMN_CALCULATIONS_HPP */
