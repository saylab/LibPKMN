/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

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

        pkmn::pkstring nickname = boost::algorithm::to_upper_copy(_base->get_species().std_wstring());
        conversions::export_gen3_text(nickname, _raw.pc.nickname, 10);

        _raw.pc.language = 0x202; // English
        conversions::export_gen3_text("LIBPKMN", _raw.pc.otname, 7);
        // TODO: checksum

        _growth = &(_raw.pc.blocks.growth);
        _attacks = &(_raw.pc.blocks.attacks);
        _effort = &(_raw.pc.blocks.effort);
        _misc = &(_raw.pc.blocks.misc);

        _growth->species = database::get_pokemon_game_index(_species_id, _game_id);
        _growth->held_item = 0;
        // experience will be determined by level
        _growth->pp_up = 0; // TODO
        _growth->friendship = 70;
        _growth->unknown_0xA; // TODO: check for expected value
        _attacks->moves[0] = move1;
        _attacks->moves[1] = move2;
        _attacks->moves[2] = move3;
        _attacks->moves[3] = move4;
        for(size_t i = 0; i < 4; i++)
            _attacks->move_pps[i] = database::get_move_pp(_attacks->moves[i]);
        do
        {
            _effort->ev_hp    = _prng->lcrng() % 256;
            _effort->ev_atk   = _prng->lcrng() % 256;
            _effort->ev_def   = _prng->lcrng() % 256;
            _effort->ev_spd   = _prng->lcrng() % 256;
            _effort->ev_spatk = _prng->lcrng() % 256;
            _effort->ev_spdef = _prng->lcrng() % 256;
        }
        while((_effort->ev_hp  + _effort->ev_atk   + _effort->ev_def +
               _effort->ev_spd + _effort->ev_spatk + _effort->ev_spdef) > 510);
        // TODO: contest stats data structure
        // TODO: Pokerus
        _misc->met_location = 255; // Fateful encounter

        // Origin info
        _misc->origin_info = (level & 0x7F);
        _misc->origin_info |= (uint8_t(libpkmn_game_to_hometown(_game_id)) << 6);
        _misc->origin_info |= (uint8_t(Balls::LUXURY_BALL) << 10);
        // Don't set trainer gender, male by default

        _misc->iv_egg_ability = _prng->lcrng();
        _misc->iv_egg_ability &= ~(1<<30); // Not an egg

        _misc->ribbons_obedience = 0;
        _misc->ribbons_obedience |= (1<<31); // Mew and Deoxys will be obedient

        _raw.condition = 0; // OK
        _set_level(level);
        _set_stats();
    }

    pkmn::pkstring team_pokemon_gen3impl::get_nickname() const
    {
        return conversions::import_gen3_text(_raw.pc.nickname, 10);
    }

    pkmn::pkstring team_pokemon_gen3impl::get_trainer_name() const
    {
        return conversions::import_gen3_text(_raw.pc.otname, 7);
    }

    pkmn::pkstring team_pokemon_gen3impl::get_trainer_gender() const
    {
        return (_misc->origin_info & (1<<31)) ? "Female" : "Male";
    }

    unsigned int team_pokemon_gen3impl::get_trainer_id() const
    {
        return _raw.pc.ot_id;
    }

    unsigned short team_pokemon_gen3impl::get_trainer_public_id() const
    {
        return _raw.pc.ot_pid;
    }

    unsigned short team_pokemon_gen3impl::get_trainer_secret_id() const
    {
        return _raw.pc.ot_sid;
    }

    pkmn::pkstring team_pokemon_gen3impl::get_ball() const
    {
        return database::get_ball_name((_misc->origin_info & 0x3C00) >> 11);
    }

    unsigned int team_pokemon_gen3impl::get_met_level() const
    {
        return (_misc->origin_info & 0x3F00) >> 8;
    }

    void team_pokemon_gen3impl::set_original_game(unsigned int game)
    {
        if(database::get_generation(game) != 3)
            throw std::runtime_error("Original game must be from Generation III.");

        _misc->origin_info = 0xFC3F | (uint8_t(libpkmn_game_to_hometown(game)) << 6);
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
        if(gender.std_string() != "Male" and gender.std_string() != "Female")
            throw std::runtime_error("Gender must be male or female.");

        if(gender == "Male") _misc->origin_info &= ~(1<<31);
        else                 _misc->origin_info |= (1<<31);
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
        _misc->origin_info = 0x87FF | (ball_id << 10);
    }

    // Met level of 0 means Pokemon was hatched
    void team_pokemon_gen3impl::set_met_level(unsigned int level)
    {
        if(level > 100)
            throw std::runtime_error("Level must be 0-100.");

        _misc->origin_info = 0xFFC0 | (uint8_t(level) & 0x3F);
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
        return _growth->exp;
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
        return pkmn::nature_t(_raw.pc.personality % 24);
    }

    pkmn::pkstring team_pokemon_gen3impl::get_ability() const
    {
        int ability_slot = get_gen3_ability_slot(_misc->iv_egg_ability) + 1;

        std::ostringstream query_stream;
        query_stream << "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=" << _species_id
                     << " AND is_hidden=0 AND slot=" << ability_slot;
        SQLite::Statement query(*_db, query_stream.str().c_str());

        unsigned int ability_id;
        if((not query.executeStep()) and ability_slot == 2)
        {
            //Account for odd situation where slot=1 but there is no second ability
            query_stream.str("");
            query_stream << "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=" << _species_id
                         << " AND is_hidden=0 AND slot=1";
            ability_id = _db->execAndGet(query_stream.str().c_str());
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
        EVs["HP"]              = _effort->ev_hp;
        EVs["Attack"]          = _effort->ev_atk;
        EVs["Defense"]         = _effort->ev_def;
        EVs["Speed"]           = _effort->ev_spd;
        EVs["Special Attack"]  = _effort->ev_spatk;
        EVs["Special Defense"] = _effort->ev_spdef;

        return EVs;
    }

    pkmn::dict<pkmn::pkstring, unsigned int> team_pokemon_gen3impl::get_IVs() const
    {
        pkmn::dict<pkmn::pkstring, unsigned int> IVs;
        IVs["HP"]              = conversions::get_modern_IV(Stats::HP,      _misc->iv_egg_ability);
        IVs["Attack"]          = conversions::get_modern_IV(Stats::ATTACK,  _misc->iv_egg_ability);
        IVs["Defense"]         = conversions::get_modern_IV(Stats::DEFENSE, _misc->iv_egg_ability);
        IVs["Speed"]           = conversions::get_modern_IV(Stats::SPEED,   _misc->iv_egg_ability);
        IVs["Special Attack"]  = conversions::get_modern_IV(Stats::SPECIAL, _misc->iv_egg_ability);
        IVs["Special Defense"] = conversions::get_modern_IV(Stats::SPECIAL, _misc->iv_egg_ability);

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
        int8_t current_nature = (_raw.pc.personality % 24);
        int8_t desired_nature = database::get_nature_id(nature_name);
        _raw.pc.personality += (current_nature - desired_nature);
    }

    // NOTE: this affects many things
    void team_pokemon_gen3impl::set_ability(const pkmn::pkstring &ability)
    {
        // Check to see if Pokemon has this ability
        std::ostringstream query_stream;
        query_stream << "SELECT slot FROM pokemon_abilities WHERE ability_id="
                     << database::get_ability_id(ability) << " AND is_hidden=0"
                     << " AND pokemon_id=" << _species_id;
        SQLite::Statement query(*_db, query_stream.str().c_str());
        if(query.executeStep())
        {
            uint8_t ability_slot = (unsigned int)(query.getColumn(0)) - 1;
            uint8_t current_ability_slot = get_gen3_ability_slot(_misc->iv_egg_ability) + 1;

            if(ability_slot != current_ability_slot) _raw.pc.personality++;
        }
        else throw std::runtime_error("Invalid ability for this Pokemon.");
    }

    // No hidden abilities in Generation II
    void team_pokemon_gen3impl::set_using_hidden_ability(bool value)
    {
        /* NOP */
    }

    // NOTE: this affects stats
    void team_pokemon_gen3impl::set_EV(const pkmn::pkstring &stat_name, unsigned int stat)
    {
        if(stat_name == "Special")
            throw std::runtime_error("The Special stat is only in Generations I-II.");
        if(stat > 255)
            throw std::runtime_error("EV's have a maximum value of 255 in Generation III.");

        uint16_t sum_of_rest;

        switch(database::get_stat_id(stat_name)) // Will throw if stat_name is invalid
        {
            case Stats::HP:
                sum_of_rest = _effort->ev_atk + _effort->ev_def + _effort->ev_spd + _effort->ev_spatk
                            + _effort->ev_spdef;
                if((sum_of_rest + stat) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                             % (510 - sum_of_rest)));

                _effort->ev_hp = stat;
                break;

            case Stats::ATTACK:
                sum_of_rest = _effort->ev_hp + _effort->ev_def + _effort->ev_spd + _effort->ev_spatk
                            + _effort->ev_spdef;
                if((sum_of_rest + stat) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                                 % (510 - sum_of_rest)));

                _effort->ev_atk = stat;
                break;

            case Stats::DEFENSE:
                sum_of_rest = _effort->ev_hp + _effort->ev_atk + _effort->ev_spd + _effort->ev_spatk
                            + _effort->ev_spdef;
                if((sum_of_rest + stat) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                                 % (510 - sum_of_rest)));

                _effort->ev_def = stat;
                break;

            case Stats::SPEED:
                sum_of_rest = _effort->ev_hp + _effort->ev_atk + _effort->ev_def + _effort->ev_spatk
                            + _effort->ev_spdef;
                if((sum_of_rest + stat) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                                 % (510 - sum_of_rest)));

                _effort->ev_spd = stat;
                break;

            case Stats::SPECIAL_ATTACK:
                sum_of_rest = _effort->ev_hp + _effort->ev_atk + _effort->ev_def + _effort->ev_spd
                            + _effort->ev_spdef;
                if((sum_of_rest + stat) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                                 % (510 - sum_of_rest)));

                _effort->ev_spatk = stat;
                break;

            default: // Stats::SPECIAL_DEFENSE
                sum_of_rest = _effort->ev_hp + _effort->ev_atk + _effort->ev_def + _effort->ev_spd
                            + _effort->ev_spatk;
                if((sum_of_rest + stat) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                                 % (510 - sum_of_rest)));

                _effort->ev_spdef = stat;
                break;
        }

        _set_stats();
    }

    // NOTE: this affects stats
    void team_pokemon_gen3impl::set_IV(const pkmn::pkstring &stat_name, unsigned int stat)
    {
        if(stat_name == "Special")
            throw std::runtime_error("The Special stat is only in Generations I-II.");
        if(stat > 31)
            throw std::runtime_error("IV's have a maximum value of 31 in Generation III.");

        // Will throw if stat_name is invalid
        conversions::set_modern_IV(database::get_stat_id(stat_name), _misc->iv_egg_ability, stat);

        _set_stats();
    }

    pkmn::pkstring team_pokemon_gen3impl::get_status() const
    {
        return conversions::retro_statuses.at(_raw.condition, "OK");
    }

    item::sptr team_pokemon_gen3impl::get_held_item() const
    {
        return item::make(database::get_item_id(_growth->held_item, _game_id), _game_id);
    }

    void team_pokemon_gen3impl::set_status(const pkmn::pkstring &status)
    {
        if(not conversions::reverse_retro_statuses.has_key(status))
            throw std::runtime_error("Invalid status given.");

        _raw.condition = conversions::reverse_retro_statuses.at(status);
    }

    void team_pokemon_gen3impl::set_held_item(const pkmn::pkstring &item_name)
    {
        _growth->held_item = database::get_item_game_index(item_name,
                                                           database::get_version_name(_game_id));
    }

    move::sptr team_pokemon_gen3impl::get_move(unsigned int pos) const
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        return move::make(_attacks->moves[pos-1], _game_id);
    }

    void team_pokemon_gen3impl::get_moves(pkmn::moveset_t &moves) const
    {
        moves.clear();
        for(size_t i = 0; i < 4; i++) moves.push_back(get_move(i+1));
    }

    unsigned int team_pokemon_gen3impl::get_move_PP(unsigned int pos) const
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        return _attacks->move_pps[pos-1];
    }

    void team_pokemon_gen3impl::get_move_PPs(std::vector<unsigned int> &move_PPs) const
    {
        move_PPs.clear();
        for(size_t i = 0; i < 4; i++) move_PPs.push_back(_attacks->move_pps[i+1]); 
    }

    void team_pokemon_gen3impl::set_move(const pkmn::pkstring &move_name, unsigned int pos)
    {   
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        // Make sure move exists in given generation
        std::ostringstream query_stream;
        query_stream << "SELECT generation_id FROM moves WHERE id="
                     << database::get_move_id(move_name);
        SQLite::Statement query(*_db, query_stream.str().c_str());
        if(query.executeStep()) _attacks->moves[pos-1] = int(query.getColumn(0));
        else throw std::runtime_error("This move does not exist in Generation III.");
    }   

    void team_pokemon_gen3impl::set_move_PP(unsigned int PP, unsigned int pos)
    {   
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        if(PP <= database::get_move_pp(_attacks->moves[pos-1])) _attacks->move_pps[pos-1] = PP; 
        else throw std::runtime_error("This move PP is invalid.");
    }

    pkmn::markings team_pokemon_gen3impl::get_markings() const
    {
        return _raw.pc.markings;
    }

    void team_pokemon_gen3impl::set_markings(const pkmn::markings &mark)
    {
        pkmn::markings m = mark;
        _raw.pc.markings = m;
    }

    pkmn::ribbons team_pokemon_gen3impl::get_ribbons() const
    {
        pkmn::ribbons rib;
        rib.hoenn = _misc->ribbons_obedience;

        return rib;
    }

    void team_pokemon_gen3impl::set_ribbons(const pkmn::ribbons &rib)
    {
        pkmn::hoenn_ribbons hoenn = rib.hoenn;
        _misc->ribbons_obedience = hoenn;
    }

    unsigned int team_pokemon_gen3impl::get_original_game_id() const
    {
        return hometown_to_libpkmn_game((_misc->origin_info &= ~0xFC3F) >> 6);
    }

    unsigned int team_pokemon_gen3impl::get_ability_id() const
    {
        return database::get_ability_id(get_ability());
    }

    unsigned int team_pokemon_gen3impl::get_item_id() const
    {
        return database::get_item_id(_growth->held_item, _game_id);
    }

    unsigned int team_pokemon_gen3impl::get_nature_id() const
    {
        return (_raw.pc.personality % 24);
    }

    void* team_pokemon_gen3impl::get_native()
    {
        return &_raw;
    }

    void team_pokemon_gen3impl::_set_experience(const uint32_t exp)
    {
        _growth->exp = exp;
        _raw.level = database::get_level(_species_id, exp);
    }

    void team_pokemon_gen3impl::_set_level(const uint8_t level)
    {
        _raw.level = level;
        _growth->exp = database::get_experience(_species_id, level);
    }

    void team_pokemon_gen3impl::_set_stats()
    {
        pkmn::dict<pkmn::pkstring, unsigned int> stats = _base->get_base_stats();
        pkmn::dict<pkmn::pkstring, unsigned int> IVs = get_IVs();
        pkmn::nature_t nature = get_nature();

        _raw.max_hp = int(floor(((double(IVs["HP"]) + (2.0*double(stats["HP"]))
                                  + (0.25*double(_effort->ev_hp)) + 100.0)
                                * double(_raw.level))/100.0 + 10.0));
        _raw.current_hp = _raw.max_hp;

        _raw.atk    = int(ceil(((((double(IVs["Attack"]) + 2.0*double(stats["Attack"])
                                   + 0.25*double(_effort->ev_atk))
                             * double(_raw.level))/100.0) + 5.0) * nature["Attack"]));

        _raw.def    = int(ceil(((((double(IVs["Defense"]) + 2.0*double(stats["Defense"])
                                   + 0.25*double(_effort->ev_def))
                             * double(_raw.level))/100.0) + 5.0) * nature["Defense"]));

        _raw.spd    = int(ceil(((((double(IVs["Speed"]) + 2.0*double(stats["Speed"])
                                   + 0.25*double(_effort->ev_spd))
                             * double(_raw.level))/100.0) + 5.0) * nature["Speed"]));

        _raw.spatk  = int(ceil(((((double(IVs["Special Attack"]) + 2.0*double(stats["Special Attack"])
                                   + 0.25*double(_effort->ev_spatk))
                             * double(_raw.level))/100.0) + 5.0) * nature["Special Attack"]));

        _raw.spdef  = int(ceil(((((double(IVs["Special Defense"]) + 2.0*double(stats["Special Defense"])
                                   + 0.25*double(_effort->ev_spdef))
                             * double(_raw.level))/100.0) + 5.0) * nature["Special Defense"]));
    }
}
