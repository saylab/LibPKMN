/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/algorithm/string.hpp>

#include <pkmn/calculations.hpp>
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
        _nickname(boost::algorithm::to_upper_copy(base->get_species().std_wstring())),
        _otname("LIBPKMN")
    {
        /*
         * Populate native struct
         */
        _raw.pc.species = database::get_pokemon_game_index(_species_id, _game_id);
        // current_hp populated by _set_stats()
        _set_level(level);
        _raw.pc.status = 0x00;
        _raw.pc.types[0] = database::get_type_id(_base->get_types().first);
        _raw.pc.types[1] = database::get_type_id(_base->get_types().second);
        _raw.pc.catch_rate = conversions::gen1_catch_rates[_species_id];
        _raw.pc.moves[0] = move1;
        _raw.pc.moves[1] = move2;
        _raw.pc.moves[2] = move3;
        _raw.pc.moves[3] = move4;
        _raw.pc.ot_id = _prng->lcrng() % 65536;
        // experience determined by level
        _raw.pc.ev_hp = _prng->lcrng() % 65536;
        _raw.pc.ev_atk = _prng->lcrng() % 65536;
        _raw.pc.ev_def = _prng->lcrng() % 65536;
        _raw.pc.ev_spd = _prng->lcrng() % 65536;
        _raw.pc.ev_spcl = _prng->lcrng() % 65536;
        _raw.pc.iv_data = _prng->lcrng() % 65536;
        for(size_t i = 0; i < 4; i++)
            _raw.pc.move_pps[i] = database::get_move_pp(_raw.pc.moves[i]);
        _set_stats(); // Will populate party portion of struct
    }

    team_pokemon_gen1impl::team_pokemon_gen1impl(const team_pokemon_gen1impl &other):
        team_pokemon_impl(other),
        _raw(other._raw),
        _nickname(other._nickname),
        _otname(other._otname) {};

    team_pokemon_gen1impl& team_pokemon_gen1impl::operator=(const team_pokemon_gen1impl &other)
    {
        team_pokemon_impl::operator=(other);
        _raw      = other._raw;
        _nickname = other._nickname;
        _otname   = other._otname;
    }

    pkmn::pkstring team_pokemon_gen1impl::get_nickname() const
    {
        return _nickname;
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
    unsigned int team_pokemon_gen1impl::get_trainer_id() const
    {
        return _raw.pc.ot_id;
    }

    // Generation I Pokemon only have a public ID, so return the whole thing
    unsigned short team_pokemon_gen1impl::get_trainer_public_id() const
    {
        return _raw.pc.ot_id;
    }

    // No secret ID in Generation I
    unsigned short team_pokemon_gen1impl::get_trainer_secret_id() const
    {
        return 0;
    }

    // Ball not tracked in Generation I
    pkmn::pkstring team_pokemon_gen1impl::get_ball() const
    {
        return database::get_ball_name(Balls::POKE_BALL);
    }

    // Met level not tracked in Generation I
    unsigned int team_pokemon_gen1impl::get_met_level() const
    {
        return _raw.pc.level;
    }

    // Original game not tracked in Generation I
    void team_pokemon_gen1impl::set_original_game(unsigned int game)
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

        _raw.pc.ot_id = id;
    }

    void team_pokemon_gen1impl::set_trainer_public_id(unsigned short id)
    {
        _raw.pc.ot_id = id;
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
        return _raw.level;
    }

    // Stored in base 256
    unsigned int team_pokemon_gen1impl::get_experience() const
    {
        return (65536*_raw.pc.exp[0]) + (256*_raw.pc.exp[1]) * _raw.pc.exp[2];
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
        stats["HP"]      = _raw.max_hp;
        stats["Attack"]  = _raw.atk;
        stats["Defense"] = _raw.def;
        stats["Speed"]   = _raw.spd;
        stats["Special"] = _raw.spcl;

        return stats;
    }

    pkmn::dict<pkmn::pkstring, unsigned int> team_pokemon_gen1impl::get_EVs() const
    {
        pkmn::dict<pkmn::pkstring, unsigned int> EVs;
        EVs["HP"]      = _raw.pc.ev_hp;
        EVs["Attack"]  = _raw.pc.ev_atk;
        EVs["Defense"] = _raw.pc.ev_def;
        EVs["Speed"]   = _raw.pc.ev_spd;
        EVs["Special"] = _raw.pc.ev_spcl;

        return EVs;
    }

    pkmn::dict<pkmn::pkstring, unsigned int> team_pokemon_gen1impl::get_IVs() const
    {
        pkmn::dict<pkmn::pkstring, unsigned int> IVs;
        IVs["HP"]      = conversions::get_retro_IV(Stats::HP,      _raw.pc.iv_data);
        IVs["Attack"]  = conversions::get_retro_IV(Stats::ATTACK,  _raw.pc.iv_data);
        IVs["Defense"] = conversions::get_retro_IV(Stats::DEFENSE, _raw.pc.iv_data);
        IVs["Speed"]   = conversions::get_retro_IV(Stats::SPEED,   _raw.pc.iv_data);
        IVs["Special"] = conversions::get_retro_IV(Stats::SPECIAL, _raw.pc.iv_data);

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
                _raw.pc.ev_hp = stat;
                break;

            case Stats::ATTACK:
                _raw.pc.ev_atk = stat;
                break;

            case Stats::DEFENSE:
                _raw.pc.ev_def = stat;
                break;

            case Stats::SPEED:
                _raw.pc.ev_spd = stat;
                break;

            default: // Stats::SPECIAL
                _raw.pc.ev_spcl = stat;
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
        conversions::set_retro_IV(database::get_stat_id(stat_name), _raw.pc.iv_data, stat);

        _set_stats();
    }

    pkmn::pkstring team_pokemon_gen1impl::get_status() const
    {
        return conversions::retro_statuses.at(_raw.pc.status, "OK");
    }

    // No held item in Generation I
    item::sptr team_pokemon_gen1impl::get_held_item() const
    {
        return item::make(Items::NONE, _game_id);
    }

    void team_pokemon_gen1impl::set_status(const pkmn::pkstring &status)
    {
        if(not conversions::reverse_retro_statuses.has_key(status))
            throw std::runtime_error("Invalid status given.");

        _raw.pc.status = conversions::reverse_retro_statuses.at(status);
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

        return move::make(_raw.pc.moves[pos-1], _game_id);
    }

    void team_pokemon_gen1impl::get_moves(pkmn::moveset_t2 &moves) const
    {
        moves.clear();
        for(size_t i = 0; i < 4; i++) moves.push_back(get_move(i+1));
    }

    unsigned int team_pokemon_gen1impl::get_move_PP(unsigned int pos) const
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        return _raw.pc.move_pps[pos-1];
    }

    void team_pokemon_gen1impl::get_move_PPs(std::vector<unsigned int> &move_PPs) const
    {
        move_PPs.clear();
        for(size_t i = 0; i < 4; i++) move_PPs.push_back(_raw.pc.move_pps[i+1]); 
    }

    void team_pokemon_gen1impl::set_move(const pkmn::pkstring &move_name, unsigned int pos)
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        // Make sure move exists in given generation
        std::ostringstream query_stream;
        query_stream << "SELECT generation_id FROM moves WHERE id="
                     << database::get_move_id(move_name);
        SQLite::Statement query(*_db, query_stream.str().c_str());
        if(query.executeStep()) _raw.pc.moves[pos-1] = int(query.getColumn(0));
        else throw std::runtime_error("This move does not exist in Generation I.");
    }

    void team_pokemon_gen1impl::set_move_PP(unsigned int PP, unsigned int pos)
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        if(PP <= database::get_move_pp(_raw.pc.moves[pos-1])) _raw.pc.move_pps[pos-1] = PP;
        else throw std::runtime_error("This move PP is invalid.");
    }

    // No markings in Generation I
    pkmn::markings_t team_pokemon_gen1impl::get_markings() const
    {
        return pkmn::markings_t();
    }

    // No markings in Generation I
    void team_pokemon_gen1impl::set_markings(const pkmn::markings_t &mark)
    {
        /* NOP */
    }

    // No ribbons in Generation I
    pkmn::ribbons_t team_pokemon_gen1impl::get_ribbons() const
    {
        return pkmn::ribbons_t();
    }

    // No ribbons in Generation I
    void team_pokemon_gen1impl::set_ribbons(const pkmn::ribbons_t &rib)
    {
        /* NOP */
    }

    // Original game not tracked in Generation I
    unsigned int team_pokemon_gen1impl::get_original_game_id() const
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

    void* team_pokemon_gen1impl::get_native()
    {
        return &_raw;
    }

    void team_pokemon_gen1impl::_set_experience(const uint32_t exp)
    {
        uint32_t e = exp;

        _raw.pc.exp[0] = (e /= 65536);
        _raw.pc.exp[1] = (e /= 256);
        _raw.pc.exp[2] =  e;

        _raw.pc.level = database::get_level(_species_id, exp);
        _raw.level = _raw.pc.level;
    }

    void team_pokemon_gen1impl::_set_level(const uint8_t level)
    {
        _raw.pc.level = level;
        _raw.level = level;

        uint32_t exp = database::get_experience(_species_id, level);
        _raw.pc.exp[0] = (exp /= 65536);
        _raw.pc.exp[1] = (exp /= 256);
        _raw.pc.exp[2] =  exp;
    }

    void team_pokemon_gen1impl::_set_stats()
    {
        pkmn::dict<pkmn::pkstring, unsigned int> stats = _base->get_base_stats();
        pkmn::dict<pkmn::pkstring, unsigned int> IVs = get_IVs();

        _raw.max_hp = calculations::get_retro_stat(Stats::HP, stats["HP"], _raw.level,
                                                   _raw.pc.ev_hp, IVs["HP"]);
        _raw.pc.current_hp = _raw.max_hp;
        _raw.atk = calculations::get_retro_stat(Stats::ATTACK, stats["Attack"], _raw.level,
                                                _raw.pc.ev_atk, IVs["Attack"]);
        _raw.def = calculations::get_retro_stat(Stats::DEFENSE, stats["Defense"], _raw.level,
                                                _raw.pc.ev_def, IVs["Defense"]);
        _raw.spd = calculations::get_retro_stat(Stats::SPEED, stats["Speed"], _raw.level,
                                                _raw.pc.ev_spd, IVs["Speed"]);
        _raw.spcl = calculations::get_retro_stat(Stats::SPECIAL, stats["Special"], _raw.level,
                                                 _raw.pc.ev_spcl, IVs["Special"]);
    }
}
