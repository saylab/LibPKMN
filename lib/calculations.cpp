/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <cmath>

#include <boost/format.hpp>
#include <boost/assign/list_of.hpp>

#include <pkmn/calculations.hpp>
#include <pkmn/database.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/pokedex.hpp>
#include <pkmn/types/dict.hpp>

#include "internal.hpp"
#include "SQLiteCpp/SQLiteC++.h"

namespace pkmn
{
    namespace calculations
    {
        int get_ability_num(uint32_t personality)
        {
            return personality % 2;
        }

        std::pair<int, int> get_hidden_power(int ivHP, int ivATK, int ivDEF,
                                             int ivSATK, int ivSDEF, int ivSPD)
        {
            ivHP %= 4;
            ivATK %= 4;
            ivDEF %= 4;
            ivSPD %= 4;
            ivSATK %= 4;
            ivSDEF %= 4;

            uint8_t power, type;

            power = (32 * (ivHP >> 1))
                  + (16 * (ivATK >> 1))
                  + (8 * (ivDEF >> 1))
                  + (4 * (ivSPD >> 1))
                  + (2 * (ivSATK >> 1))
                  + (ivSDEF >> 1);

            type = (32 * (ivHP & 1))
                 + (16 * (ivATK & 1))
                 + (8 * (ivDEF & 1))
                 + (4 * (ivSPD & 1))
                 + (2 * (ivSATK & 1))
                 + (ivSDEF & 1);

            return std::make_pair(power, type);
        }

        int get_retro_stat(const pkmn::pkstring &stat_name,
                           int base_stat_value,
                           int level,
                           int EV, int IV)
        {
            if(stat_name == "HP")
            {
                return int(floor((((double(IV) + double(base_stat_value) + (pow(double(EV),0.5)/8.0)
                           + 50.0) * double(level))/50.0) + 10.0));
            }
            else
            {
                return int(ceil((((double(IV) + double(base_stat_value) + (pow(double(EV),0.5)/8.0))
                           * double(level))/50.0) + 5.0));
            }
        }

        static int get_modern_stat(const pkmn::pkstring &stat_name,
                                   int base_stat_value,
                                   int level, float nature_mod,
                                   int EV, int IV)
        {
            if(stat_name == "HP")
            {
                return int(floor(((double(IV) + (2.0*double(base_stat_value)) + (0.25*double(EV)) + 100.0)
                                 * double(level))/100.0 + 10.0));
            }
            else
            {
                return int(ceil(((((double(IV) + 2.0*double(base_stat_value) + 0.25*double(EV))
                                * double(level))/100.0) + 5.0) * nature_mod));
            }
        }

        int get_modern_stat(const pkmn::pkstring &stat_name,
                                 int base_stat_value,
                                 int level, const pkmn::nature_t &nature,
                                 int EV, int IV)
        {
            return get_modern_stat(stat_name, base_stat_value, level,
                                   ((stat_name == "HP") ? 1.0 : nature[stat_name]), EV, IV);
        }

        std::pair<int, int> get_stat_range(const pkmn::pokemon_entry_t &entry,
                                           const pkmn::pkstring &game,
                                           const pkmn::pkstring &stat_name,
                                           int level)
        {
            std::pair<int, int> stat_range;

            if(database::get_generation(database::get_version_id(game)) < 3)
            {
                stat_range.first  = get_retro_stat(stat_name, entry.base_stats.at(stat_name),
                                                   level, 0, 0);
                stat_range.second = get_retro_stat(stat_name, entry.base_stats.at(stat_name),
                                                   level, 65535, 31);
            }
            else
            {
                stat_range.first  = get_modern_stat(stat_name, entry.base_stats.at(stat_name),
                                                    level, 0.9, 0, 0);
                stat_range.second = get_modern_stat(stat_name, entry.base_stats.at(stat_name),
                                                    level, 1.1, 255, 15);
            }

            return stat_range;
        }

        bool is_stat_possible(const pkmn::pokemon_entry_t &entry,
                              const pkmn::pkstring &game,
                              const pkmn::pkstring &stat_name,
                              int stat_value,
                              int level)
        {
            std::pair<int, int> stat_range = get_stat_range(entry, game, stat_name,
                                                            level);
            return((stat_value >= stat_range.first) and
                   (stat_value <= stat_range.second));
        }

        pkmn::nature_t get_nature(uint32_t personality)
        {
            return pkmn::nature_t(personality % 24);
        }

        bool get_gen2_shiny(int ivATK, int ivDEF,
                            int ivSPD, int ivSPCL)
        {
            return ((ivDEF == 10 and ivSPD == 10 and ivSPCL == 10) and
                    (ivATK == 2 or ivATK == 3 or ivATK == 6 or ivATK == 7
                     or ivATK == 10 or ivATK == 11 or ivATK == 14
                     or ivATK == 15));
        }

        /*
         * Source: http://www.smogon.com/ingame/rng/pid_iv_creation#how_shiny
         */
        bool get_modern_shiny(uint32_t personality, uint16_t secret_tid, uint16_t public_tid)
        {
            uint16_t hid = (personality & 0xFFFF0000) >> 19;
            uint16_t lid = (personality & 0xFFFF) >> 3;
            uint16_t tid = public_tid >> 3;
            uint16_t sid = secret_tid >> 3;

            uint8_t num1_hid = count_ones(hid);
            if(num1_hid == 1 or num1_hid == 3) return false;
            uint8_t num1_lid = count_ones(lid);
            if(num1_lid == 1 or num1_lid == 3) return false;
            uint8_t num1_tid = count_ones(tid);
            if(num1_tid == 1 or num1_tid == 3) return false;
            uint8_t num1_sid = count_ones(sid);
            if(num1_sid == 1 or num1_sid == 3) return false;

            return true;
        }

        pkmn::pkstring get_gen2_unown_form(int ivATK, int ivDEF,
                                           int ivSPD, int ivSPCL)
        {
            uint8_t form = (((ivATK & 6) << 6)
                         +  ((ivDEF & 6) << 4)
                         +  ((ivSPD & 6) << 2)
                         +  (ivSPCL & 6));
            form /= 10;

            return database::get_form_name((form == 0) ? 201 : (Forms::Unown::B + form - 1));
        }

        pkmn::pkstring get_gen3_unown_form(uint32_t personality)
        {
            uint32_t form = ((personality & 0x3000000)
                          +  (personality & 0x30000)
                          +  (personality & 0x300)
                          +  (personality & 0x3));
            form %= 28;

            return database::get_form_name((form == 0) ? 201 : (Forms::Unown::B + form - 1));
        }

        pkmn::pkstring get_wurmple_evolution(uint32_t personality)
        {
            return database::get_species_name((((personality & 0xFFFF) % 10) < 5) ?
                                                         Species::SILCOON
                                                       : Species::CASCOON);
        }

        float get_type_damage_mod(const pkmn::pkstring &attacking_type,
                                  const pkmn::pkstring &defending_type,
                                  int gen)
        {
            int attacking_type_id = database::get_type_id(attacking_type);
            int defending_type_id = database::get_type_id(defending_type);

            std::ostringstream query_stream;
            query_stream << "SELECT damage_factor FROM " << ((gen == 1) ? "gen1_" : "")
                         << "type_efficacy WHERE attacking_type=" << attacking_type_id
                         << " AND defending_type=" << defending_type_id;
            SQLite::Database db(get_database_path());
            float damage_mod = float(db.execAndGet(query_stream.str().c_str())) / 100.0f;

            // Account for edge-case Gen V mods, not enough to warrant a database table
            if(gen > 5 and (attacking_type == "Dark" or attacking_type == "Ghost")
                       and defending_type == "Steel")
            {
                damage_mod = 1.0;
            }

            return damage_mod;
        }

        int get_base_damage(int level, int attack,
                            int defense, int base_power)
        {
            return std::max<int>(1, (int)(floor((((2.0 * double(level) + 10.0) / 250.0) *
                   (double(attack) / double(defense)) * double(base_power) * 2.0))));
        }

        int get_base_damage(pokemon::sptr attacker, pokemon::sptr defender,
                            const pkmn::pkstring &move)
        {
            pkmn::pokemon_entry_t attacker_entry = attacker->get_pokedex_entry();
            pkmn::dict<pkmn::pkstring, int> attacker_stats = attacker->get_stats();
            pkmn::pokemon_entry_t defender_entry = defender->get_pokedex_entry();
            pkmn::dict<pkmn::pkstring, int> defender_stats = defender->get_stats();
            pkmn::move_entry_t move_entry        = pkmn::move_entry_t(attacker->get_game(),
                                                                      move);
            pkmn::pkstring attack_stat_name;
            pkmn::pkstring defense_stat_name;
            if(move_entry.name == "Psystrike"
               or move_entry.name == "Psyshock"
               or move_entry.name == "Secret Sword")
            {
                attack_stat_name = "Special Attack";
                defense_stat_name = "Defense";
            }
            else
            {
                attack_stat_name = (move_entry.damage_class == "Physical") ? "Attack"
                                    : (attacker->get_generation() == 1) ? "Special"
                                                                        : "Special Attack";
                defense_stat_name = (move_entry.damage_class == "Physical") ? "Defense"
                                     : (attacker->get_generation() == 1) ? "Special"
                                                                         : "Special Defense";
            }

            int attack_stat = attacker_stats[attack_stat_name];
            int defense_stat = defender_stats[defense_stat_name];

            float damage_mod = get_type_damage_mod(move_entry.type, defender_entry.types.first,
                                                   attacker->get_generation())
                             * get_type_damage_mod(move_entry.type, defender_entry.types.second,
                                                   defender->get_generation());

            if(damage_mod == 0)
                return 0;

            if(damage_mod != 0)
            {
                if(defender->get_species_id() == Species::SHEDINJA)
                {
                    /*
                     * Shedinja's ability, Wonder Guard, blocks any attacks except for
                     * super-effective moves. However, super-effective moves will
                     * automatically make it faint.
                     */
                    if(damage_mod > 1.0)
                        return 1;
                    else
                        return 0;
                }
                else if(move_entry.name == "Seismic Toss"
                        or move_entry.name == "Night Shade")
                {
                    /*
                     * Seismic Toss and Night Shade's damage automatically
                     * matches the attacker's level.
                     */
                    return defender->get_level();
                }
                else if(move_entry.name == "Dragon Rage")
                {
                    // Dragon Rage always deals 40 damage
                    return 40;
                }
                else if(database::get_move_id(move_entry.name) == Moves::SONIC_BOOM)
                {
                    // Sonicboom always deals 20 damage
                    return 20;
                }
                else
                {
                    /*
                     * STAB: Same-type attack bonus
                     * If an attack's type matches the attacker's type,
                     * it's damage will be increased by 150%.
                     */
                    if(move_entry.type == attacker_entry.types.first or
                       move_entry.type == attacker_entry.types.second)
                    {
                        damage_mod *= 1.5;
                    }
                }
            }

            return get_base_damage(attacker->get_level(), attacker_entry.base_stats[attack_stat_name],
                                   defender_entry.base_stats[defense_stat_name], move_entry.power);
        }
    } /* namespace calculations */
} /* namespace pkmn */
