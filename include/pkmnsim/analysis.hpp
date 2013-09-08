/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_ANALYSIS_HPP
#define INCLUDED_PKMNSIM_ANALYSIS_HPP

#include <string>
#include <vector>

#include <pkmnsim/base_pokemon.hpp>
#include <pkmnsim/config.hpp>
#include <pkmnsim/spec_pokemon.hpp>

namespace pkmnsim
{
    /*
     * Returns the damage mod an attack of type1 would have against a Pokémon of type2.
     *
     * Parameters:
     *  - type1: Type of attack
     *  - type2: Type of receiving Pokemon
     *  - gen1: Whether or not analysis should be done with Gen 1 types
     */
    double PKMNSIM_API get_type_damage_mod(std::string type1, std::string type2, bool gen1);

    /*
     * Returns the minimum possible stat of a Pokemon.
     * Uses 0 EV, 0 IV, and disadvantageous nature (Gen 3-5).
     *
     * Parameters:
     *  - b_pkmn: Base Pokemon whose stats to analyze
     *  - stat: HP, ATK, DEF, SPD, SPCL, SATK, or SDEF
     *  - level: Pokemon's level for analysis
     *  - gen: Generation 1-5
     */
    int PKMNSIM_API get_min_possible_stat(base_pokemon::sptr b_pkmn, std::string stat, int level, int gen);

    /*
     * Returns the maximum possible stat of a Pokemon.
     * Uses max EV, max IV, and advantageous nature (Gen 3-5).
     *
     * Parameters:
     *  - b_pkmn: Base Pokemon whose stats to analyze
     *  - stat: HP, ATK, DEF, SPD, SPCL, SATK, or SDEF
     *  - level: Pokemon's level for analysis
     *  - gen: Generation 1-5
     */
    int PKMNSIM_API get_max_possible_stat(base_pokemon::sptr b_pkmn, std::string stat, int level, int gen);

    /*
     * Checks validity of specified statistic by using above functions.
     *
     * Parameters:
     *  - b_pkmn: Base Pokemon whose stats to analyze
     *  - stat: HP, ATK, DEF, SPD, SPCL, SATK, or SDEF
     *  - level: Pokemon's level for analysis
     *  - gen: Generation 1-5
     */
    bool PKMNSIM_API is_stat_possible(base_pokemon::sptr b_pkmn, int, std::string stat, int level, int gen);

    /*
     * Get range of given stat for given base_pokemon.
     *
     * Parameters:
     *  - b_pkmn: Base Pokemon whose stats to analyze
     *  - stat: HP, ATK, DEF, SPD, SPCL, SATK, or SDEF
     *  - level: Pokemon's level for analysis
     *  - gen: Generation 1-5
     *  - stat_vec: Reference to vector in which to return values
     */
    void PKMNSIM_API get_stat_range(base_pokemon::sptr b_pkmn, std::string stat, int level, int gen, std::vector<int>& stat_vec);

    /*
     * Pass-by-value equivalent of previous function.
     */
    std::vector<int> PKMNSIM_API get_stat_range_vec(base_pokemon::sptr b_pkmn, std::string stat, int level, int gen);

    /*
     * Gets damage of a move, before taking type advantages and STAB into account,
     * and without random variation.
     *
     * Parameters:
     *  - attacker: Specific Pokemon executing move
     *  - defender: Specific Pokemon receiving move
     *  - move: Move being executed
     */
    int PKMNSIM_API get_base_damage(spec_pokemon::sptr attacker, spec_pokemon::sptr defender, base_move::sptr move);

    /*
     * Gets damage of a move, before taking type advantages and STAB into account,
     * and without random variation.
     *
     * Parameters:
     *  - level: Pokemon's level for analysis
     *  - attack: Attacker's ATK/SPCL/SATK stat, as appropriate
     *  - defense: Defender's DEF/SPCL/SDEF stat, as appropriate
     *  - base_power: Move's base power
     */
    int PKMNSIM_API get_base_damage(int level, int attack, int defense, int base_power);

    /*
     * Gets range of damage of a move, taking type advantages and STAB into account.
     *
     * Parameters:
     *  - attacker: Specific Pokemon executing move
     *  - defender: Specific Pokemon receiving move
     *  - move: Move being executed
     *  - gen: Generation of analysis
     *  - damage_range_vec: Reference to vector in which to return values
     */
    void PKMNSIM_API get_damage_range(spec_pokemon::sptr attacker, spec_pokemon::sptr defender,
                                      base_move::sptr move, std::vector<int>& damage_range_vec);

    /*
     * Pass-by-value equivalent of previous function.
     */
    std::vector<int> PKMNSIM_API get_damage_range_from_spec_pokemon(spec_pokemon::sptr attacker, spec_pokemon::sptr defender,
                                                                 base_move::sptr move);

    /*
     * Gets range of damage of a move, taking type advantages and STAB into account.
     *
     * Parameters:
     *  - attacker: Base Pokemon executing move
     *  - defender: Base Pokemon receiving move
     *  - move: Move being executed
     *  - attacker_level: Attacker's level, taken into account in equation
     *  - gen: Generation of analysis
     *  - damage_range_vec: Reference to vector in which to return values
     */
    void PKMNSIM_API get_damage_range(base_pokemon::sptr attacker, base_pokemon::sptr defender,
                                      base_move::sptr move, int attacker_level,
                                      int defender_level, std::vector<int>& damage_range_vec);

    /*
     * Pass-by-value equivalent of previous function.
     */
    std::vector<int> PKMNSIM_API get_damage_range_from_base_pokemon(base_pokemon::sptr attacker, base_pokemon::sptr defender,
                                                                 base_move::sptr move, int attacker_level, int defender_level);
}

#endif /* INCLUDED_PKMNSIM_ANALYSIS_HPP */
