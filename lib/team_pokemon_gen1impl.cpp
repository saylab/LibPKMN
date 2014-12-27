/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/algorithm/string.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/database.hpp>
#include <pkmn/types/prng.hpp>

#include "team_pokemon_gen1impl.hpp"
#include "conversions/utils.hpp"

namespace pkmn
{
    team_pokemon_gen1impl::team_pokemon_gen1impl(base_pokemon::sptr base, uint8_t level,
                                                 uint8_t move1, uint8_t move2,
                                                 uint8_t move3, uint8_t move4):
        team_pokemon_impl(base),
        _nickname(boost::algorithm::to_upper_copy(base->get_species().std_wstring()),
        _otname("Red")
    {
        /*
         * Populate native struct
         */
        raw.pc.species = database::get_pokemon_game_index(_species_id, _game_id);
        // current_hp populated by _set_stats()
        _set_level(level);
        raw.pc.status = 0x00;
        raw.pc.types[0] = database::get_type_id(_base->get_types().first);
        raw.pc.types[1] = database::get_type_id(_base->get_types().second);
        raw.pc.catch_rate = gen1_catch_rates[_species_id];
        raw.pc.moves[0] = move1;
        raw.pc.moves[1] = move2;
        raw.pc.moves[2] = move3;
        raw.pc.moves[3] = move4;
        raw.pc.ot_id = prng->lcrng() % 65536;
        // experience determined by level
        raw.pc.ev_hp = prng->lcrng() % 65536;
        raw.pc.ev_atk = prng->lcrng() % 65536;
        raw.pc.ev_def = prng->lcrng() % 65536;
        raw.pc.ev_spd = prng->lcrng() % 65536;
        raw.pc.ev_spcl = prng->lcrng() % 65536;
        raw.pc.iv_data = prng->lcrng() % 65536;
        for(size_t i = 0; i < 4; i++)
            raw.pc.move_pps[i] = database::get_move_pp(raw.pc.moves[i]);
        _set_stats(); // Will populate party portion of struct
    }

    pkmn::pkstring team_pokemon_gen1impl::get_trainer_name() const
    {
        return _otname;
    }

    pkmn::pkstring team_pokemon_gen1impl::get_trainer_gender() const
    {
        return "Male";
    }

    // Trainer ID is only 16 bits in Generation I
    pkmn::pkstring team_pokemon_gen1impl::get_trainer_id() const
    {
        return raw.pc.ot_id;
    }

    // Generation I Pokemon only have a public ID, so return the whole thing
    pkmn::pkstring team_pokemon_gen1impl::get_trainer_public_id() const
    {
        return raw.pc.ot_id;
    }

    // No secret ID in Generation I
    pkmn::pkstring team_pokemon_gen1impl::get_trainer_secret_id() const
    {
        return 0;
    }

    // Ball not tracked in Generation I
    pkmn::pkstring team_pokemon_gen1impl::get_ball() const
    {
        return database::get_ball_name(Balls::POKE_BALL);
    }

    // Met level not tracked in Generation I
    pkmn::pkstring team_pokemon_gen1impl::get_met_level() const
    {
        return raw.pc.level;
    }

    // Original game not tracked in Generation I
    pkmn::pkstring team_pokemon_gen1impl::set_original_game(unsigned int game)
    {
        /* NOP */
    }

    // Original game not tracked in Generation I
    pkmn::pkstring team_pokemon_gen1impl::set_original_game(const pkmn::pkstring &game)
    {
        /* NOP */
    }

    void team_pokemon_gen1impl::set_nickname(const pkmn::pkstring &nickname)
    {
        if(nickname.length() > 10)
            throw std::runtime_error("Nicknames can have a maximum of 10 characters in Generation I.");

        _nickname = nickname;
    }

    void team_pokemon_gen1impl::set_trainer_name(const pkmn::pkstring &trainer_name)
    {
        if(trainer_name.length() > 7)
            throw std::runtime_error("Trainer names can have a maximum of 7 characters in Generation I.");

        _otname = trainer_name;
    }

    // All trainers male in Generation I
    void team_pokemon_gen1impl::set_trainer_gender(const pkmn::pkstring &gender)
    {
        /* NOP */
    }

    // Trainer ID is only 16 bits in Generation I
    void team_pokemon_gen1impl::set_trainer_id(unsigned int id)
    {
        if(id > 65535)
            throw std::runtime_error("Trainer ID's have a maximum value of 65535 in Generation I.");

        raw.pc.ot_id = id;
    }

    void team_pokemon_gen1impl::set_trainer_public_id(unsigned short id)
    {
        raw.pc.ot_id = id;
    }

    // No secret ID in Generation I
    void team_pokemon_gen1impl::set_trainer_secret_id(unsigned short id)
    {
        /* NOP */
    }

    // Ball not tracked in Generation I
    void team_pokemon_gen1impl::set_ball(const pkmn::pkstring &ball)
    {
        /* NOP */
    }

    // Met level not tracked in Generation I
    void team_pokemon_gen1impl::set_met_level(unsigned int level)
    {
        /* NOP */
    }

    // No personality in Generation I
    unsigned int team_pokemon_gen1impl::get_personality() const
    {
        return 0;
    }

    unsigned int team_pokemon_gen1impl::get_level() const
    {
        return raw.level;
    }

    // Stored in base 256
    unsigned int team_pokemon_gen1impl::get_experience() const
    {
        return (65536*raw.pc.exp[0]) + (256*raw.pc.exp[1]) * raw.pc.exp[2];
    }

    // No Pokemon genders in Generation I
    pkmn::pkstring team_pokemon_gen1impl::get_gender() const
    {
        return "Male";
    }

    // No natures in Generation I
    pkmn::nature_t team_pokemon_gen1impl::get_nature() const
    {
        return pkmn::nature_t();
    }

    // No abilities in Generation I
    pkmn::pkstring team_pokemon_gen1impl::get_ability() const
    {
        return "";
    }

    // No abilities in Generation I
    bool team_pokemon_gen1impl::using_hidden_ability() const
    {
        return false;
    }

    // No shininess in Generation I
    bool team_pokemon_gen1impl::is_shiny() const
    {
        return false;
    }

    pkmn::dict<pkmn::pkstring, unsigned int> team_pokemon_gen1impl::get_stats() const
    {
        pkmn::dict<pkmn::pkstring, unsigned int> stats;
        stats["HP"]      = raw.max_hp;
        stats["Attack"]  = raw.atk;
        stats["Defense"] = raw.def;
        stats["Speed"]   = raw.spd;
        stats["Special"] = raw.spcl;

        return stats;
    }

    pkmn::dict<pkmn::pkstring, unsigned int> team_pokemon_gen1impl::get_EVs() const
    {
        pkmn::dict<pkmn::pkstring, unsigned int> EVs;
        EVs["HP"]      = raw.pc.ev_hp;
        EVs["Attack"]  = raw.pc.ev_atk;
        EVs["Defense"] = raw.pc.ev_def;
        EVs["Speed"]   = raw.pc.ev_spd;
        EVs["Special"] = raw.pc.ev_spcl;

        return EVs;
    }

    pkmn::dict<pkmn::pkstring, unsigned int> team_pokemon_gen1impl::get_IVs() const
    {
        pkmn::dict<pkmn::pkstring, unsigned int> IVs;
        IVs["HP"]      = conversions::get_retro_IV(Stats::HP,      raw.pc.iv_data);
        IVs["Attack"]  = conversions::get_retro_IV(Stats::ATTACK,  raw.pc.iv_data);
        IVs["Defense"] = conversions::get_retro_IV(Stats::DEFENSE, raw.pc.iv_data);
        IVs["Speed"]   = conversions::get_retro_IV(Stats::SPEED,   raw.pc.iv_data);
        IVs["Special"] = conversions::get_retro_IV(Stats::SPECIAL, raw.pc.iv_data);

        return IVs;
    }

    // No personality in Generation I
    void team_pokemon_gen1impl::set_personality(unsigned int personality)
    {
        /* NOP */
    }

    // NOTE: this changes experience and stats
    void team_pokemon_gen1impl::set_level(unsigned int level)
    {
        /*
         * Pokemon in Generation I-II cannot be level 1.
         *
         * http://bulbapedia.bulbagarden.net/wiki/Experience#Experience_underflow_glitch
         */
        if(level < 2 or level > 100)
            throw std::runtime_error("Level must be 2-100.");

        _set_level(level);
        _set_stats();
    }

    // NOTE: this affects level and stats
    void team_pokemon_gen1impl::set_experience(unsigned int experience)
    {
        _set_experience(experience);
        _set_stats();
    }

    // No natures in Generation I
    void team_pokemon_gen1impl::set_nature(const pkmn::pkstring &nature_name)
    {
        /* NOP */
    }

    // No abilities in Generation I
    void team_pokemon_gen1impl::set_ability(const pkmn::pkstring &ability)
    {
        /* NOP */
    }

    // No abilities in Generation I
    void team_pokemon_gen1impl::set_using_hidden_ability(bool value)
    {
        /* NOP */
    }

    // NOTE: this affects stats
    void team_pokemon_gen1impl::set_EV(const pkmn::pkstring &stat_name, unsigned int stat)
    {
        if(stat_name == "Special Attack" or stat_name == "Special Defense")
            throw std::runtime_error("This stat is not present in Generation I.");

        if(stat > 65535)
            throw std::runtime_error("EV's have a maximum value of 65535 in Generation I.");

        switch(database::get_stat_id(stat_name)) // Will throw if stat_name is invalid
        {
            case Stats::HP:
                raw.pc.ev_hp = stat;
                break;

            case Stats::ATTACK:
                raw.pc.ev_atk = stat;
                break;

            case Stats::DEFENSE:
                raw.pc.ev_def = stat;
                break;

            case Stats::SPEED:
                raw.pc.ev_spd = stat;
                break;

            default: // Stats::SPECIAL
                raw.pc.ev_spcl = stat;
                break;
        }

        _set_stats();
    }

    // NOTE: this affects stats and other IV's
    void team_pokemon_gen1impl::set_IV(const pkmn::pkstring &stat_name, unsigned int stat)
    {
        if(stat_name == "Special Attack" or stat_name == "Special Defense")
            throw std::runtime_error("This stat is not present in Generation I.");

        if(stat > 15)
            throw std::runtime_error("IV's have a maximum value of 15 in Generation I.");

        // Will throw if stat_name is invalid
        conversions::set_retro_IV(database::get_stat_id(stat_name), raw.pc.iv_data, stat);

        _set_stats();
    }

    pkmn::pkstring team_pokemon_gen1impl::get_status() const
    {
        return retro_statuses.at(raw.pc.status, "OK");
    }

    // No held item in Generation I
    item::sptr team_pokemon_gen1impl::get_held_item() const
    {
        return item::make(Items::NONE, _game_id);
    }

    void team_pokemon_gen1impl::set_status(const pkmn::pkstring &status)
    {
        if(not reverse_retro_statuses.has_key(status))
            throw std::runtime_error("Invalid status given.");

        raw.pc.status = reverse_retro_statuses.at(status);
    }

    // No held items in Generation I
    void team_pokemon_gen1impl::set_held_item(const pkmn::pkstring &item_name)
    {
        /* NOP */
    }

    move::sptr team_pokemon_gen1impl::get_move(unsigned int pos) const
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        return move::make(raw.pc.moves[pos-1], _game_id);
    }

    void team_pokemon_gen1impl::get_moves(pkmn::moveset &moves) const
    {
        moves.clear();
        for(size_t i = 0; i < 4; i++) moves.push_back(get_move(i+1));
    }

    unsigned int team_pokemon_gen1impl::get_move_PP(unsigned int pos) const
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        return raw.pc.move_pps[pos-1];
    }

    void team_pokemon_gen1impl::get_move_PPs(std::vector<unsigned int> &move_PPs) const
    {
        move_PPs.clear();
        for(size_t i = 0; i < 4; i++) move_PPs.push_back(raw.pc.move_pps[pos]); 
    }

    // No markings in Generation I
    pkmn::markings team_pokemon_gen1impl::get_markings() const
    {
        return pkmn::markings();
    }

    // No markings in Generation I
    void team_pokemon_gen1impl::set_markings(const pkmn::markings &mark)
    {
        /* NOP */
    }

    // Original game not tracked in Generation I
    unsigned int team_pokemon_gen1impl::get_original_game() const
    {
        return _game_id;
    }

    // No abilities in Generation I
    unsigned int team_pokemon_gen1impl::get_ability_id() const
    {
        return Abilities::NONE;
    }

    // No held items in Generation I
    unsigned int team_pokemon_gen1impl::get_item_id() const
    {
        return Items::NONE;
    }

    // No natures in Generation I
    unsigned int team_pokemon_gen1impl::get_nature_id() const
    {
        return Natures::NONE;
    }

    void team_pokemon_gen1impl::_set_experience(const uint32_t exp)
    {
        uint32_t e = exp;

        raw.pc.exp[0] = (e /= 65536);
        raw.pc.exp[1] = (e /= 256);
        raw.pc.exp[2] =  e;

        raw.pc.level = database::get_level(_species_id, exp);
        raw.level = raw.pc.level;
    }

    void team_pokemon_gen1impl::_set_level(const uint8_t level)
    {
        raw.pc.level = level;
        raw.level = level;

        uint32_t exp = database::get_experience(_species_id, level);
        raw.pc.exp[0] = (exp /= 65536);
        raw.pc.exp[1] = (exp /= 256);
        raw.pc.exp[2] =  exp;
    }

    void team_pokemon_gen1impl::_set_stats()
    {
        pkmn::dict<pkmn::pkstring, unsigned int> stats = _base->get_base_stats();
        pkmn::dict<pkmn::pkstring, unsigned int> IVs = get_IVs();

        raw.max_hp = int(floor((((double(IVs["HP"]) + double(stats["HP"]) + (pow(raw.pc.ev_hp,0.5)/8.0)
                            + 50.0) * double(raw.level))/50.0) + 10.0));
        raw.pc.current_hp = raw.max_hp;

        raw.atk    = int(ceil((((double(IVs["Attack"]) + double(stats["Attack"]) + (pow(raw.pc.ev_atk,0.5)/8.0))
                                * double(raw.level))/50.0) + 5.0));

        raw.def    = int(ceil((((double(IVs["Defense"]) + double(stats["Defense"]) + (pow(raw.pc.ev_def,0.5)/8.0))
                                * double(raw.level))/50.0) + 5.0));

        raw.spd    = int(ceil((((double(IVs["Speed"]) + double(stats["Speed"]) + (pow(raw.pc.ev_spd,0.5)/8.0))
                                * double(raw.level))/50.0) + 5.0));

        raw.spcl   = int(ceil((((double(IVs["Special"]) + double(stats["Special"]) + (pow(raw.pc.ev_spcl,0.5)/8.0))
                                * double(raw.level))/50.0) + 5.0));
    }
}
