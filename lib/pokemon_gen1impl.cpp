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

#include "pokemon_gen1impl.hpp"
#include "conversions/utils.hpp"

namespace pkmn
{
    pokemon_gen1impl::pokemon_gen1impl(uint16_t species, uint16_t version,
                                       uint8_t level,
                                       uint8_t move1, uint8_t move2,
                                       uint8_t move3, uint8_t move4):
        pokemon_impl(species, version),
        _nickname(PKSTRING_UPPERCASE(database::get_species_name(species))),
        _otname("LIBPKMN")
    {
        /*
         * Populate native struct
         */
        _raw.pc.species = database::get_pokemon_game_index(_species_id, _version_id);
        // current_hp populated by _set_stats()
        _set_level(level);
        _raw.pc.status = 0x00;
        _raw.pc.types[0] = database::get_type_id(_pokedex_entry.types.first);
        _raw.pc.types[1] = database::get_type_id(_pokedex_entry.types.second);
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

    pokemon_gen1impl::pokemon_gen1impl(const pkmn::gen1_pc_pokemon_t& raw,
                                       uint8_t version):
        pokemon_impl(database::get_pokemon_id(raw.species, Versions::RED),
                     version),
        _nickname(UPPERCASE_SPECIES_NAME(raw.species, Versions::RED)),
        _otname("LIBPKMN")
    {
        _raw.pc = raw;

        _set_stats(); // Will populate party portion of struct
    }

    pokemon_gen1impl::pokemon_gen1impl(const pkmn::gen1_pc_pokemon_t& raw,
                                       const pkmn::pkstring& nickname,
                                       const pkmn::pkstring& otname,
                                       uint8_t version):
        pokemon_impl(database::get_pokemon_id(raw.species, Versions::RED),
                     version),
        _nickname(nickname),
        _otname(otname)
    {
        _raw.pc = raw;
        _set_stats(); // Will populate party portion of struct
    }

    pokemon_gen1impl::pokemon_gen1impl(const pkmn::gen1_party_pokemon_t& raw,
                                       uint8_t version):
        pokemon_impl(database::get_pokemon_id(raw.pc.species, Versions::RED),
                     version),
        _raw(raw),
        _nickname(UPPERCASE_SPECIES_NAME(raw.pc.species, Versions::RED)),
        _otname("LIBPKMN") {};

    pokemon_gen1impl::pokemon_gen1impl(const pkmn::gen1_party_pokemon_t& raw,
                                       const pkmn::pkstring& nickname,
                                       const pkmn::pkstring& otname,
                                       uint8_t version):
        pokemon_impl(database::get_pokemon_id(raw.pc.species, Versions::RED),
                     version),
        _raw(raw),
        _nickname(nickname),
        _otname(otname) {};

    pokemon_gen1impl::pokemon_gen1impl(const pokemon_gen1impl& other):
        pokemon_impl(other),
        _raw(other._raw),
        _nickname(other._nickname),
        _otname(other._otname) {};

    pokemon_gen1impl& pokemon_gen1impl::operator=(const pokemon_gen1impl& other)
    {
        pokemon_impl::operator=(other);

        _raw      = other._raw;
        _nickname = other._nickname;
        _otname   = other._otname;
    }

    /*
     * Getting Non-battle info
     */

    // No contests in Generation I
    pkmn::contest_stats_t pokemon_gen1impl::get_contest_stats() const
    {
        return pkmn::contest_stats_t();
    }

    // No markings in Generation I
    pkmn::markings_t pokemon_gen1impl::get_markings() const
    {
        return pkmn::markings_t();
    }

    // No ribbons in Generation I
    pkmn::ribbons_t pokemon_gen1impl::get_ribbons() const
    {
        return pkmn::ribbons_t();
    }

    /*
     * Setting Non-battle info
     */

    // No contests in Generation I
    void pokemon_gen1impl::set_contest_stats(const pkmn::contest_stats_t& contest_stats)
    {
        /* NOP */
    }

    // No markings in Generation I
    void pokemon_gen1impl::set_markings(const pkmn::markings_t& markings)
    {
        /* NOP */
    }

    // No ribbons in Generation I
    void pokemon_gen1impl::set_ribbons(const pkmn::ribbons_t& ribbons)
    {
        /* NOP */
    }

    /*
     * Getting Trainer Info
     */

    pkmn::pkstring pokemon_gen1impl::get_nickname() const
    {
        return _nickname;
    }

    pkmn::pkstring pokemon_gen1impl::get_trainer_name() const
    {
        return _otname;
    }

    // All trainers are male in Generation I
    pkmn::pkstring pokemon_gen1impl::get_trainer_gender() const
    {
        return "Male";
    }

    // Trainer ID is only 16 bits, all publicly exposed, in Generation I
    uint32_t pokemon_gen1impl::get_trainer_id() const
    {
        return _raw.pc.ot_id;
    }

    // Trainer ID is only 16 bits, all publicly exposed, in Generation I
    uint16_t pokemon_gen1impl::get_trainer_public_id() const
    {
        return _raw.pc.ot_id;
    }

    // Trainer ID is only 16 bits, all publicly exposed, in Generation I
    uint16_t pokemon_gen1impl::get_trainer_secret_id() const
    {
        return 0;
    }

    // Ball not recorded in Generation I
    pkmn::pkstring pokemon_gen1impl::get_ball() const
    {
        return database::get_ball_name(Balls::POKE_BALL);
    }

    // Original game not recorded in Generation I
    pkmn::pkstring pokemon_gen1impl::get_original_game() const
    {
        return get_game();
    }

    // Met level not recorded in Generation I
    uint8_t pokemon_gen1impl::get_met_level() const
    {
        return _raw.level;
    }

    /*
     * Setting Trainer Info
     */

    void pokemon_gen1impl::set_nickname(const pkmn::pkstring& nickname)
    {
        if(nickname.length() > 10)
            throw std::runtime_error("Nicknames have a maximum of 10 characters.");

        _nickname = nickname;
    }

    void pokemon_gen1impl::set_trainer_name(const pkmn::pkstring& trainer_name)
    {
        if(trainer_name.length() > 7)
            throw std::runtime_error("Trainer names have a maximum of 7 characters.");

        _otname = trainer_name;
    }

    // All trainers are male in Generation I
    void pokemon_gen1impl::set_trainer_gender(const pkmn::pkstring& gender)
    {
        /* NOP */
    }

    // Trainer ID is only 16 bits, all publicly exposed, in Generation I
    void pokemon_gen1impl::set_trainer_id(uint32_t id)
    {
        _raw.pc.ot_id = uint16_t(id);
    }

    // Trainer ID is only 16 bits, all publicly exposed, in Generation I
    void pokemon_gen1impl::set_trainer_public_id(uint16_t id)
    {
        _raw.pc.ot_id = id;
    }

    // Trainer ID is only 16 bits, all publicly exposed, in Generation I
    void pokemon_gen1impl::set_trainer_secret_id(uint16_t id)
    {
        /* NOP */
    }

    // Ball not recorded in Generation I
    void pokemon_gen1impl::set_ball(const pkmn::pkstring& ball)
    {
        /* NOP */
    }

    // Original game not recorded in Generation I
    void pokemon_gen1impl::set_original_game(const pkmn::pkstring& game)
    {
        /* NOP */
    }

    // Met level not recorded in Generation I
    void pokemon_gen1impl::set_met_level(uint8_t level)
    {
        /* NOP */
    }

    /*
     * Getting Individual Stat Info
     */

    // No personality in Generation I
    uint32_t pokemon_gen1impl::get_personality() const
    {
        return 0;
    }

    // No friendship in Generation I
    uint8_t pokemon_gen1impl::get_friendship() const
    {
        return 0;
    }

    uint8_t pokemon_gen1impl::get_level() const
    {
        return _raw.level;
    }

    // Stored in Base-256 in three bytes
    uint32_t pokemon_gen1impl::get_experience() const
    {
        return (65536*_raw.pc.exp[0]) + (256*_raw.pc.exp[1]) * _raw.pc.exp[2];
    }

    // No gender in Generation I
    pkmn::pkstring pokemon_gen1impl::get_gender() const
    {
        return "Male";
    }

    // No natures in Generation I
    pkmn::nature_t pokemon_gen1impl::get_nature() const
    {
        return pkmn::nature_t();
    }

    // No abilities in Generation I
    pkmn::pkstring pokemon_gen1impl::get_ability() const
    {
        return "None";
    }

    // No shininess in Generation I
    bool pokemon_gen1impl::is_shiny() const
    {
        return false;
    }

    pkmn::dict<pkmn::pkstring, uint16_t> pokemon_gen1impl::get_stats() const
    {
        pkmn::dict<pkmn::pkstring, uint16_t> stats;
        stats["HP"]      = _raw.max_hp;
        stats["Attack"]  = _raw.atk;
        stats["Defense"] = _raw.def;
        stats["Speed"]   = _raw.spd;
        stats["Special"] = _raw.spcl;

        return stats;
    }

    pkmn::dict<pkmn::pkstring, uint16_t> pokemon_gen1impl::get_EVs() const
    {
        pkmn::dict<pkmn::pkstring, uint16_t> EVs;
        EVs["HP"]      = _raw.pc.ev_hp;
        EVs["Attack"]  = _raw.pc.ev_atk;
        EVs["Defense"] = _raw.pc.ev_def;
        EVs["Speed"]   = _raw.pc.ev_spd;
        EVs["Special"] = _raw.pc.ev_spcl;

        return EVs;
    }

    pkmn::dict<pkmn::pkstring, uint16_t> pokemon_gen1impl::get_IVs() const
    {
        pkmn::dict<pkmn::pkstring, uint16_t> IVs;
        IVs["HP"]      = conversions::get_retro_IV(Stats::HP,      _raw.pc.iv_data);
        IVs["Attack"]  = conversions::get_retro_IV(Stats::ATTACK,  _raw.pc.iv_data);
        IVs["Defense"] = conversions::get_retro_IV(Stats::DEFENSE, _raw.pc.iv_data);
        IVs["Speed"]   = conversions::get_retro_IV(Stats::SPEED,   _raw.pc.iv_data);
        IVs["Special"] = conversions::get_retro_IV(Stats::SPECIAL, _raw.pc.iv_data);

        return IVs;
    }

    /*
     * Setting Individual Stat Info
     */

    // No personality in Generation I
    void pokemon_gen1impl::set_personality(uint32_t personality)
    {
        /* NOP */
    }

    // No friendship in Generation I
    void pokemon_gen1impl::set_friendship(uint8_t friendship)
    {
        /* NOP */
    }

    // NOTE: this changes experience and stats
    void pokemon_gen1impl::set_level(uint8_t level)
    {
        /*
         * Pokémon in Generation I-II cannot be level 1.
         *
         * http://bulbapedia.bulbagarden.net/wiki/Experience#Experience_underflow_glitch
         */
        if(level < 2 or level > 100)
            throw std::runtime_error("Level must be 2-100.");

        _set_level(level);
        _set_stats();
    }

    // NOTE: this affects level and stats
    void pokemon_gen1impl::set_experience(uint32_t experience)
    {
        _set_experience(experience);
        _set_stats();
    }

    // No gender in Generation I
    void pokemon_gen1impl::set_gender(const pkmn::pkstring& gender)
    {
        /* NOP */
    }

    // No natures in Generation I
    void pokemon_gen1impl::set_nature(const pkmn::pkstring& nature_name)
    {
        /* NOP */
    }

    // No abilities in Generation I
    void pokemon_gen1impl::set_ability(const pkmn::pkstring& ability)
    {
        /* NOP */
    }

    // No alternate forms in Generation I
    void pokemon_gen1impl::set_form(const pkmn::pkstring& form)
    {
        /* NOP */
    }

    // No shininess in Generation I
    void pokemon_gen1impl::set_shiny(bool value)
    {
        /* NOP */
    }

    // NOTE: this affects stats
    void pokemon_gen1impl::set_EV(const pkmn::pkstring& stat, uint16_t value)
    {
        if(stat == "Special Attack" or stat == "Special Defense")
            throw std::runtime_error("This stat is not present in Generation I.");

        if(value > 65535)
            throw std::runtime_error("EV's have a maximum value of 65535 in Generation I.");

        switch(database::get_stat_id(stat)) // Will throw if stat is invalid
        {
            case Stats::HP:
                _raw.pc.ev_hp = value;
                break;

            case Stats::ATTACK:
                _raw.pc.ev_atk = value;
                break;

            case Stats::DEFENSE:
                _raw.pc.ev_def = value;
                break;

            case Stats::SPEED:
                _raw.pc.ev_spd = value;
                break;

            default: // Stats::SPECIAL
                _raw.pc.ev_spcl = value;
                break;
        }

        _set_stats();
    }

    // NOTE: this affects stats and other IV's
    void pokemon_gen1impl::set_IV(const pkmn::pkstring& stat, uint16_t value)
    {
        if(stat == "Special Attack" or stat == "Special Defense")
            throw std::runtime_error("This stat is not present in Generation I.");

        if(value > 15)
            throw std::runtime_error("IV's have a maximum value of 15 in Generation I.");

        // Will throw if stat is invalid
        conversions::set_retro_IV(database::get_stat_id(stat), _raw.pc.iv_data, value);

        _set_stats();
    }

    /*
     * Battle Stat Info
     */

    pkmn::pkstring pokemon_gen1impl::get_status() const
    {
        return conversions::retro_statuses.at(_raw.pc.status, "OK");
    }

    // No held items in Generation I
    pkmn::item_entry_t pokemon_gen1impl::get_held_item() const
    {
        return _pokedex->get_item_entry(Items::NONE);
    }

    void pokemon_gen1impl::set_status(const pkmn::pkstring& status)
    {
        if(not conversions::reverse_retro_statuses.has_key(status))
            throw std::runtime_error("Invalid status given.");

        _raw.pc.status = conversions::reverse_retro_statuses.at(status);
    }

    // No held items in Generation I
    void pokemon_gen1impl::set_held_item(const pkmn::pkstring& item_name)
    {
        /* NOP */
    }

    /*
     * Getting Move Info
     */

    pkmn::move_entry_t pokemon_gen1impl::get_move(uint8_t pos) const
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        return _pokedex->get_move_entry(_raw.pc.moves[pos-1]);
    }

    void pokemon_gen1impl::get_moves(pkmn::moveset_t& moves) const
    {
        moves.clear();
        for(size_t i = 0; i < 4; i++)
            moves.push_back(get_move(i+1));
    }

    uint8_t pokemon_gen1impl::get_move_PP(uint8_t pos) const
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        return _raw.pc.move_pps[pos-1];
    }

    void pokemon_gen1impl::get_move_PPs(std::vector<uint8_t>& move_PPs) const
    {
        move_PPs.clear();
        for(size_t i = 0; i < 4; i++)
            move_PPs.push_back(_raw.pc.move_pps[i+1]); 
    }

    void pokemon_gen1impl::set_move(const pkmn::pkstring& move_name, uint8_t pos)
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        // Make sure move exists in given generation
        std::ostringstream query_stream;
        query_stream << "SELECT generation_id FROM moves WHERE id="
                     << database::get_move_id(move_name);
        SQLite::Statement query(*_db, query_stream.str().c_str());
        if(query.executeStep())
            _raw.pc.moves[pos-1] = int(query.getColumn(0));
        else
            throw std::runtime_error("This move does not exist in Generation I.");
    }

    void pokemon_gen1impl::set_move_PP(uint8_t PP, uint8_t pos)
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        if(PP <= database::get_move_pp(_raw.pc.moves[pos-1]))
            _raw.pc.move_pps[pos-1] = PP;
        else
            throw std::runtime_error("This move PP is invalid.");
    }

    /*
     * Database Info
     */

    // Original game not recorded in Generation I
    uint16_t pokemon_gen1impl::get_original_game_id() const
    {
        return _version_id;
    }

    // No abilities in Generation I
    uint16_t pokemon_gen1impl::get_ability_id() const
    {
        return Abilities::NONE;
    }

    // No held items in Generation I
    uint16_t pokemon_gen1impl::get_item_id() const
    {
        return Items::NONE;
    }

    // No natures in Generation I
    uint16_t pokemon_gen1impl::get_nature_id() const
    {
        return Natures::NONE;
    }

    const void* pokemon_gen1impl::get_native()
    {
        return &_raw;
    }

    void pokemon_gen1impl::_set_experience(uint32_t exp)
    {
        _raw.pc.exp[0] = (exp /= 65536);
        _raw.pc.exp[1] = (exp /= 256);
        _raw.pc.exp[2] =  exp;

        _raw.pc.level = database::get_level(_species_id, exp);
        _raw.level    = _raw.pc.level;
    }

    void pokemon_gen1impl::_set_level(uint8_t level)
    {
        _raw.pc.level = level;
        _raw.level = level;

        uint32_t exp = database::get_experience(_species_id, level);
        _raw.pc.exp[0] = (exp /= 65536);
        _raw.pc.exp[1] = (exp /= 256);
        _raw.pc.exp[2] =  exp;
    }

    void pokemon_gen1impl::_set_stats()
    {
        pkmn::dict<pkmn::pkstring, uint16_t> stats = _pokedex_entry.base_stats;
        pkmn::dict<pkmn::pkstring, uint16_t> IVs   = get_IVs();

        _raw.max_hp = calculations::get_retro_stat(Stats::HP, stats["HP"], _raw.level,
                                                   _raw.pc.ev_hp, IVs["HP"]);
        _raw.pc.current_hp = _raw.max_hp;
        _raw.atk  = calculations::get_retro_stat(Stats::ATTACK, stats["Attack"], _raw.level,
                                                 _raw.pc.ev_atk, IVs["Attack"]);
        _raw.def  = calculations::get_retro_stat(Stats::DEFENSE, stats["Defense"], _raw.level,
                                                 _raw.pc.ev_def, IVs["Defense"]);
        _raw.spd  = calculations::get_retro_stat(Stats::SPEED, stats["Speed"], _raw.level,
                                                 _raw.pc.ev_spd, IVs["Speed"]);
        _raw.spcl  = calculations::get_retro_stat(Stats::SPECIAL, stats["Special"], _raw.level,
                                                  _raw.pc.ev_spcl, IVs["Special"]);
    }
}
