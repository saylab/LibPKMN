/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include <pkmn/calculations.hpp>
#include <pkmn/database.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/types/prng.hpp>

#include "pokemon_gen3impl.hpp"
#include "conversions/text.hpp"
#include "conversions/utils.hpp"

namespace pkmn
{
    pokemon_gen3impl::pokemon_gen3impl(uint16_t species, uint16_t version,
                                       uint8_t level,
                                       uint8_t move1, uint8_t move2,
                                       uint8_t move3, uint8_t move4):
        pokemon_impl(species, version)
    {
        /*
         * Populate native struct
         */
        _raw.pc.personality = _prng->lcrng();
        _raw.pc.ot_id = _prng->lcrng();

        pkmn::pkstring nickname = PKSTRING_UPPERCASE(_pokedex_entry.species_name);
        conversions::export_gen3_text(nickname, _raw.pc.nickname, 10);

        _raw.pc.language = 0x202; // English
        conversions::export_gen3_text("LIBPKMN", _raw.pc.otname, 7);
        // TODO: checksum

        _growth = &(_raw.pc.blocks.growth);
        _attacks = &(_raw.pc.blocks.attacks);
        _effort = &(_raw.pc.blocks.effort);
        _misc = &(_raw.pc.blocks.misc);

        _growth->species = database::get_pokemon_game_index(_species_id, _version_id);
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
        // TODO: Pokerus
        _misc->met_location = 255; // Fateful encounter

        // Origin info
        _misc->origin_info = (level & 0x7F);
        _misc->origin_info |= (uint8_t(database::get_version_game_index(_version_id)) << 6);
        _misc->origin_info |= (uint8_t(Balls::LUXURY_BALL) << 10);

        _misc->iv_egg_ability = _prng->lcrng();
        _misc->iv_egg_ability &= ~(1<<30); // Not an egg

        _misc->ribbons_obedience = 0;
        _misc->ribbons_obedience |= (1<<31); // Mew and Deoxys will be obedient

        _raw.condition = 0; // OK
        _set_level(level);
        _set_stats(); // Will populate party portion
        _set_form();
    }

    pokemon_gen3impl::pokemon_gen3impl(const pkmn::gen3_pc_pokemon_t& raw,
                                       uint8_t version):
        pokemon_impl(database::get_pokemon_id(raw.blocks.growth.species, version),
                     version)
    {
        _raw.pc = raw;
        _growth = &(_raw.pc.blocks.growth);
        _attacks = &(_raw.pc.blocks.attacks);
        _effort = &(_raw.pc.blocks.effort);
        _misc = &(_raw.pc.blocks.misc);

        _set_stats(); // Will populate party portion
    }

    pokemon_gen3impl::pokemon_gen3impl(const pkmn::gen3_party_pokemon_t& raw,
                                       uint8_t version):
        pokemon_impl(database::get_pokemon_id(raw.pc.blocks.growth.species, version),
                     version),
        _raw(raw)
    {
        _growth = &(_raw.pc.blocks.growth);
        _attacks = &(_raw.pc.blocks.attacks);
        _effort = &(_raw.pc.blocks.effort);
        _misc = &(_raw.pc.blocks.misc);
    };

    pokemon_gen3impl::pokemon_gen3impl(const pokemon_gen3impl& other):
        pokemon_impl(other),
        _raw(other._raw)
    {
        _growth = &(_raw.pc.blocks.growth);
        _attacks = &(_raw.pc.blocks.attacks);
        _effort = &(_raw.pc.blocks.effort);
        _misc = &(_raw.pc.blocks.misc);
    }

    pokemon_gen3impl& pokemon_gen3impl::operator=(const pokemon_gen3impl& other)
    {
        pokemon_impl::operator=(other);

        _growth = &(_raw.pc.blocks.growth);
        _attacks = &(_raw.pc.blocks.attacks);
        _effort = &(_raw.pc.blocks.effort);
        _misc = &(_raw.pc.blocks.misc);
    }

    /*
     * Getting Non-Battle Info
     */

    pkmn::contest_stats_t pokemon_gen3impl::get_contest_stats() const
    {
        return _effort->contest_stats;
    }

    pkmn::markings_t pokemon_gen3impl::get_markings() const
    {
        return _raw.pc.markings;
    }

    pkmn::ribbons_t pokemon_gen3impl::get_ribbons() const
    {
        pkmn::ribbons_t ribbons;
        ribbons.hoenn = _misc->ribbons_obedience;

        return ribbons;
    }

    /*
     * Setting Non-Battle Info
     */

    void pokemon_gen3impl::set_contest_stats(const pkmn::contest_stats_t& contest_stats)
    {
        _effort->contest_stats = contest_stats;
    }

    void pokemon_gen3impl::set_markings(const pkmn::markings_t& markings)
    {
        pkmn::markings_t _markings = markings;
        _raw.pc.markings = _markings;
    }

    void pokemon_gen3impl::set_ribbons(const pkmn::ribbons_t& ribbons)
    {
        pkmn::ribbons_t _ribbons = ribbons;
        _misc->ribbons_obedience = _ribbons.hoenn;
    }

    /*
     * Getting Trainer Info
     */

    pkmn::pkstring pokemon_gen3impl::get_nickname() const
    {
        return conversions::import_gen3_text(_raw.pc.nickname, 10);
    }

    pkmn::pkstring pokemon_gen3impl::get_trainer_name() const
    {
        return conversions::import_gen3_text(_raw.pc.otname, 7);
    }

    pkmn::pkstring pokemon_gen3impl::get_trainer_gender() const
    {
        return (_misc->origin_info & (1<<31)) ? "Female" : "Male";
    }

    uint32_t pokemon_gen3impl::get_trainer_id() const
    {
        return _raw.pc.ot_id;
    }

    uint16_t pokemon_gen3impl::get_trainer_public_id() const
    {
        return _raw.pc.ot_pid;
    }

    uint16_t pokemon_gen3impl::get_trainer_secret_id() const
    {
        return _raw.pc.ot_sid;
    }

    pkmn::pkstring pokemon_gen3impl::get_ball() const
    {
        return database::get_ball_name((_misc->origin_info & 0x7800) >> 11);
    }

    pkmn::pkstring pokemon_gen3impl::get_original_game() const
    {
        return database::get_version_name(get_original_game_id());
    }

    uint8_t pokemon_gen3impl::get_met_level() const
    {
        return (_misc->origin_info & 0x7F);
    }

    /*
     * Setting Trainer Info
     */

    void pokemon_gen3impl::set_nickname(const pkmn::pkstring& nickname)
    {
        if(nickname.length() > 10)
            throw std::runtime_error("Nicknames can have a maximum of 10 characters in Generation III.");

        conversions::export_gen3_text(nickname, _raw.pc.nickname, 10);
    }

    void pokemon_gen3impl::set_trainer_name(const pkmn::pkstring& trainer_name)
    {
        if(trainer_name.length() > 7)
            throw std::runtime_error("Trainer names can have a maximum of 7 characters in Generation III.");

        conversions::export_gen3_text(trainer_name, _raw.pc.otname, 7);
    }

    void pokemon_gen3impl::set_trainer_gender(const pkmn::pkstring& gender)
    {
        if(gender.std_string() != "Male" and gender.std_string() != "Female")
            throw std::runtime_error("Gender must be male or female.");

        if(gender.std_string() == "Male") _misc->origin_info &= ~(1<<31);
        else                              _misc->origin_info |= (1<<31);
    }

    void pokemon_gen3impl::set_trainer_id(uint32_t id)
    {
        _raw.pc.ot_id = id;
    }

    void pokemon_gen3impl::set_trainer_public_id(uint16_t id)
    {
        _raw.pc.ot_pid = id;
    }

    void pokemon_gen3impl::set_trainer_secret_id(uint16_t id)
    {
        _raw.pc.ot_sid = id;
    }

    void pokemon_gen3impl::set_ball(const pkmn::pkstring& ball)
    {
        uint8_t ball_id = database::get_ball_id(ball);
        _misc->origin_info = 0x87FF | (ball_id << 10);
    }

    void pokemon_gen3impl::set_original_game(const pkmn::pkstring& game)
    {
        if(database::get_generation(game) != 3)
            throw std::runtime_error("Original game must be from Generation III.");

        _misc->origin_info = 0xFC3F | (uint8_t(database::get_version_game_index(game)) << 6);
    }

    // Met level of 0 means Pokemon was hatched
    void pokemon_gen3impl::set_met_level(uint8_t level)
    {
        if(level > 100)
            throw std::runtime_error("Level must be 0-100.");

        _misc->origin_info = 0xFFC0 | (uint8_t(level) & 0x3F);
    }

    /*
     * Getting Individual Stat Info
     */

    uint32_t pokemon_gen3impl::get_personality() const
    {
        return _raw.pc.personality;
    }

    uint8_t pokemon_gen3impl::get_friendship() const
    {
        return _growth->friendship;
    }

    uint8_t pokemon_gen3impl::get_level() const
    {
        return _raw.level;
    }

    uint32_t pokemon_gen3impl::get_experience() const
    {
        return _growth->exp;
    }

    /*
     * Source: http://www.smogon.com/ingame/rng/pid_iv_creation#finding_gender_from_pid
     */
    pkmn::pkstring pokemon_gen3impl::get_gender() const
    {
        float chance_male   = _pokedex_entry.chance_male;
        float chance_female = _pokedex_entry.chance_female;

        if(chance_male + chance_female == 0.0) return "Genderless";
        else if(chance_male == 1.0) return "Male";
        else if(chance_female == 1.0) return "Female";
        else
        {
            /*
             * Gender is determined by (personality & 0xFF).
             */
            uint8_t truncated_pid = (_raw.pc.personality & 0xFF);
            if(chance_male == 0.875)     return (truncated_pid > 30)  ? "Male" : "Female";
            else if(chance_male == 0.75) return (truncated_pid > 63)  ? "Male" : "Female";
            else if(chance_male == 0.5)  return (truncated_pid > 126) ? "Male" : "Female";
            else                         return (truncated_pid > 190) ? "Male" : "Female";
        }
    }

    pkmn::nature_t pokemon_gen3impl::get_nature() const
    {
        return pkmn::nature_t(_raw.pc.personality % 24);
    }

    pkmn::pkstring pokemon_gen3impl::get_ability() const
    {
        int ability_slot = get_gen3_ability_slot(_misc->iv_egg_ability) + 1;

        std::ostringstream query_stream;
        query_stream << "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id=" << _species_id
                     << " AND is_hidden=0 AND slot=" << ability_slot;
        SQLite::Statement query(*_db, query_stream.str().c_str());

        uint16_t ability_id;
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

    bool pokemon_gen3impl::is_shiny() const
    {
        return calculations::get_modern_shiny(_raw.pc.personality,
                                              _raw.pc.ot_sid,
                                              _raw.pc.ot_pid);
    }

    pkmn::dict<pkmn::pkstring, uint16_t> pokemon_gen3impl::get_stats() const
    {
        pkmn::dict<pkmn::pkstring, uint16_t> stats;
        stats["HP"]              = _raw.max_hp;
        stats["Attack"]          = _raw.atk;
        stats["Defense"]         = _raw.def;
        stats["Speed"]           = _raw.spd;
        stats["Special Attack"]  = _raw.spatk;
        stats["Special Defense"] = _raw.spdef;

        return stats;
    }

    pkmn::dict<pkmn::pkstring, uint16_t> pokemon_gen3impl::get_EVs() const
    {
        pkmn::dict<pkmn::pkstring, uint16_t> EVs;
        EVs["HP"]              = _effort->ev_hp;
        EVs["Attack"]          = _effort->ev_atk;
        EVs["Defense"]         = _effort->ev_def;
        EVs["Speed"]           = _effort->ev_spd;
        EVs["Special Attack"]  = _effort->ev_spatk;
        EVs["Special Defense"] = _effort->ev_spdef;

        return EVs;
    }

    pkmn::dict<pkmn::pkstring, uint16_t> pokemon_gen3impl::get_IVs() const
    {
        pkmn::dict<pkmn::pkstring, uint16_t> IVs;
        IVs["HP"]              = conversions::get_modern_IV(Stats::HP,      _misc->iv_egg_ability);
        IVs["Attack"]          = conversions::get_modern_IV(Stats::ATTACK,  _misc->iv_egg_ability);
        IVs["Defense"]         = conversions::get_modern_IV(Stats::DEFENSE, _misc->iv_egg_ability);
        IVs["Speed"]           = conversions::get_modern_IV(Stats::SPEED,   _misc->iv_egg_ability);
        IVs["Special Attack"]  = conversions::get_modern_IV(Stats::SPECIAL, _misc->iv_egg_ability);
        IVs["Special Defense"] = conversions::get_modern_IV(Stats::SPECIAL, _misc->iv_egg_ability);

        return IVs;
    }

    /*
     * Getting Individual Stat Infos
     */

    // NOTE: this affects many things
    void pokemon_gen3impl::set_personality(uint32_t personality)
    {
        _raw.pc.personality = personality;
        if(_species_id == Species::UNOWN)
            _set_form();
    }

    void pokemon_gen3impl::set_friendship(uint8_t friendship)
    {
        _growth->friendship = friendship;
    }

    // NOTE: this changes experience and stats
    void pokemon_gen3impl::set_level(uint8_t level)
    {
        if(level < 1 or level > 100)
            throw std::runtime_error("Level must be 1-100.");

        _set_level(level);
        _set_stats();
    }

    // NOTE: this affects level and stats
    void pokemon_gen3impl::set_experience(uint32_t experience)
    {
        _set_experience(experience);
        _set_stats();
    }

    /*
     * Source: http://www.smogon.com/ingame/rng/pid_iv_creation#finding_gender_from_pid
     *
     * NOTE: this affects many things
     */
    void pokemon_gen3impl::set_gender(const pkmn::pkstring& gender)
    {
        float chance_male = _pokedex_entry.chance_male;
        float chance_female = _pokedex_entry.chance_female;

        // Sanity check
        if(((chance_male + chance_female) == 0) or
            (chance_male == 1.0) or (chance_female == 1.0))
            throw std::runtime_error("This species's gender cannot be changed.");
        else if(gender.std_string() != "Male" and gender.std_string() != "Female")
            throw std::runtime_error("Gender can only be \"Male\" or \"Female\".");
        else
        {
            /*
             * Gender is determined by (personality & 0xFF).
             */
            if(gender.std_string() == "Male") _raw.pc.personality |= 0xFF;
            else                              _raw.pc.personality &= 0x00;
        }
    }

    // NOTE: this affects many things
    void pokemon_gen3impl::set_nature(const pkmn::pkstring& nature_name)
    {
        // Set personality to nearest value that results in given nature
        int8_t current_nature = (_raw.pc.personality % 24);
        int8_t desired_nature = database::get_nature_id(nature_name);
        _raw.pc.personality += (current_nature - desired_nature);
    }

    // NOTE: this affects many things
    void pokemon_gen3impl::set_ability(const pkmn::pkstring& ability)
    {
        // Check to see if Pokemon has this ability
        std::ostringstream query_stream;
        query_stream << "SELECT slot FROM pokemon_abilities WHERE ability_id="
                     << database::get_ability_id(ability) << " AND is_hidden=0"
                     << " AND pokemon_id=" << _species_id;
        SQLite::Statement query(*_db, query_stream.str().c_str());
        if(query.executeStep())
        {
            uint8_t ability_slot = uint8_t(query.getColumn(0)) - 1;
            uint8_t current_ability_slot = get_gen3_ability_slot(_misc->iv_egg_ability) + 1;

            if(ability_slot != current_ability_slot) _raw.pc.personality++;
        }
        else throw std::runtime_error("Invalid ability for this Pokemon.");
    }

    // NOTE: this affects stats
    void pokemon_gen3impl::set_EV(const pkmn::pkstring& stat, uint16_t value)
    {
        if(stat == "Special")
            throw std::runtime_error("The Special value is only in Generations I-II.");
        if(value > 255)
            throw std::runtime_error("EV's have a maximum value of 255 in Generation IV-V.");

        uint16_t sum_of_rest;

        switch(database::get_stat_id(stat)) // Will throw if stat is invalid
        {
            case Stats::HP:
                sum_of_rest = _effort->ev_atk + _effort->ev_def + _effort->ev_spd + _effort->ev_spatk
                            + _effort->ev_spdef;
                if((sum_of_rest + value) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                             % (510 - sum_of_rest)));

                _effort->ev_hp = value;
                break;

            case Stats::ATTACK:
                sum_of_rest = _effort->ev_hp + _effort->ev_def + _effort->ev_spd + _effort->ev_spatk
                            + _effort->ev_spdef;
                if((sum_of_rest + value) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                                 % (510 - sum_of_rest)));

                _effort->ev_atk = value;
                break;

            case Stats::DEFENSE:
                sum_of_rest = _effort->ev_hp + _effort->ev_atk + _effort->ev_spd + _effort->ev_spatk
                            + _effort->ev_spdef;
                if((sum_of_rest + value) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                                 % (510 - sum_of_rest)));

                _effort->ev_def = value;
                break;

            case Stats::SPEED:
                sum_of_rest = _effort->ev_hp + _effort->ev_atk + _effort->ev_def + _effort->ev_spatk
                            + _effort->ev_spdef;
                if((sum_of_rest + value) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                                 % (510 - sum_of_rest)));

                _effort->ev_spd = value;
                break;

            case Stats::SPECIAL_ATTACK:
                sum_of_rest = _effort->ev_hp + _effort->ev_atk + _effort->ev_def + _effort->ev_spd
                            + _effort->ev_spdef;
                if((sum_of_rest + value) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                                 % (510 - sum_of_rest)));

                _effort->ev_spatk = value;
                break;

            default: // Stats::SPECIAL_DEFENSE
                sum_of_rest = _effort->ev_hp + _effort->ev_atk + _effort->ev_def + _effort->ev_spd
                            + _effort->ev_spatk;
                if((sum_of_rest + value) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                                 % (510 - sum_of_rest)));

                _effort->ev_spdef = value;
                break;
        }

        _set_stats();
    }

    // NOTE: this affects stats
    void pokemon_gen3impl::set_IV(const pkmn::pkstring& stat, uint16_t value)
    {
        if(stat == "Special")
            throw std::runtime_error("The Special stat is only in Generations I-II.");
        if(value > 31)
            throw std::runtime_error("IV's have a maximum value of 31 in Generation III.");

        // Will throw if stat_name is invalid
        conversions::set_modern_IV(database::get_stat_id(stat), _misc->iv_egg_ability, value);

        _set_stats();
    }

    /*
     * Battle Stat Info
     */

    pkmn::pkstring pokemon_gen3impl::get_status() const
    {
        return conversions::modern_statuses.at(_raw.condition, "OK");
    }

    pkmn::item_entry_t pokemon_gen3impl::get_held_item() const
    {
        return _pokedex->get_item_entry(database::get_item_id(_growth->held_item, _version_id));
    }

    void pokemon_gen3impl::set_status(const pkmn::pkstring& status)
    {
        if(not conversions::reverse_modern_statuses.has_key(status))
            throw std::runtime_error("Invalid status given.");

        _raw.condition = conversions::reverse_modern_statuses.at(status);
    }

    void pokemon_gen3impl::set_held_item(const pkmn::pkstring& item_name)
    {
        _growth->held_item = database::get_item_game_index(item_name,
                                                           database::get_version_name(_version_id));
    }

    /*
     * Getting Move Info
     */

    pkmn::move_entry_t pokemon_gen3impl::get_move(uint8_t pos) const
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        return _pokedex->get_move_entry(_attacks->moves[pos-1]);
    }

    void pokemon_gen3impl::get_moves(pkmn::moveset_t& moves) const
    {
        moves.clear();
        for(size_t i = 0; i < 4; i++) moves.push_back(get_move(i+1));
    }

    uint8_t pokemon_gen3impl::get_move_PP(uint8_t pos) const
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        return _attacks->move_pps[pos-1];
    }

    void pokemon_gen3impl::get_move_PPs(std::vector<uint8_t>& move_PPs) const
    {
        move_PPs.clear();
        for(size_t i = 0; i < 4; i++) move_PPs.push_back(_attacks->move_pps[i+1]); 
    }

    /*
     * Setting Move Info
     */

    void pokemon_gen3impl::set_move(const pkmn::pkstring& move_name, uint8_t pos)
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

    void pokemon_gen3impl::set_move_PP(uint8_t PP, uint8_t pos)
    {   
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        if(PP <= database::get_move_pp(_attacks->moves[pos-1])) _attacks->move_pps[pos-1] = PP; 
        else throw std::runtime_error("This move PP is invalid.");
    }

    /*
     * Database Info
     */

    uint16_t pokemon_gen3impl::get_original_game_id() const
    {
        return database::get_version_id((_misc->origin_info &= ~0xFC3F) >> 6);
    }

    uint16_t pokemon_gen3impl::get_ability_id() const
    {
        return database::get_ability_id(get_ability());
    }

    uint16_t pokemon_gen3impl::get_item_id() const
    {
        return database::get_item_id(_growth->held_item, _version_id);
    }

    uint16_t pokemon_gen3impl::get_nature_id() const
    {
        return (_raw.pc.personality % 24);
    }

    const void* pokemon_gen3impl::get_native()
    {
        return &_raw;
    }

    void pokemon_gen3impl::_set_experience(uint32_t exp)
    {
        _growth->exp = exp;
        _raw.level = database::get_level(_species_id, exp);
    }

    void pokemon_gen3impl::_set_level(uint8_t level)
    {
        _raw.level = level;
        _growth->exp = database::get_experience(_species_id, level);
    }

    void pokemon_gen3impl::_set_stats()
    {
        pkmn::dict<pkmn::pkstring, uint16_t> stats = _pokedex_entry.base_stats;
        pkmn::dict<pkmn::pkstring, uint16_t> IVs = get_IVs();
        pkmn::nature_t nature = get_nature();

        _raw.max_hp = calculations::get_modern_stat(Stats::HP, stats["HP"], _raw.level,
                                                    nature, _effort->ev_hp, IVs["HP"]);
        _raw.current_hp = _raw.max_hp;
        _raw.atk   = calculations::get_modern_stat(Stats::ATTACK, stats["Attack"], _raw.level,
                                                   nature, _effort->ev_atk, IVs["Attack"]);
        _raw.def   = calculations::get_modern_stat(Stats::DEFENSE, stats["Defense"], _raw.level,
                                                   nature, _effort->ev_def, IVs["Defense"]);
        _raw.spd   = calculations::get_modern_stat(Stats::SPEED, stats["Speed"], _raw.level,
                                                   nature, _effort->ev_spd, IVs["Speed"]);
        _raw.spatk = calculations::get_modern_stat(Stats::SPECIAL_ATTACK, stats["Special Attack"],
                                                   _raw.level,nature, _effort->ev_spatk,
                                                   IVs["Special Attack"]);
        _raw.spdef = calculations::get_modern_stat(Stats::SPECIAL_DEFENSE, stats["Special Defense"],
                                                   _raw.level,nature, _effort->ev_spdef,
                                                   IVs["Special Defense"]);
    }

    void pokemon_gen3impl::_set_form()
    {
        if(_species_id == Species::UNOWN)
            _form_id = calculations::get_gen3_unown_form(_raw.pc.personality);
        else if(_species_id == Species::DEOXYS)
        {
            switch(_version_id)
            {
                case Versions::FIRERED:
                    _form_id = Forms::Deoxys::ATTACK;
                    break;

                case Versions::LEAFGREEN:
                    _form_id = Forms::Deoxys::DEFENSE;
                    break;

                default: // Ruby/Sapphire
                    _form_id = Forms::Deoxys::NORMAL;
                    break;
            }
        }

        _pokedex_entry = _pokedex->get_pokemon_entry(_species_id, _form_id);
    }
}
