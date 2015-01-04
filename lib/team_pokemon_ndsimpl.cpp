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
#include "team_pokemon_ndsimpl.hpp"

#include "conversions/text.hpp"
#include "conversions/utils.hpp"

namespace pkmn
{
    team_pokemon_ndsimpl::team_pokemon_ndsimpl(base_pokemon::sptr base, uint8_t level,
                                               uint8_t move1, uint8_t move2,
                                               uint8_t move3, uint8_t move4):
        team_pokemon_impl(base)
    {
        _blockA = &(_raw.pc.blocks.blockA);
        _blockB = &(_raw.pc.blocks.blockB);
        _blockC = &(_raw.pc.blocks.blockC);
        _blockD = &(_raw.pc.blocks.blockD);

        if(get_generation() == 4)
        {
            conversions::export_gen4_text(_base->get_species(), _blockC->nickname, 10);
            conversions::export_gen4_text("LIBPKMN",            _blockD->otname,   7);
        }
        else
        {
            conversions::export_modern_text(_base->get_species(), _blockC->nickname, 10);
            conversions::export_modern_text("LIBPKMN",            _blockD->otname,   7);
        }

        /*
         * Populate native struct
         */
        _raw.pc.personality = _prng->lcrng();
        _blockA->species = database::get_pokemon_game_index(_species_id, _game_id);
        _blockA->held_item = Items::NONE;
        _blockA->ot_id = _prng->lcrng();
        // Experience set by level
        _blockA->friendship = 70;
        _blockA->ability = (_raw.pc.personality % 2) ? database::get_ability_id(_base->get_abilities().second)
                                                     : database::get_ability_id(_base->get_abilities().first);
        _blockA->country = 0x2; // English
        do
        {
            _blockA->ev_hp    = _prng->lcrng() % 256;
            _blockA->ev_atk   = _prng->lcrng() % 256;
            _blockA->ev_def   = _prng->lcrng() % 256;
            _blockA->ev_spd   = _prng->lcrng() % 256;
            _blockA->ev_spatk = _prng->lcrng() % 256;
            _blockA->ev_spdef = _prng->lcrng() % 256;
        }
        while((_blockA->ev_hp  + _blockA->ev_atk   + _blockA->ev_def +
               _blockA->ev_spd + _blockA->ev_spatk + _blockA->ev_spdef) > 510);
        // TODO: contest stat data structure
        _blockB->moves[0] = move1;
        _blockB->moves[1] = move2;
        _blockB->moves[2] = move3;
        _blockB->moves[3] = move4;
        for(size_t i = 0; i < 4; i++)
            _blockB->move_pps[i] = database::get_move_pp(_blockB->moves[i]);
        _blockB->iv_isegg_isnicknamed = _prng->lcrng();
        _blockB->iv_isegg_isnicknamed &= ~(2<<30); // Not an egg, not nicknamed

        // Encounter info
        _blockB->form_encounterinfo = 1; // Fateful encounter
        _set_default_gender();

        if(get_generation() == 5)
            _blockB->nature = (_raw.pc.personality % 24);

        if(_game_id == Versions::PLATINUM)
        {
            _blockB->eggmet_plat = 2008; // Distant land
            _blockB->met_plat = 2008; // Distant land
        }
        _blockC->hometown = libpkmn_game_to_hometown(_game_id);
        // TODO: check for expected values in "unknown" fields
        // TODO: met dates
        if(_game_id == Versions::DIAMOND or _game_id == Versions::PEARL)
        {
            _blockD->eggmet_dp = 2008; // Distant land
            _blockD->met_dp = 2008; // Distant land
        }
        // TODO: Pokerus
        if(_game_id != Versions::HEARTGOLD and _game_id != Versions::SOULSILVER)
            _blockD->ball = Balls::LUXURY_BALL;
        _blockD->metlevel_otgender = (level |= ~(1<<31));
        _blockD->encounter_info = 0; // Special event
        if(_game_id == Versions::HEARTGOLD or _game_id == Versions::SOULSILVER)
            _blockD->ball_hgss = Balls::LUXURY_BALL;

        _set_level(level);
        _set_stats();
    }

    pkmn::pkstring team_pokemon_ndsimpl::get_nickname() const
    {
        return (get_generation() == 4) ? conversions::import_gen4_text(_blockC->nickname, 10)
                                       : conversions::import_modern_text(_blockC->nickname, 10);
    }

    pkmn::pkstring team_pokemon_ndsimpl::get_trainer_name() const
    {
        return (get_generation() == 4) ? conversions::import_gen4_text(_blockD->otname, 7)
                                       : conversions::import_modern_text(_blockD->otname, 7);
    }

    pkmn::pkstring team_pokemon_ndsimpl::get_trainer_gender() const
    {
        return (_blockD->metlevel_otgender & (1<<31)) ? "Female" : "Male";
    }

    unsigned int team_pokemon_ndsimpl::get_trainer_id() const
    {
        return _blockA->ot_id;
    }

    unsigned short team_pokemon_ndsimpl::get_trainer_public_id() const
    {
        return _blockA->ot_pid;
    }

    unsigned short team_pokemon_ndsimpl::get_trainer_secret_id() const
    {
        return _blockA->ot_sid;
    }

    pkmn::pkstring team_pokemon_ndsimpl::get_ball() const
    {
        if(_game_id == Versions::HEARTGOLD or _game_id == Versions::SOULSILVER)
            return database::get_ball_name(_blockD->ball_hgss);
        else return database::get_ball_name(_blockD->ball);
    }

    unsigned int team_pokemon_ndsimpl::get_met_level() const
    {
        return _blockD->metlevel_otgender & ~(1<<7);
    }

    void team_pokemon_ndsimpl::set_original_game(unsigned int game)
    {
        uint8_t generation = database::get_generation(game);
        if(generation < 3 or generation > get_generation())
            throw std::runtime_error("Invalid game."); // TODO: more detail

        _blockC->hometown = libpkmn_game_to_hometown(game);
    }

    void team_pokemon_ndsimpl::set_nickname(const pkmn::pkstring &nickname)
    {
        if(nickname.length() > 10)
            throw std::runtime_error("Nicknames can have a maximum of 10 characters in Generation III.");

        if(get_generation() == 4)
            conversions::export_gen4_text(nickname, _blockC->nickname, 10);
        else
            conversions::export_modern_text(nickname, _blockC->nickname, 10);

        if(nickname == _base->get_species())
            _blockB->iv_isegg_isnicknamed &= ~(1<<31);
        else
            _blockB->iv_isegg_isnicknamed |= (1<<31);
    }

    void team_pokemon_ndsimpl::set_trainer_name(const pkmn::pkstring &trainer_name)
    {
        if(trainer_name.length() > 7)
            throw std::runtime_error("Trainer names can have a maximum of 7 characters in Generation III.");

        if(get_generation() == 4)
            conversions::export_gen4_text(trainer_name, _blockD->otname, 7);
        else
            conversions::export_modern_text(trainer_name, _blockD->otname, 7);
    }

    void team_pokemon_ndsimpl::set_trainer_gender(const pkmn::pkstring &gender)
    {
        if(gender.std_string() != "Male" and gender.std_string() != "Female")
            throw std::runtime_error("Gender must be male or female.");

        if(gender == "Male")
            _blockD->metlevel_otgender &= ~(1<<31);
        else
            _blockD->metlevel_otgender |= (1<<31);
    }

    void team_pokemon_ndsimpl::set_trainer_id(unsigned int id)
    {
        _blockA->ot_id = id;
    }

    void team_pokemon_ndsimpl::set_trainer_public_id(unsigned short id)
    {
        _blockA->ot_pid = id;
    }

    void team_pokemon_ndsimpl::set_trainer_secret_id(unsigned short id)
    {
        _blockA->ot_sid = id;
    }

    void team_pokemon_ndsimpl::set_ball(const pkmn::pkstring &ball)
    {
        // TODO: generation check
        if(_game_id == Versions::HEARTGOLD or _game_id == Versions::SOULSILVER)
            _blockD->ball_hgss = database::get_ball_id(ball);
        else _blockD->ball = database::get_ball_id(ball);
    }

    // Met level of 0 means Pokemon was hatched
    void team_pokemon_ndsimpl::set_met_level(unsigned int level)
    {
        if(level > 100)
            throw std::runtime_error("Level must be 0-100.");

        _blockD->metlevel_otgender &= (1<<7);
        _blockD->metlevel_otgender |= (level & ~(1<<7));
    }

    unsigned int team_pokemon_ndsimpl::get_personality() const
    {
        return _raw.pc.personality;
    }

    unsigned int team_pokemon_ndsimpl::get_level() const
    {
        return _raw.level;
    }

    unsigned int team_pokemon_ndsimpl::get_experience() const
    {
        return _blockA->exp;
    }

    pkmn::pkstring team_pokemon_ndsimpl::get_gender() const
    {
        if(_blockB->form_encounterinfo & (1<<1))      return "Female";
        else if(_blockB->form_encounterinfo & (1<<2)) return "Genderless";
        else                                          return "Male";
    }

    pkmn::nature_t team_pokemon_ndsimpl::get_nature() const
    {
        if(get_generation() == 5) return pkmn::nature_t(_blockB->nature);
        else return pkmn::nature_t(_raw.pc.personality % 24);
    }

    pkmn::pkstring team_pokemon_ndsimpl::get_ability() const
    {
        return database::get_ability_name(_blockA->ability);
    }

    // No hidden abilities in Generation III
    bool team_pokemon_ndsimpl::using_hidden_ability() const
    {
        if(get_generation() == 5)
            return (_blockB->gen5_info & 0x1);
        else return false;
    }

    bool team_pokemon_ndsimpl::is_shiny() const
    {
        return calculations::get_modern_shiny(_raw.pc.personality,
                                              _blockA->ot_sid,
                                              _blockA->ot_pid);
    }

    pkmn::dict<pkmn::pkstring, unsigned int> team_pokemon_ndsimpl::get_stats() const
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

    pkmn::dict<pkmn::pkstring, unsigned int> team_pokemon_ndsimpl::get_EVs() const
    {
        pkmn::dict<pkmn::pkstring, unsigned int> EVs;
        EVs["HP"]              = _blockA->ev_hp;
        EVs["Attack"]          = _blockA->ev_atk;
        EVs["Defense"]         = _blockA->ev_def;
        EVs["Speed"]           = _blockA->ev_spd;
        EVs["Special Attack"]  = _blockA->ev_spatk;
        EVs["Special Defense"] = _blockA->ev_spdef;

        return EVs;
    }

    pkmn::dict<pkmn::pkstring, unsigned int> team_pokemon_ndsimpl::get_IVs() const
    {
        pkmn::dict<pkmn::pkstring, unsigned int> IVs;
        IVs["HP"]              = conversions::get_modern_IV(Stats::HP,      _blockB->iv_isegg_isnicknamed);
        IVs["Attack"]          = conversions::get_modern_IV(Stats::ATTACK,  _blockB->iv_isegg_isnicknamed);
        IVs["Defense"]         = conversions::get_modern_IV(Stats::DEFENSE, _blockB->iv_isegg_isnicknamed);
        IVs["Speed"]           = conversions::get_modern_IV(Stats::SPEED,   _blockB->iv_isegg_isnicknamed);
        IVs["Special Attack"]  = conversions::get_modern_IV(Stats::SPECIAL, _blockB->iv_isegg_isnicknamed);
        IVs["Special Defense"] = conversions::get_modern_IV(Stats::SPECIAL, _blockB->iv_isegg_isnicknamed);

        return IVs;
    }

    // NOTE: this affects many things
    void team_pokemon_ndsimpl::set_personality(unsigned int personality)
    {
        _raw.pc.personality = personality;
    }

    // NOTE: this changes experience and stats
    void team_pokemon_ndsimpl::set_level(unsigned int level)
    {
        if(level < 1 or level > 100)
            throw std::runtime_error("Level must be 1-100.");

        _set_level(level);
        _set_stats();
    }

    // NOTE: this affects level and stats
    void team_pokemon_ndsimpl::set_experience(unsigned int experience)
    {
        _set_experience(experience);
        _set_stats();
    }

    // NOTE: this affects many things in Generation IV
    void team_pokemon_ndsimpl::set_nature(const pkmn::pkstring &nature_name)
    {
        if(get_generation() == 4)
        {
            // Set personality to nearest value that results in given nature
            int8_t current_nature = (_raw.pc.personality % 24);
            int8_t desired_nature = database::get_nature_id(nature_name);
            _raw.pc.personality += (current_nature - desired_nature);
        }
        else _blockB->nature = database::get_nature_id(nature_name);
    }

    void team_pokemon_ndsimpl::set_ability(const pkmn::pkstring &ability)
    {
        // TODO: check generation
        _blockA->ability = database::get_ability_id(ability);
        if(ability == _base->get_hidden_ability()) _blockB->gen5_info |= 1;
    }

    // No hidden abilities in Generation IV
    void team_pokemon_ndsimpl::set_using_hidden_ability(bool value)
    {
        if(get_generation() == 5)
        {
            if(value) _blockB->gen5_info |= 0x1;
            else      _blockB->gen5_info &= ~(0x1);
        }
    }

    // NOTE: this affects stats
    void team_pokemon_ndsimpl::set_EV(const pkmn::pkstring &stat_name, unsigned int stat)
    {
        if(stat_name == "Special")
            throw std::runtime_error("The Special stat is only in Generations I-II.");
        if(stat > 255)
            throw std::runtime_error("EV's have a maximum value of 255 in Generations IV-V.");

        uint16_t sum_of_rest;

        switch(database::get_stat_id(stat_name)) // Will throw if stat_name is invalid
        {
            case Stats::HP:
                sum_of_rest = _blockA->ev_atk + _blockA->ev_def + _blockA->ev_spd + _blockA->ev_spatk
                            + _blockA->ev_spdef;
                if((sum_of_rest + stat) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                             % (510 - sum_of_rest)));

                _blockA->ev_hp = stat;
                break;

            case Stats::ATTACK:
                sum_of_rest = _blockA->ev_hp + _blockA->ev_def + _blockA->ev_spd + _blockA->ev_spatk
                            + _blockA->ev_spdef;
                if((sum_of_rest + stat) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                                 % (510 - sum_of_rest)));

                _blockA->ev_atk = stat;
                break;

            case Stats::DEFENSE:
                sum_of_rest = _blockA->ev_hp + _blockA->ev_atk + _blockA->ev_spd + _blockA->ev_spatk
                            + _blockA->ev_spdef;
                if((sum_of_rest + stat) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                                 % (510 - sum_of_rest)));

                _blockA->ev_def = stat;
                break;

            case Stats::SPEED:
                sum_of_rest = _blockA->ev_hp + _blockA->ev_atk + _blockA->ev_def + _blockA->ev_spatk
                            + _blockA->ev_spdef;
                if((sum_of_rest + stat) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                                 % (510 - sum_of_rest)));

                _blockA->ev_spd = stat;
                break;

            case Stats::SPECIAL_ATTACK:
                sum_of_rest = _blockA->ev_hp + _blockA->ev_atk + _blockA->ev_def + _blockA->ev_spd
                            + _blockA->ev_spdef;
                if((sum_of_rest + stat) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                                 % (510 - sum_of_rest)));

                _blockA->ev_spatk = stat;
                break;

            default: // Stats::SPECIAL_DEFENSE
                sum_of_rest = _blockA->ev_hp + _blockA->ev_atk + _blockA->ev_def + _blockA->ev_spd
                            + _blockA->ev_spatk;
                if((sum_of_rest + stat) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                                 % (510 - sum_of_rest)));

                _blockA->ev_spdef = stat;
                break;
        }

        _set_stats();
    }

    // NOTE: this affects stats
    void team_pokemon_ndsimpl::set_IV(const pkmn::pkstring &stat_name, unsigned int stat)
    {
        if(stat_name == "Special")
            throw std::runtime_error("The Special stat is only in Generations I-II.");
        if(stat > 31)
            throw std::runtime_error("IV's have a maximum value of 31 in Generations IV-V.");

        // Will throw if stat_name is invalid
        conversions::set_modern_IV(database::get_stat_id(stat_name), _blockB->iv_isegg_isnicknamed, stat);

        _set_stats();
    }

    pkmn::pkstring team_pokemon_ndsimpl::get_status() const
    {
        return conversions::retro_statuses.at(_raw.status, "OK");
    }

    item::sptr team_pokemon_ndsimpl::get_held_item() const
    {
        return item::make(database::get_item_id(_blockA->held_item, _game_id), _game_id);
    }

    void team_pokemon_ndsimpl::set_status(const pkmn::pkstring &status)
    {
        if(not conversions::reverse_retro_statuses.has_key(status))
            throw std::runtime_error("Invalid status given.");

        _raw.status = conversions::reverse_retro_statuses.at(status);
    }

    void team_pokemon_ndsimpl::set_held_item(const pkmn::pkstring &item_name)
    {
        _blockA->held_item = database::get_item_game_index(item_name,
                                                           database::get_version_name(_game_id));
    }

    move::sptr team_pokemon_ndsimpl::get_move(unsigned int pos) const
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        return move::make(_blockB->moves[pos-1], _game_id);
    }

    void team_pokemon_ndsimpl::get_moves(pkmn::moveset_t &moves) const
    {
        moves.clear();
        for(size_t i = 0; i < 4; i++) moves.push_back(get_move(i+1));
    }

    unsigned int team_pokemon_ndsimpl::get_move_PP(unsigned int pos) const
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        return _blockB->move_pps[pos-1];
    }

    void team_pokemon_ndsimpl::get_move_PPs(std::vector<unsigned int> &move_PPs) const
    {
        move_PPs.clear();
        for(size_t i = 0; i < 4; i++) move_PPs.push_back(_blockB->move_pps[i+1]); 
    }

    void team_pokemon_ndsimpl::set_move(const pkmn::pkstring &move_name, unsigned int pos)
    {   
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        // Make sure move exists in given generation
        std::ostringstream query_stream;
        query_stream << "SELECT generation_id FROM moves WHERE id="
                     << database::get_move_id(move_name);
        SQLite::Statement query(*_db, query_stream.str().c_str());
        if(query.executeStep()) _blockB->moves[pos-1] = int(query.getColumn(0));
        else throw std::runtime_error("This move does not exist in this generation.");
    }   

    void team_pokemon_ndsimpl::set_move_PP(unsigned int PP, unsigned int pos)
    {   
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        if(PP <= database::get_move_pp(_blockB->moves[pos-1])) _blockB->move_pps[pos-1] = PP; 
        else throw std::runtime_error("This move PP is invalid.");
    }

    pkmn::markings team_pokemon_ndsimpl::get_markings() const
    {
        return _blockA->markings;
    }

    void team_pokemon_ndsimpl::set_markings(const pkmn::markings &mark)
    {
        pkmn::markings m = mark;
        _blockA->markings = m;
    }

    pkmn::ribbons team_pokemon_ndsimpl::get_ribbons() const
    {
        pkmn::ribbons rib;

        rib.hoenn = _blockB->hoenn_ribbons;
        rib.sinnoh.ribbons1 = _blockA->sinnoh_ribbons1;
        if(get_generation() == 4) rib.sinnoh.ribbons2 = _blockA->sinnoh_ribbons2;
        else rib.unova = _blockA->unova_ribbons;
        rib.sinnoh.ribbons3 = _blockC->sinnoh_ribbons3;

        return rib;
    }

    void team_pokemon_ndsimpl::set_ribbons(const pkmn::ribbons &rib)
    {
        pkmn::ribbons r = rib;
        _blockB->hoenn_ribbons = r.hoenn;
        _blockA->sinnoh_ribbons1 = r.sinnoh.ribbons1;
        if(get_generation() == 4) _blockA->sinnoh_ribbons2 = r.sinnoh.ribbons2;
        else _blockA->unova_ribbons = r.unova;
        _blockC->sinnoh_ribbons3 = r.sinnoh.ribbons3;
    }

    unsigned int team_pokemon_ndsimpl::get_original_game_id() const
    {
        return hometown_to_libpkmn_game(_blockC->hometown);
    }

    unsigned int team_pokemon_ndsimpl::get_ability_id() const
    {
        return _blockA->ability;
    }

    unsigned int team_pokemon_ndsimpl::get_item_id() const
    {
        return database::get_item_id(_blockA->held_item, _game_id);
    }

    unsigned int team_pokemon_ndsimpl::get_nature_id() const
    {
        if(get_generation() == 4) return (_raw.pc.personality % 24);
        else return _blockB->nature;
    }

    void* team_pokemon_ndsimpl::get_native()
    {
        return &_raw;
    }

    void team_pokemon_ndsimpl::_set_experience(const uint32_t exp)
    {
        _blockA->exp = exp;
        _raw.level = database::get_level(_species_id, exp);
    }

    void team_pokemon_ndsimpl::_set_level(const uint8_t level)
    {
        _raw.level = level;
        _blockA->exp = database::get_experience(_species_id, level);
    }

    void team_pokemon_ndsimpl::_set_stats()
    {
        pkmn::dict<pkmn::pkstring, unsigned int> stats = _base->get_base_stats();
        pkmn::dict<pkmn::pkstring, unsigned int> IVs = get_IVs();
        pkmn::nature_t nature = get_nature();

        _raw.max_hp = int(floor(((double(IVs["HP"]) + (2.0*double(stats["HP"]))
                                  + (0.25*double(_blockA->ev_hp)) + 100.0)
                                * double(_raw.level))/100.0 + 10.0));
        _raw.current_hp = _raw.max_hp;

        _raw.atk    = int(ceil(((((double(IVs["Attack"]) + 2.0*double(stats["Attack"])
                                   + 0.25*double(_blockA->ev_atk))
                             * double(_raw.level))/100.0) + 5.0) * nature["Attack"]));

        _raw.def    = int(ceil(((((double(IVs["Defense"]) + 2.0*double(stats["Defense"])
                                   + 0.25*double(_blockA->ev_def))
                             * double(_raw.level))/100.0) + 5.0) * nature["Defense"]));

        _raw.spd    = int(ceil(((((double(IVs["Speed"]) + 2.0*double(stats["Speed"])
                                   + 0.25*double(_blockA->ev_spd))
                             * double(_raw.level))/100.0) + 5.0) * nature["Speed"]));

        _raw.spatk  = int(ceil(((((double(IVs["Special Attack"]) + 2.0*double(stats["Special Attack"])
                                   + 0.25*double(_blockA->ev_spatk))
                             * double(_raw.level))/100.0) + 5.0) * nature["Special Attack"]));

        _raw.spdef  = int(ceil(((((double(IVs["Special Defense"]) + 2.0*double(stats["Special Defense"])
                                   + 0.25*double(_blockA->ev_spdef))
                             * double(_raw.level))/100.0) + 5.0) * nature["Special Defense"]));
    }

    // Source: http://www.smogon.com/ingame/rng/pid_iv_creation#finding_gender_from_pid
    void team_pokemon_ndsimpl::_set_default_gender()
    {
        _blockB->form_encounterinfo &= ~(3<<1); // Clear gender flags

        float chance_male = _base->get_chance_male();
        float chance_female = _base->get_chance_female();

        if(chance_male + chance_female == 0.0) _blockB->form_encounterinfo |= (1<<2);
        else if(chance_male == 1.0) return;
        else if(chance_female == 1.0) _blockB->form_encounterinfo |= (1<<1);
        else
        {
            /*
             * Gender is determined by personality
             * If the personality is below a certain number, the Pokemon will be female
             */
            if((chance_female == 0.125 and _raw.pc.personality <= 30) or
               (chance_female == 0.25 and _raw.pc.personality <= 63) or
               (chance_female == 0.5 and _raw.pc.personality <= 126) or
               (_raw.pc.personality <= 190))
            {
                _blockB->form_encounterinfo |= (1<<1);
            }
        }
    }

}
