/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_JAVA_CALCULATIONSWRAPPER_HPP
#define INCLUDED_JAVA_CALCULATIONSWRAPPER_HPP

#include <string>
#include <utility>

#include <pkmn/config.hpp>
#include <pkmn/calculations.hpp>

int PKMN_INLINE getAbilityNum(int personality)
{
    return pkmn::calculations::get_ability_num(personality);
}

int PKMN_INLINE getGen2Gender(int species_id, int ivATK)
{
    return pkmn::calculations::get_gen2_gender(species_id, ivATK);
}

int PKMN_INLINE getModernGender(int species_id, int personality)
{
    return pkmn::calculations::get_modern_gender(species_id, personality);
}

std::pair<int, int> PKMN_INLINE getHiddenPower(int ivHP, int ivATK, int ivDEF,
                                               int ivSATK, int ivSDEF, int ivSPD)
{
    std::pair<uint8_t, uint8_t> cpp_pair = pkmn::calculations::get_hidden_power(ivHP,ivATK,ivDEF,ivSATK,ivSDEF,ivSPD);
    std::pair<int, int> java_pair;
    java_pair.first = cpp_pair.first;
    java_pair.second = cpp_pair.second;

    return java_pair;
}

int PKMN_INLINE getNature(int personality)
{
    return pkmn::calculations::get_nature(personality);
}

bool PKMN_INLINE getGen2Shiny(int ivATK, int ivDEF, int ivSPD, int ivSPCL)
{
    return pkmn::calculations::get_gen2_shiny(ivATK,ivDEF,ivSPD,ivSPCL);
}

bool PKMN_INLINE getModernShiny(int personality, int secretTID, int publicTID)
{
    return pkmn::calculations::get_modern_shiny(personality, secretTID, publicTID);
}

int PKMN_INLINE getGen2UnownForm(int ivATK, int ivDEF, int ivSPD, int ivSPCL)
{
    return pkmn::calculations::get_gen2_unown_form(ivATK,ivDEF,ivSPD,ivSPCL);
}

int PKMN_INLINE getGen3UnownForm(int personality)
{
    return pkmn::calculations::get_gen3_unown_form(personality);
}

int PKMN_INLINE getWurmpleEvolution(int personality)
{
    return pkmn::calculations::get_wurmple_evolution(personality);
}

int PKMN_INLINE getMinPossibleStat(pkmn::base_pokemon::sptr bPKMN, const std::wstring &statName,
                                   int level, int gen)
{
    return pkmn::calculations::get_min_possible_stat(bPKMN, statName, level, gen);
}

int PKMN_INLINE getMaxPossibleStat(pkmn::base_pokemon::sptr bPKMN, const std::wstring &statName,
                                   int level, int gen)
{
    return pkmn::calculations::get_max_possible_stat(bPKMN, statName, level, gen);
}

int PKMN_INLINE isStatPossible(pkmn::base_pokemon::sptr bPKMN, int statValue, const std::wstring &statName,
                               int level, int gen)
{
    return pkmn::calculations::is_stat_possible(bPKMN, statValue, statName, level, gen);
}

std::pair<int, int> PKMN_INLINE getStatRange(pkmn::base_pokemon::sptr bPKMN, const std::wstring &statName,
                                             int level, int gen)
{
    std::pair<unsigned int, unsigned int> cpp_pair = pkmn::calculations::get_stat_range(bPKMN, statName, level, gen);
    std::pair<int, int> java_pair;
    java_pair.first = cpp_pair.first;
    java_pair.second = cpp_pair.second;

    return java_pair;
}

double PKMN_INLINE getTypeDamageMod(const std::wstring &type1, const std::wstring &type2, int gen)
{
    return pkmn::calculations::get_type_damage_mod(type1, type2, gen);
}

int PKMN_INLINE getBaseDamage(int level, int attack, int defense, int base_power)
{
    return pkmn::calculations::get_base_damage(level, attack, defense, base_power);
}

int PKMN_INLINE getBaseDamage(pkmn::team_pokemon::sptr attacker, pkmn::team_pokemon::sptr defender, pkmn::move::sptr attack)
{
    return pkmn::calculations::get_base_damage(attacker, defender, attack);
}

std::pair<int, int> PKMN_INLINE getDamageRange(pkmn::base_pokemon::sptr attacker, pkmn::base_pokemon::sptr defender,
                                               pkmn::move::sptr attack, int attackerLevel, int defenderLevel)
{
    std::pair<unsigned int, unsigned int> cpp_pair = pkmn::calculations::get_damage_range(attacker, defender, attack,
                                                                                          attackerLevel, defenderLevel);
    std::pair<int, int> java_pair;
    java_pair.first = cpp_pair.first;
    java_pair.second = cpp_pair.second;

    return java_pair;
}

std::pair<int, int> PKMN_INLINE getDamageRange(pkmn::team_pokemon::sptr attacker, pkmn::team_pokemon::sptr defender,
                                               pkmn::move::sptr attack)
{
    std::pair<unsigned int, unsigned int> cpp_pair = pkmn::calculations::get_damage_range(attacker, defender, attack);
    std::pair<int, int> java_pair;
    java_pair.first = cpp_pair.first;
    java_pair.second = cpp_pair.second;

    return java_pair;
}

#endif /* INCLUDED_JAVA_CALCULATIONSWRAPPER_HPP */
