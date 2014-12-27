/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/algorithm/string.hpp>

#include <pkmn/calculations.hpp>
#include <pkmn/database.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/types/prng.hpp>

#include "library_bridge.hpp" 

#include "base_pokemon_modernimpl.hpp"
#include "team_pokemon_gen3impl.hpp"

#include "conversions/text.hpp"
#include "conversions/utils.hpp"

namespace pkmn
{
    team_pokemon_gen3impl::team_pokemon_gen3impl(base_pokemon::sptr base, uint8_t level,
                                                 uint8_t move1, uint8_t move2,
                                                 uint8_t move3, uint8_t move4):
        team_pokemon_impl(base)
    {
        /*
         * Populate native struct
         */
        _raw.pc.personality = _prng->lcrng();
        _raw.pc.ot_id = _prng->lcrng();

        pkmn::pkstring nickname = boost::to_upper_copy(_base->get_species());
        conversions::export_gen3_text(nickname, _raw.pc.nickname, 10);

        _raw.pc.language = 0x202; // English
        conversions::export_gen3_text("RED", _raw.pc.otname, 7);
        // TODO: checksum

        growth = &(_raw.pc.blocks.growth);
        attacks = &(_raw.pc.blocks.attacks);
        effort = &(_raw.pc.blocks.effort);
        misc = &(_raw.pc.blocks.misc);

        growth->species = database::get_pokemon_game_index(_species_id, _game_id);
        growth->held_item = 0;
        // experience will be determined by level
        growth->pp_up = 0; // TODO
        growth->friendship = 70;
        growth->unknown_0xA; // TODO: check for expected value
        attacks->moves[0] = move1;
        attacks->moves[1] = move2;
        attacks->moves[2] = move3;
        attacks->moves[3] = move4;
        for(size_t i = 0; i < 4; i++)
            attacks->move_pps[i] = database::get_move_pp(attacks->moves[i]);
        do
        {
            effort->ev_hp    = _prng->lcrng() % 256;
            effort->ev_atk   = _prng->lcrng() % 256;
            effort->ev_def   = _prng->lcrng() % 256;
            effort->ev_spd   = _prng->lcrng() % 256;
            effort->ev_spatk = _prng->lcrng() % 256;
            effort->ev_spdef = _prng->lcrng() % 256;
        }
        while((effort->ev_hp  + effort->ev_atk   + effort->ev_def +
               effort->ev_spd + effort->ev_spatk + effort->ev_spdef) > 510);
        // TODO: contest stats data structure
        _attributes["cool"]   = effort->coolness;
        _attributes["beauty"] = effort->beauty;
        _attributes["cute"]   = effort->cute;
        _attributes["smart"]  = effort->smart;
        _attributes["tough"]  = effort->tough;
        _attributes["feel"]   = effort->feel;
        // TODO: Pokerus
        misc->met_location = 255; // Fateful encounter

        // Origin info
        misc->origin_info = (level & 0x3F);
        misc->origin_info |= (uint8_t(libpkmn_game_to_hometown(_game_id)) << 6);
        misc->origin_info |= (uint8_t(Balls::LUXURY_BALL) << 10);
        // Don't set trainer gender, male by default

        misc->iv_egg_ability = _prng->lcrng();
        misc->iv_egg_ability &= ~(1<<30); // Not an egg

        misc->ribbons_obedience = 0;
        // TODO: ribbons
        misc->ribbons_obedience |= (1<<31); // Mew and Deoxys will be obedient

        _raw.condition = 0; // OK
        _set_level();
        _set_stats();
    }

    pkmn::pkstring team_pokemon_gen3impl::get_trainer_name() const
    {
        return conversions::import_gen3_text(_raw.pc.otname, 7);
    }

    pkmn::pkstring team_pokemon_gen3impl::get_trainer_gender() const
    {
        return (misc->origin_info & (1<<31)) ? "Female" : "Male";
    }

    pkmn::pkstring team_pokemon_gen3impl::get_trainer_id() const
    {
        return _raw.pc.ot_id;
    }

    pkmn::pkstring team_pokemon_gen3impl::get_trainer_public_id() const
    {
        return _raw.pc.ot_pid;
    }

    pkmn::pkstring team_pokemon_gen3impl::get_trainer_secret_id() const
    {
        return _raw.pc.ot_sid;
    }

    pkmn::pkstring team_pokemon_gen3impl::get_ball() const
    {
        return database::get_ball_name((misc->origin_info & 0x3C00) >> 10);
    }

    pkmn::pkstring team_pokemon_gen3impl::get_met_level() const
    {
        return (_raw.pc.caught_data & 0x3F00) >> 8;
    }

    pkmn::pkstring team_pokemon_gen3impl::set_original_game(unsigned int game)
    {
        if(database::get_generation(game) != 3)
            throw std::runtime_error("Original game must be from Generation III.");

        misc->origin_info = 0xFC3F | (uint8_t(libpkmn_game_to_hometown(game)) << 6);
    }

    pkmn::pkstring team_pokemon_gen3impl::set_original_game(const pkmn::pkstring &game)
    {
        set_original_game(database::get_version_id(game));
    }

    void team_pokemon_gen3impl::set_nickname(const pkmn::pkstring &nickname)
    {
        if(nickname.length() > 10)
            throw std::runtime_error("Nicknames can have a maximum of 10 characters in Generation III.");

        conversions::export_gen3_text(nickname, _raw.pc.nickname, 10);
    }

    void team_pokemon_gen3impl::set_trainer_name(const pkmn::pkstring &trainer_name)
    {
        if(trainer_name.length() > 7)
            throw std::runtime_error("Trainer names can have a maximum of 7 characters in Generation III.");

        conversions::export_gen3_text(trainer_name, _raw.pc.otname, 7);
    }

    void team_pokemon_gen3impl::set_trainer_gender(const pkmn::pkstring &gender)
    {
        if(gender != "Male" and gender != "Female")
            throw std::runtime_error("Gender must be male or female.");

        if(gender == "Male") _raw.pc.origin_info &= ~(1<<31);
        else                 _raw.pc.origin_info |= (1<<31);
    }

    void team_pokemon_gen3impl::set_trainer_id(unsigned int id)
    {
        _raw.pc.ot_id = id;
    }

    void team_pokemon_gen3impl::set_trainer_public_id(unsigned short id)
    {
        _raw.pc.ot_pid = id;
    }

    void team_pokemon_gen3impl::set_trainer_secret_id(unsigned short id)
    {
        _raw.pc.ot_sid = id;
    }

    void team_pokemon_gen3impl::set_ball(const pkmn::pkstring &ball)
    {
        uint8_t ball_id = database::get_ball_id(ball);
        misc->origin_info = 0x87FF | (ball_id << 10);
    }

    // Met level of 0 means Pokemon was hatched
    void team_pokemon_gen3impl::set_met_level(unsigned int level)
    {
        if(level > 100)
            throw std::runtime_error("Level must be 0-100.");

        misc->origin_info = 0xFFC0 | (uint8_t(level) & 0x3F);
    }

    unsigned int team_pokemon_gen3impl::get_personality() const
    {
        return _raw.pc.personality;
    }

    unsigned int team_pokemon_gen3impl::get_level() const
    {
        return _raw.level;
    }

    unsigned int team_pokemon_gen3impl::get_experience() const
    {
        return growth->exp;
    }

    // Source: http://www.smogon.com/ingame/rng/pid_iv_creation#finding_gender_from_pid
    pkmn::pkstring team_pokemon_gen3impl::get_gender() const
    {
        float chance_male = _base->get_chance_male();
        float chance_female = _base->get_chance_female();

        if(chance_male + chance_female == 0.0) return "Genderless";
        else if(chance_male == 1.0) return "Male";
        else if(chance_female == 1.0) return "Female";
        else
        {
            /*
             * Gender is determined by personality
             * If the personality is below a certain number, the Pokemon will be female
             */
            if(chance_male == 0.875)     return (_raw.pc.personality > 30)  ? "Male" : "Female";
            else if(chance_male == 0.75) return (_raw.pc.personality > 63)  ? "Male" : "Female";
            else if(chance_male == 0.5)  return (_raw.pc.personality > 126) ? "Male" : "Female";
            else                         return (_raw.pc.personality > 190) ? "Male" : "Female";
        }
    }

    pkmn::nature_t team_pokemon_gen3impl::get_nature() const
    {
        return pkmn::nature_t(_raw.pc.personality % 25);
    }

    pkmn::pkstring team_pokemon_gen3impl::get_ability() const
    {
        int ability_slot = get_gen3_ability_slot(misc->iv_egg_ability) + 1; 
        query_stream.str("");
        query_stream << "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=" << t_pkmn->get_pokemon_id()
                     << " AND is_hidden=0 AND slot=" << ability_slot;
        SQLite::Statement query(db, query_stream.str().c_str());

        unsigned int ability_id;
        if((not query.executeStep()) and ability_slot == 2)
        {
            //Account for odd situation where slot=1 but there is no second ability
            query_stream.str("");
            query_stream << "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=" << t_pkmn->get_pokemon_id()
                         << " AND is_hidden=0 AND slot=1";
            ability_id = db.execAndGet(query_stream.str().c_str());
        }
        else ability_id = query.getColumn(0);

        return database::get_ability_name(ability_id);
    }

    // No hidden abilities in Generation III
    bool team_pokemon_gen3impl::using_hidden_ability() const
    {
        return false;
    }

    bool team_pokemon_gen3impl::is_shiny() const
    {
        return calculations::get_modern_shiny(_raw.pc.personality,
                                              _raw.pc.ot_sid,
                                              _raw.pc.ot_pid);
    }

    pkmn::dict<pkmn::pkstring, unsigned int> team_pokemon_gen3impl::get_stats() const
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

    pkmn::dict<pkmn::pkstring, unsigned int> team_pokemon_gen3impl::get_EVs() const
    {
        pkmn::dict<pkmn::pkstring, unsigned int> EVs;
        EVs["HP"]              = _raw.pc.ev_hp;
        EVs["Attack"]          = _raw.pc.ev_atk;
        EVs["Defense"]         = _raw.pc.ev_def;
        EVs["Speed"]           = _raw.pc.ev_spd;
        EVs["Special Attack"]  = _raw.pc.ev_spcl;
        EVs["Special Defense"] = _raw.pc.ev_spcl;

        return EVs;
    }

    pkmn::dict<pkmn::pkstring, unsigned int> team_pokemon_gen3impl::get_IVs() const
    {
        pkmn::dict<pkmn::pkstring, unsigned int> IVs;
        IVs["HP"]              = conversions::get_modern_IV(Stats::HP,      misc->iv_egg_ability);
        IVs["Attack"]          = conversions::get_modern_IV(Stats::ATTACK,  misc->iv_egg_ability);
        IVs["Defense"]         = conversions::get_modern_IV(Stats::DEFENSE, misc->iv_egg_ability);
        IVs["Speed"]           = conversions::get_modern_IV(Stats::SPEED,   misc->iv_egg_ability);
        IVs["Special Attack"]  = conversions::get_modern_IV(Stats::SPECIAL, misc->iv_egg_ability);
        IVs["Special Defense"] = conversions::get_modern_IV(Stats::SPECIAL, misc->iv_egg_ability);

        return IVs;
    }

    // NOTE: this affects many things
    void team_pokemon_gen3impl::set_personality(unsigned int personality)
    {
        _raw.pc.personality = personality;
    }

    // NOTE: this changes experience and stats
    void team_pokemon_gen3impl::set_level(unsigned int level)
    {
        if(level < 1 or level > 100)
            throw std::runtime_error("Level must be 1-100.");

        _set_level(level);
        _set_stats();
    }

    // NOTE: this affects level and stats
    void team_pokemon_gen3impl::set_experience(unsigned int experience)
    {
        _set_experience(experience);
        _set_stats();
    }

    // NOTE: this affects many things
    void team_pokemon_gen3impl::set_nature(const pkmn::pkstring &nature_name)
    {
        // Set personality to nearest value that results in given nature
        int8_t current_nature = (raw.pc.personality % 25);
        int8_t desired_nature = database::get_nature_id(nature_name);
        raw.pc.personality += (current_nature - desired_nature);
    }

    // NOTE: this affects many things
    void team_pokemon_gen3impl::set_ability(const pkmn::pkstring &ability)
    {
        uint8_t current_ability_slot = get_gen3_ability_slot(misc->iv_egg_ability) + 1; 
    }

    // No abilities in Generation II
    void team_pokemon_gen3impl::set_using_hidden_ability(bool value)
    {
        /* NOP */
    }

    // NOTE: this affects stats
    void team_pokemon_gen3impl::set_EV(const pkmn::pkstring &stat_name, unsigned int stat)
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
    void team_pokemon_gen3impl::set_IV(const pkmn::pkstring &stat_name, unsigned int stat)
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

    pkmn::pkstring team_pokemon_gen3impl::get_status() const
    {
        return retro_statuses.at(_raw.pc.status, "OK");
    }

    item::sptr team_pokemon_gen3impl::get_held_item() const
    {
        return item::make(database::get_item_id(_raw.pc.held_item, _game_id), _game_id);
    }

    void team_pokemon_gen3impl::set_status(const pkmn::pkstring &status)
    {
        if(not reverse_retro_statuses.has_key(status))
            throw std::runtime_error("Invalid status given.");

        _raw.pc.status = reverse_retro_statuses.at(status);
    }

    void team_pokemon_gen3impl::set_held_item(const pkmn::pkstring &item_name)
    {
        _raw.pc.held_item = database::get_item_game_index(item_name,
                                                         database::get_version_id(_game_id));
    }

    move::sptr team_pokemon_gen3impl::get_move(unsigned int pos) const
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        return move::make(_raw.pc.moves[pos-1], _game_id);
    }

    void team_pokemon_gen3impl::get_moves(pkmn::moveset &moves) const
    {
        moves.clear();
        for(size_t i = 0; i < 4; i++) moves.push_back(get_move(i+1));
    }

    unsigned int team_pokemon_gen3impl::get_move_PP(unsigned int pos) const
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        return _raw.pc.move_pps[pos-1];
    }

    void team_pokemon_gen3impl::get_move_PPs(std::vector<unsigned int> &move_PPs) const
    {
        move_PPs.clear();
        for(size_t i = 0; i < 4; i++) move_PPs.push_back(_raw.pc.move_pps[pos]); 
    }

    // No markings in Generation II
    pkmn::markings team_pokemon_gen3impl::get_markings() const
    {
        return pkmn::markings();
    }

    // No markings in Generation II
    void team_pokemon_gen3impl::set_markings(const pkmn::markings &mark)
    {
        /* NOP */
    }

    // Original game not tracked in Generation II
    unsigned int team_pokemon_gen3impl::get_original_game() const
    {
        return _game_id;
    }

    // No abilities in Generation II
    unsigned int team_pokemon_gen3impl::get_ability_id() const
    {
        return Abilities::NONE;
    }

    // No held items in Generation II
    unsigned int team_pokemon_gen3impl::get_item_id() const
    {
        return Items::NONE;
    }

    // No natures in Generation II
    unsigned int team_pokemon_gen3impl::get_nature_id() const
    {
        return Natures::NONE;
    }

    void team_pokemon_gen3impl::_set_experience(const uint32_t exp)
    {
        uint32_t e = exp;

        _raw.pc.exp[0] = (e /= 65536);
        _raw.pc.exp[1] = (e /= 256);
        _raw.pc.exp[2] =  e;

        _raw.pc.level = database::get_level(_species_id, exp);
        _raw.level = _raw.pc.level;
    }

    void team_pokemon_gen3impl::_set_level(const uint8_t level)
    {
        _raw.pc.level = level;
        _raw.level = level;

        uint32_t exp = database::get_experience(_species_id, level);
        _raw.pc.exp[0] = (exp /= 65536);
        _raw.pc.exp[1] = (exp /= 256);
        _raw.pc.exp[2] =  exp;
    }

    void team_pokemon_gen3impl::_set_stats()
    {
        pkmn::dict<pkmn::pkstring, unsigned int> stats = _base->get_base_stats();
        pkmn::dict<pkmn::pkstring, unsigned int> IVs = get_IVs();

        _raw.max_hp = int(floor((((double(IVs["HP"]) + double(stats["HP"]) + (pow(_raw.pc.ev_hp,0.5)/8.0)
                            + 50.0) * double(_raw.level))/50.0) + 10.0));
        _raw.current_hp = _raw.max_hp;

        _raw.atk    = int(ceil((((double(IVs["Attack"]) + double(stats["Attack"]) + (pow(_raw.pc.ev_atk,0.5)/8.0))
                                * double(_raw.level))/50.0) + 5.0));

        _raw.def    = int(ceil((((double(IVs["Defense"]) + double(stats["Defense"]) + (pow(_raw.pc.ev_def,0.5)/8.0))
                                * double(_raw.level))/50.0) + 5.0));

        _raw.spd    = int(ceil((((double(IVs["Speed"]) + double(stats["Speed"]) + (pow(_raw.pc.ev_spd,0.5)/8.0))
                                * double(_raw.level))/50.0) + 5.0));

        _raw.spatk  = int(ceil((((double(IVs["Special"]) + double(stats["Special Attack"]) + (pow(_raw.pc.ev_spcl,0.5)/8.0))
                                * double(_raw.level))/50.0) + 5.0));

        _raw.spdef  = int(ceil((((double(IVs["Special"]) + double(stats["Special Defense"]) + (pow(_raw.pc.ev_spcl,0.5)/8.0))
                                * double(_raw.level))/50.0) + 5.0));
    }

    void team_pokemon_gen3impl::_set_unown_form_from_IVs()
    {
        pkmn::dict<pkmn::pkstring, unsigned int> IVs = get_IVs();
        uint32_t unown_form = calculations::get_gen3_unown_form(IVs["Attack"], IVs["Defense"],
                                                                IVs["Speed"],  IVs["Special"]);

        _unown_signal(unown_form);
    }

    void team_pokemon_gen3impl::_set_unown_IVs_from_form()
    {
        uint32_t form_id = _base->get_form_id();
        uint8_t letter = (form_id == Species::UNOWN) ? 0 : (form_id - Forms::Unown::B + 1);

        // Some parts of the IV are cut off, so give highest possible value
        uint8_t ivATK  = 0x9 | ((letter & 0xC0) >> 5);
        uint8_t ivDEF  = 0x9 | ((letter & 0x30) >> 3);
        uint8_t ivSPD  = 0x9 | ((letter & 0xC)  >> 1);
        uint8_t ivSPCL = 0x9 | ((letter & 0x3)  << 1);

        conversions::set_retro_IV(database::get_stat_id(Stats::ATTACK),  _raw.pc.iv_data, ivATK);
        conversions::set_retro_IV(database::get_stat_id(Stats::DEFENSE), _raw.pc.iv_data, ivDEF);
        conversions::set_retro_IV(database::get_stat_id(Stats::SPEED),   _raw.pc.iv_data, ivSPD);
        conversions::set_retro_IV(database::get_stat_id(Stats::SPECIAL), _raw.pc.iv_data, ivSPCL);

        _set_stats();
    }
}
