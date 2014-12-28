/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/algorithm/string.hpp>

#include <pkmn/calculations.hpp>
#include <pkmn/database.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/types/prng.hpp>

#include "base_pokemon_gen2impl.hpp"
#include "team_pokemon_gen2impl.hpp"

#include "conversions/utils.hpp"

namespace pkmn
{
    team_pokemon_gen2impl::team_pokemon_gen2impl(base_pokemon::sptr base, uint8_t level,
                                                 uint8_t move1, uint8_t move2,
                                                 uint8_t move3, uint8_t move4):
        team_pokemon_impl(base),
        _nickname(boost::algorithm::to_upper_copy(base->get_species().std_wstring())),
        _otname("GOLD")
    {
        /*
         * Populate native struct
         */
        _raw.pc.species = database::get_pokemon_game_index(_species_id, _game_id);
        _raw.pc.held_item = 0;
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
        _raw.pc.friendship = 70;
        _raw.pc.pokerus = 0;
        if(_game_id == Versions::CRYSTAL)
        {
            _raw.pc.caught_data = ((level & 0x3F) << 8);
            // Trainer male by default, can be set
            // TODO: met location
        }
        _set_level(level);
        _raw.status = 0x00;
        _raw.unused = 0x00; // TODO: check in-game data to see if there is any expected value
        _set_stats(); // Will populate rest of party portion of struct

        /*
         * Implement relationship between Unown's IV's and its form.
         */
        if(_species_id == Species::UNOWN)
        {
            _set_unown_form_from_IVs();

            base_pokemon_gen2impl* b_pkmn = reinterpret_cast<base_pokemon_gen2impl*>(_base.get());

            // When Unown's form is changed, update its IV's
            b_pkmn->_unown_signal.Connect(this, &team_pokemon_gen2impl::_set_unown_IVs_from_form);

            // When Unown's IV's are changed, update its form
            _unown_signal.Connect(b_pkmn, &base_pokemon_gen2impl::_set_unown_form);
        }
    }

    pkmn::pkstring team_pokemon_gen2impl::get_nickname() const
    {
        return _nickname;
    }

    pkmn::pkstring team_pokemon_gen2impl::get_trainer_name() const
    {
        return _otname;
    }

    pkmn::pkstring team_pokemon_gen2impl::get_trainer_gender() const
    {
        return (_raw.pc.caught_data & 0x80) ? "Female" : "Male";
    }

    // Trainer ID is only 16 bits in Generation II
    unsigned int team_pokemon_gen2impl::get_trainer_id() const
    {
        return _raw.pc.ot_id;
    }

    // Generation II Pokemon only have a public ID, so return the whole thing
    unsigned short team_pokemon_gen2impl::get_trainer_public_id() const
    {
        return _raw.pc.ot_id;
    }

    // No secret ID in Generation II
    unsigned short team_pokemon_gen2impl::get_trainer_secret_id() const
    {
        return 0;
    }

    // Ball not tracked in Generation II
    pkmn::pkstring team_pokemon_gen2impl::get_ball() const
    {
        return database::get_ball_name(Balls::POKE_BALL);
    }

    unsigned int team_pokemon_gen2impl::get_met_level() const
    {
        return (_raw.pc.caught_data & 0x3F00) >> 8;
    }

    // Use original game to check if caught data needs to be set (Crystal only)
    void team_pokemon_gen2impl::set_original_game(unsigned int game)
    {
        if(database::get_generation(game) > 2)
            throw std::runtime_error("Original game must be from Generation I-II.");

        if(game == Versions::CRYSTAL) _reset_caught_data();
        else _raw.pc.caught_data = 0;
    }

    void team_pokemon_gen2impl::set_nickname(const pkmn::pkstring &nickname)
    {
        if(nickname.length() > 10)
            throw std::runtime_error("Nicknames can have a maximum of 10 characters in Generation II.");

        _nickname = nickname;
    }

    void team_pokemon_gen2impl::set_trainer_name(const pkmn::pkstring &trainer_name)
    {
        if(trainer_name.length() > 7)
            throw std::runtime_error("Trainer names can have a maximum of 7 characters in Generation II.");

        _otname = trainer_name;
    }

    // All trainers male in Gold/Silver
    void team_pokemon_gen2impl::set_trainer_gender(const pkmn::pkstring &gender)
    {
        if(gender.std_string() != "Male" and gender.std_string() != "Female")
            throw std::runtime_error("Gender must be male or female.");

        if(_game_id == Versions::CRYSTAL)
        {
            if(gender == "Male") _raw.pc.caught_data &= ~(0x80);
            else                 _raw.pc.caught_data |= 0x80;
        }
    }

    // Trainer ID is only 16 bits in Generation II
    void team_pokemon_gen2impl::set_trainer_id(unsigned int id)
    {
        if(id > 65535)
            throw std::runtime_error("Trainer ID's have a maximum value of 65535 in Generation II.");

        _raw.pc.ot_id = id;
    }

    void team_pokemon_gen2impl::set_trainer_public_id(unsigned short id)
    {
        _raw.pc.ot_id = id;
    }

    // No secret ID in Generation II
    void team_pokemon_gen2impl::set_trainer_secret_id(unsigned short id)
    {
        /* NOP */
    }

    // Ball not tracked in Generation II
    void team_pokemon_gen2impl::set_ball(const pkmn::pkstring &ball)
    {
        /* NOP */
    }

    // Met level only tracked in Crystal
    void team_pokemon_gen2impl::set_met_level(unsigned int level)
    {
        /*
         * Pokemon in Generation I-II cannot be level 1.
         *
         * http://bulbapedia.bulbagarden.net/wiki/Experience#Experience_underflow_glitch
         */
        if(level < 2 or level > 100)
            throw std::runtime_error("Level must be 2-100.");

        if(_game_id == Versions::CRYSTAL)
            _raw.pc.caught_data |= ((level & 0x3F) << 8);
    }

    // No personality in Generation II
    unsigned int team_pokemon_gen2impl::get_personality() const
    {
        return 0;
    }

    unsigned int team_pokemon_gen2impl::get_level() const
    {
        return _raw.pc.level;
    }

    // Stored in base 256
    unsigned int team_pokemon_gen2impl::get_experience() const
    {
        return (65536*_raw.pc.exp[0]) + (256*_raw.pc.exp[1]) * _raw.pc.exp[2];
    }

    pkmn::pkstring team_pokemon_gen2impl::get_gender() const
    {
        float chance_male = _base->get_chance_male();
        float chance_female = _base->get_chance_female();
        uint8_t ivATK = conversions::get_retro_IV(Stats::ATTACK, _raw.pc.iv_data);

        if(chance_male + chance_female == 0.0) return "Genderless";
        else if(chance_male == 1.0) return "Male";
        else if(chance_female == 1.0) return "Female";
        else
        {
            /*
             * Gender is determined by gender ratio and Attack IV
             * If the Attack IV is below a certain number, the Pokemon will be female
             */
            if(chance_male == 0.875)     return (ivATK > 1)  ? "Male" : "Female";
            else if(chance_male == 0.75) return (ivATK > 3)  ? "Male" : "Female";
            else if(chance_male == 0.5)  return (ivATK > 7)  ? "Male" : "Female";
            else                         return (ivATK > 11) ? "Male" : "Female";
        }
    }

    // No natures in Generation II
    pkmn::nature_t team_pokemon_gen2impl::get_nature() const
    {
        return pkmn::nature_t();
    }

    // No abilities in Generation II
    pkmn::pkstring team_pokemon_gen2impl::get_ability() const
    {
        return "";
    }

    // No abilities in Generation II
    bool team_pokemon_gen2impl::using_hidden_ability() const
    {
        return false;
    }

    bool team_pokemon_gen2impl::is_shiny() const
    {
        uint8_t ivATK = conversions::get_retro_IV(Stats::ATTACK,   _raw.pc.iv_data);
        uint8_t ivDEF = conversions::get_retro_IV(Stats::DEFENSE,  _raw.pc.iv_data);
        uint8_t ivSPCL = conversions::get_retro_IV(Stats::SPECIAL, _raw.pc.iv_data);
        uint8_t ivSPD = conversions::get_retro_IV(Stats::SPEED,    _raw.pc.iv_data);

        return (ivSPD == 10 and ivDEF == 10 and ivSPCL == 10 and 
                (ivATK == 2 or ivATK == 3 or ivATK == 6 or
                 ivATK == 7 or ivATK == 10 or ivATK == 11 or
                 ivATK == 14 or ivATK == 15) 
               );
    }

    pkmn::dict<pkmn::pkstring, unsigned int> team_pokemon_gen2impl::get_stats() const
    {
        pkmn::dict<pkmn::pkstring, unsigned int> stats;
        stats["HP"]              = _raw.max_hp;
        stats["Attack"]          = _raw.atk;
        stats["Defense"]         = _raw.def;
        stats["Speed"]           = _raw.spd;
        stats["Special Attack"]  = _raw.spatk;
        stats["Special Defense"] = _raw.spdef;

        return stats;
    }

    pkmn::dict<pkmn::pkstring, unsigned int> team_pokemon_gen2impl::get_EVs() const
    {
        pkmn::dict<pkmn::pkstring, unsigned int> EVs;
        EVs["HP"]      = _raw.pc.ev_hp;
        EVs["Attack"]  = _raw.pc.ev_atk;
        EVs["Defense"] = _raw.pc.ev_def;
        EVs["Speed"]   = _raw.pc.ev_spd;
        EVs["Special"] = _raw.pc.ev_spcl;

        return EVs;
    }

    pkmn::dict<pkmn::pkstring, unsigned int> team_pokemon_gen2impl::get_IVs() const
    {
        pkmn::dict<pkmn::pkstring, unsigned int> IVs;
        IVs["HP"]      = conversions::get_retro_IV(Stats::HP,      _raw.pc.iv_data);
        IVs["Attack"]  = conversions::get_retro_IV(Stats::ATTACK,  _raw.pc.iv_data);
        IVs["Defense"] = conversions::get_retro_IV(Stats::DEFENSE, _raw.pc.iv_data);
        IVs["Speed"]   = conversions::get_retro_IV(Stats::SPEED,   _raw.pc.iv_data);
        IVs["Special"] = conversions::get_retro_IV(Stats::SPECIAL, _raw.pc.iv_data);

        return IVs;
    }

    // No personality in Generation II
    void team_pokemon_gen2impl::set_personality(unsigned int personality)
    {
        /* NOP */
    }

    // NOTE: this changes experience and stats
    void team_pokemon_gen2impl::set_level(unsigned int level)
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
    void team_pokemon_gen2impl::set_experience(unsigned int experience)
    {
        _set_experience(experience);
        _set_stats();
    }

    // No natures in Generation II
    void team_pokemon_gen2impl::set_nature(const pkmn::pkstring &nature_name)
    {
        /* NOP */
    }

    // No abilities in Generation II
    void team_pokemon_gen2impl::set_ability(const pkmn::pkstring &ability)
    {
        /* NOP */
    }

    // No abilities in Generation II
    void team_pokemon_gen2impl::set_using_hidden_ability(bool value)
    {
        /* NOP */
    }

    // NOTE: this affects stats
    void team_pokemon_gen2impl::set_EV(const pkmn::pkstring &stat_name, unsigned int stat)
    {
        if(stat_name == "Special Attack" or stat_name == "Special Defense")
            throw std::runtime_error("Special Attack and Special Defense use the"
                                     "Special EV in Generation II.");

        if(stat > 65535)
            throw std::runtime_error("EV's have a maximum value of 65535 in Generation II.");

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

    // NOTE: this affects stats, other IV's, and Unown's form
    void team_pokemon_gen2impl::set_IV(const pkmn::pkstring &stat_name, unsigned int stat)
    {
        if(stat_name == "Special Attack" or stat_name == "Special Defense")
            throw std::runtime_error("Special Attack and Special Defense use the"
                                     "Special IV in Generation II.");

        if(stat > 15)
            throw std::runtime_error("IV's have a maximum value of 15 in Generation II.");

        // Will throw if stat_name is invalid
        conversions::set_retro_IV(database::get_stat_id(stat_name), _raw.pc.iv_data, stat);

        if(_species_id == Species::UNOWN) _set_unown_form_from_IVs();
        _set_stats();
    }

    pkmn::pkstring team_pokemon_gen2impl::get_status() const
    {
        return conversions::retro_statuses.at(_raw.status, "OK");
    }

    item::sptr team_pokemon_gen2impl::get_held_item() const
    {
        return item::make(database::get_item_id(_raw.pc.held_item, _game_id), _game_id);
    }

    void team_pokemon_gen2impl::set_status(const pkmn::pkstring &status)
    {
        if(not conversions::reverse_retro_statuses.has_key(status))
            throw std::runtime_error("Invalid status given.");

        _raw.status = conversions::reverse_retro_statuses.at(status);
    }

    void team_pokemon_gen2impl::set_held_item(const pkmn::pkstring &item_name)
    {
        _raw.pc.held_item = database::get_item_game_index(item_name,
                                                          database::get_version_name(_game_id));
    }

    move::sptr team_pokemon_gen2impl::get_move(unsigned int pos) const
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        return move::make(_raw.pc.moves[pos-1], _game_id);
    }

    void team_pokemon_gen2impl::get_moves(pkmn::moveset_t &moves) const
    {
        moves.clear();
        for(size_t i = 0; i < 4; i++) moves.push_back(get_move(i+1));
    }

    unsigned int team_pokemon_gen2impl::get_move_PP(unsigned int pos) const
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        return _raw.pc.move_pps[pos-1];
    }

    void team_pokemon_gen2impl::get_move_PPs(std::vector<unsigned int> &move_PPs) const
    {
        move_PPs.clear();
        for(size_t i = 0; i < 4; i++) move_PPs.push_back(_raw.pc.move_pps[i+1]);
    }

    void team_pokemon_gen2impl::set_move(const pkmn::pkstring &move_name, unsigned int pos)
    {   
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        // Make sure move exists in given generation
        std::ostringstream query_stream;
        query_stream << "SELECT generation_id FROM moves WHERE id="
                     << database::get_move_id(move_name);
        SQLite::Statement query(*_db, query_stream.str().c_str());
        if(query.executeStep()) _raw.pc.moves[pos-1] = int(query.getColumn(0));
        else throw std::runtime_error("This move does not exist in Generation II.");
    }   

    void team_pokemon_gen2impl::set_move_PP(unsigned int PP, unsigned int pos)
    {   
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        if(PP <= database::get_move_pp(_raw.pc.moves[pos-1])) _raw.pc.move_pps[pos-1] = PP; 
        else throw std::runtime_error("This move PP is invalid.");
    }

    // No markings in Generation II
    pkmn::markings team_pokemon_gen2impl::get_markings() const
    {
        return pkmn::markings();
    }

    // No markings in Generation II
    void team_pokemon_gen2impl::set_markings(const pkmn::markings &mark)
    {
        /* NOP */
    }

    // No ribbons in Generation II
    pkmn::ribbons team_pokemon_gen2impl::get_ribbons() const
    {
        return pkmn::ribbons();
    }

    // No ribbons in Generation II
    void team_pokemon_gen2impl::set_ribbons(const pkmn::ribbons &rib)
    {
        /* NOP */
    }

    // Original game not tracked in Generation II
    unsigned int team_pokemon_gen2impl::get_original_game_id() const
    {
        return _game_id;
    }

    // No abilities in Generation II
    unsigned int team_pokemon_gen2impl::get_ability_id() const
    {
        return Abilities::NONE;
    }

    unsigned int team_pokemon_gen2impl::get_item_id() const
    {
        return database::get_item_id(_raw.pc.held_item, _game_id);
    }

    // No natures in Generation II
    unsigned int team_pokemon_gen2impl::get_nature_id() const
    {
        return Natures::NONE;
    }

    void* team_pokemon_gen2impl::get_native()
    {
        return &_raw;
    }

    void team_pokemon_gen2impl::_set_experience(const uint32_t exp)
    {
        uint32_t e = exp;

        _raw.pc.exp[0] = (e /= 65536);
        _raw.pc.exp[1] = (e /= 256);
        _raw.pc.exp[2] =  e;

        _raw.pc.level = database::get_level(_species_id, exp);
    }

    void team_pokemon_gen2impl::_set_level(const uint8_t level)
    {
        _raw.pc.level = level;

        uint32_t exp = database::get_experience(_species_id, level);
        _raw.pc.exp[0] = (exp /= 65536);
        _raw.pc.exp[1] = (exp /= 256);
        _raw.pc.exp[2] =  exp;
    }

    void team_pokemon_gen2impl::_set_stats()
    {
        pkmn::dict<pkmn::pkstring, unsigned int> stats = _base->get_base_stats();
        pkmn::dict<pkmn::pkstring, unsigned int> IVs = get_IVs();

        _raw.max_hp = int(floor((((double(IVs["HP"]) + double(stats["HP"]) + (pow(_raw.pc.ev_hp,0.5)/8.0)
                            + 50.0) * double(_raw.pc.level))/50.0) + 10.0));
        _raw.current_hp = _raw.max_hp;

        _raw.atk    = int(ceil((((double(IVs["Attack"]) + double(stats["Attack"]) + (pow(_raw.pc.ev_atk,0.5)/8.0))
                                * double(_raw.pc.level))/50.0) + 5.0));

        _raw.def    = int(ceil((((double(IVs["Defense"]) + double(stats["Defense"]) + (pow(_raw.pc.ev_def,0.5)/8.0))
                                * double(_raw.pc.level))/50.0) + 5.0));

        _raw.spd    = int(ceil((((double(IVs["Speed"]) + double(stats["Speed"]) + (pow(_raw.pc.ev_spd,0.5)/8.0))
                                * double(_raw.pc.level))/50.0) + 5.0));

        _raw.spatk  = int(ceil((((double(IVs["Special"]) + double(stats["Special Attack"]) + (pow(_raw.pc.ev_spcl,0.5)/8.0))
                                * double(_raw.pc.level))/50.0) + 5.0));

        _raw.spdef  = int(ceil((((double(IVs["Special"]) + double(stats["Special Defense"]) + (pow(_raw.pc.ev_spcl,0.5)/8.0))
                                * double(_raw.pc.level))/50.0) + 5.0));
    }

    void team_pokemon_gen2impl::_reset_caught_data()
    {
        if(_game_id == Versions::CRYSTAL) _raw.pc.caught_data = ((_raw.pc.level & 0x3F) << 8);
        else _raw.pc.caught_data = 0;
    }

    void team_pokemon_gen2impl::_set_unown_form_from_IVs()
    {
        pkmn::dict<pkmn::pkstring, unsigned int> IVs = get_IVs();
        uint32_t unown_form = calculations::get_gen2_unown_form(IVs["Attack"], IVs["Defense"],
                                                                IVs["Speed"],  IVs["Special"]);

        _unown_signal(unown_form);
    }

    void team_pokemon_gen2impl::_set_unown_IVs_from_form()
    {
        uint32_t form_id = _base->get_form_id();
        uint8_t letter = (form_id == Species::UNOWN) ? 0 : (form_id - Forms::Unown::B + 1);

        // Some parts of the IV are cut off, so give highest possible value
        uint8_t ivATK  = 0x9 | ((letter & 0xC0) >> 5);
        uint8_t ivDEF  = 0x9 | ((letter & 0x30) >> 3);
        uint8_t ivSPD  = 0x9 | ((letter & 0xC)  >> 1);
        uint8_t ivSPCL = 0x9 | ((letter & 0x3)  << 1);

        conversions::set_retro_IV(Stats::ATTACK,  _raw.pc.iv_data, ivATK);
        conversions::set_retro_IV(Stats::DEFENSE, _raw.pc.iv_data, ivDEF);
        conversions::set_retro_IV(Stats::SPEED,   _raw.pc.iv_data, ivSPD);
        conversions::set_retro_IV(Stats::SPECIAL, _raw.pc.iv_data, ivSPCL);

        _set_stats();
    }
}
