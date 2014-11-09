/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cmath>
#include <ctime>

#include <pkmn/enums.hpp>
#include <pkmn/database/queries.hpp>
#include <pkmn/types/prng.hpp>

#include "team_pokemon_gen2impl.hpp"
#include "conversions/utils.hpp"

namespace pkmn
{
    team_pokemon_gen2impl::team_pokemon_gen2impl(base_pokemon::sptr base, unsigned int game, unsigned int level,
                                                 unsigned int move1, unsigned int move2,
                                                 unsigned int move3, unsigned int move4): team_pokemon_impl(base,game,level,
                                                                                          move1,move2,move3,move4)
    {
        prng::sptr rng = prng::make(2);

        _iv_data = rng->lcrng();

        //Random effort values
        _evHP = rng->lcrng() % 65536;
        _evATK = rng->lcrng() % 65536;
        _evDEF = rng->lcrng() % 65536;
        _evSPD = rng->lcrng() % 65536;
        _evSPCL = rng->lcrng() % 65536;

        _gender = _determine_gender();

        _set_stats();
    }

    team_pokemon_gen2impl::team_pokemon_gen2impl(const team_pokemon_gen2impl &other): team_pokemon_impl(other)
    {
        _SATK = other._SATK;
        _SDEF = other._SDEF;
        _evSPCL = other._evSPCL;
        _iv_data = other._iv_data;
    }

    team_pokemon_gen2impl& team_pokemon_gen2impl::operator=(const team_pokemon_gen2impl &other)
    {
        team_pokemon_impl::operator=(other);

        _SATK = other._SATK;
        _SDEF = other._SDEF;
        _evSPCL = other._evSPCL;
        _iv_data = other._iv_data;

        return (*this);
    }

    pkmn::pkstring team_pokemon_gen2impl::get_gender() const {return _gender;}

    nature team_pokemon_gen2impl::get_nature() const {return nature();}

    pkmn::pkstring team_pokemon_gen2impl::get_ability() const {return "None";}

    bool team_pokemon_gen2impl::is_shiny() const
    {
        uint8_t ivATK = conversions::get_retro_IV(Stats::ATTACK, _iv_data);
        uint8_t ivDEF = conversions::get_retro_IV(Stats::DEFENSE, _iv_data);
        uint8_t ivSPCL = conversions::get_retro_IV(Stats::SPECIAL, _iv_data);
        uint8_t ivSPD = conversions::get_retro_IV(Stats::SPEED, _iv_data);

        return (ivSPD == 10 and ivDEF == 10 and ivSPCL == 10 and
                (ivATK == 2 or ivATK == 3 or ivATK == 6 or
                 ivATK == 7 or ivATK == 10 or ivATK == 11 or
                 ivATK == 14 or ivATK == 15)
               );
    }

    pkmn::dict<pkmn::pkstring, unsigned int> team_pokemon_gen2impl::get_stats() const
    {
        pkmn::dict<pkmn::pkstring, unsigned int> stats;
        stats["HP"] = _HP;
        stats["Attack"] = _ATK;
        stats["Defense"] = _DEF;
        stats["Special Attack"] = _SATK;
        stats["Special Defense"] = _SDEF;
        stats["Speed"] = _SPD;

        return stats;
    }

    pkmn::dict<pkmn::pkstring, unsigned int> team_pokemon_gen2impl::get_EVs() const
    {
        pkmn::dict<pkmn::pkstring, unsigned int> EVs;
        EVs["HP"] = _evHP;
        EVs["Attack"] = _evATK;
        EVs["Defense"] = _evDEF;
        EVs["Speed"] = _evSPD;
        EVs["Special"] = _evSPCL;

        return EVs;
    }

    pkmn::dict<pkmn::pkstring, unsigned int> team_pokemon_gen2impl::get_IVs() const
    {
        pkmn::dict<pkmn::pkstring, unsigned int> IVs;
        IVs["HP"] = conversions::get_retro_IV(Stats::HP, _iv_data);
        IVs["Attack"] = conversions::get_retro_IV(Stats::ATTACK, _iv_data);
        IVs["Defense"] = conversions::get_retro_IV(Stats::DEFENSE, _iv_data);
        IVs["Speed"] = conversions::get_retro_IV(Stats::SPEED, _iv_data);
        IVs["Special"] = conversions::get_retro_IV(Stats::SPECIAL, _iv_data);

        return IVs;
    }

    void team_pokemon_gen2impl::set_personality(unsigned int personality)
    {
        _personality = personality;
    }

    void team_pokemon_gen2impl::set_nature(const pkmn::pkstring &nature_name) {}

    void team_pokemon_gen2impl::set_ability(const pkmn::pkstring &ability) {}

    void team_pokemon_gen2impl::set_EV(const pkmn::pkstring &stat_name, unsigned int val)
    {
        if(val > 65535) throw std::runtime_error("Gen 2 EV's must be 0-65535.");

        if(stat_name == "HP") _evHP = val;
        else if(stat_name == "Attack") _evATK = val;
        else if(stat_name == "Defense") _evDEF = val;
        else if(stat_name == "Speed") _evSPD = val;
        else if(stat_name == "Special") _evSPCL = val;

        _set_stats();
    }

    void team_pokemon_gen2impl::set_IV(const pkmn::pkstring &stat_name, unsigned int val)
    {
        if(val > 15) throw std::runtime_error("Gen 2 IV's must be 0-15.");

        conversions::set_retro_IV(database::get_stat_id(stat_name), _iv_data, val);
        _set_stats();
    }

    unsigned int team_pokemon_gen2impl::_get_hp() const
    {
        pkmn::dict<pkmn::pkstring, unsigned int> stats = _base_pkmn->get_base_stats();

        uint8_t iv_hp = conversions::get_retro_IV(Stats::HP, _iv_data);

        unsigned int hp_val = int(floor((((double(iv_hp) + double(stats["HP"]) + (pow(_evHP,0.5)/8.0)
                            + 50.0) * double(_level))/50.0) + 10.0));
        return hp_val;
    }

    unsigned int team_pokemon_gen2impl::_get_stat(const pkmn::pkstring &stat, unsigned int EV, unsigned int IV) const
    {
        pkmn::dict<pkmn::pkstring, unsigned int> stats = _base_pkmn->get_base_stats();

        uint8_t iv = conversions::get_retro_IV(database::get_stat_id(stat), _iv_data);

        unsigned int stat_val = int(ceil((((double(iv) + double(stats[stat]) + (pow(EV,0.5)/8.0))
                                * double(_level))/50.0) + 5.0));
        return stat_val;
    }

    void team_pokemon_gen2impl::_set_stats()
    {
        _HP = _get_hp();
        _ATK = _get_stat("Attack", _evATK, conversions::get_retro_IV(Stats::ATTACK, _iv_data));
        _DEF = _get_stat("Defense", _evDEF, conversions::get_retro_IV(Stats::DEFENSE, _iv_data));
        _SATK = _get_stat("Special Attack", _evSPCL, conversions::get_retro_IV(Stats::SPECIAL, _iv_data));
        _SDEF = _get_stat("Special Defense", _evSPCL, conversions::get_retro_IV(Stats::SPECIAL, _iv_data));
        _SPD = _get_stat("Speed", _evSPD, conversions::get_retro_IV(Stats::SPEED, _iv_data));
    }

    pkmn::pkstring team_pokemon_gen2impl::_determine_gender() const
    {
        if(_base_pkmn->get_chance_male() + _base_pkmn->get_chance_female() == 0.0) return "Genderless";
        else if(_base_pkmn->get_chance_male() == 1.0) return "Male";
        else if(_base_pkmn->get_chance_female() == 1.0) return "Female";
        else
        {
            //Gender is determined by gender ratio and Attack IV
            //If the Attack IV is below a certain number, the Pokemon will be female
            if(_base_pkmn->get_chance_male() == 0.875)
            {
                if(_ivATK > 1) return "Male";
                else return "Female";
            }
            else if(_base_pkmn->get_chance_male() == 0.75)
            {
                if(_ivATK > 3) return "Male";
                else return "Female";
            }
            else if(_base_pkmn->get_chance_male() == 0.5)
            {
                if(_ivATK > 7) return "Male";
                else return "Female";
            }
            else //25% male
            {
                if(_ivATK > 11) return "Male";
                else return "Female";
            }
        }
    }
} /* namespace pkmn */
