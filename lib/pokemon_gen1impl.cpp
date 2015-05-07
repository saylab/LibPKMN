/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cstring>

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include <pkmn/calculations.hpp>
#include <pkmn/database.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/conversions/misc.hpp>
#include <pkmn/types/prng.hpp>

#include "internal.hpp"
#include "pokemon_gen1impl.hpp"
#include "conversions/utils.hpp"

namespace pkmn
{
    pokemon_gen1impl::pokemon_gen1impl(int species, int version,
                                       int level,
                                       int move1, int move2,
                                       int move3, int move4):
        pokemon_impl(database::get_pokemon_game_index(species, version),
                     version, (species == Species::NONE)),
        _nickname(PKSTRING_UPPERCASE(database::get_species_name(species))),
        _otname("LIBPKMN")
    {
        /*
         * Populate native struct
         */
        if(_none or _invalid)
        {
            _nickname = "NONE";
            _otname = "LIBPKMN";
            memset(&_raw, 0x0, sizeof(pkmn::native::gen1_party_pokemon_t));
        }
        else
        {
            _raw.pc.species = database::get_pokemon_game_index(_species_id, _version_id);
            // current_hp populated by _set_stats()
            _set_level(level);
            _raw.pc.status = 0x00;
            _raw.pc.types[0] = gen1_type_indices.at(_pokedex_entry.types.first, 0x09);
            _raw.pc.types[1] = (_pokedex_entry.types.first == _pokedex_entry.types.first) ? _raw.pc.types[0]
                                                                                          : gen1_type_indices.at(_pokedex_entry.types.second, 0x09);
            _raw.pc.catch_rate = conversions::gen1_catch_rates[_species_id];
            _raw.pc.moves[0] = move1;
            _raw.pc.moves[1] = move2;
            _raw.pc.moves[2] = move3;
            _raw.pc.moves[3] = move4;
            _raw.pc.ot_id = pkmn::trainer::LIBPKMN_PUBLIC_ID;
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
    }

    pokemon_gen1impl::pokemon_gen1impl(const pkmn::native::gen1_pc_pokemon_t &raw,
                                       int version):
        pokemon_impl(raw.species, version, false),
        _nickname(UPPERCASE_SPECIES_NAME(raw.species, Versions::RED)),
        _otname("LIBPKMN")
    {
        _raw.pc = raw;
        if(not _none and not _invalid)
        {
            _raw.pc.level = database::get_level(_species_id, get_experience());
            _raw.level = database::get_level(_species_id, get_experience());
            _set_stats(); // Will populate party portion of struct
        }
    }

    pokemon_gen1impl::pokemon_gen1impl(const pkmn::native::gen1_pc_pokemon_t &raw,
                                       const pkmn::pkstring &nickname,
                                       const pkmn::pkstring &otname,
                                       int version):
        pokemon_impl(raw.species, version, false),
        _nickname(nickname),
        _otname(otname)
    {
        _raw.pc = raw;
        if(not _none and not _invalid)
        {
            _raw.pc.level = database::get_level(_species_id, get_experience());
            _raw.level = database::get_level(_species_id, get_experience());
            _set_stats(); // Will populate party portion of struct
        }
    }

    pokemon_gen1impl::pokemon_gen1impl(const pkmn::native::gen1_party_pokemon_t &raw,
                                       int version):
        pokemon_impl(raw.pc.species, version, false),
        _raw(raw),
        _nickname(UPPERCASE_SPECIES_NAME(raw.pc.species, Versions::RED)),
        _otname("LIBPKMN") {};

    pokemon_gen1impl::pokemon_gen1impl(const pkmn::native::gen1_party_pokemon_t &raw,
                                       const pkmn::pkstring &nickname,
                                       const pkmn::pkstring &otname,
                                       int version):
        pokemon_impl(raw.pc.species, version, false),
        _raw(raw),
        _nickname(nickname),
        _otname(otname) {};

    pokemon_gen1impl::pokemon_gen1impl(const pokemon_gen1impl &other):
        pokemon_impl(other),
        _raw(other._raw),
        _nickname(other._nickname),
        _otname(other._otname) {};

    pokemon_gen1impl& pokemon_gen1impl::operator=(const pokemon_gen1impl &other)
    {
        pokemon_impl::operator=(other);

        _raw      = other._raw;
        _nickname = other._nickname;
        _otname   = other._otname;

        return *this;
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
        return pkmn::markings_t(0);
    }

    // No ribbons in Generation I
    pkmn::ribbons_t pokemon_gen1impl::get_ribbons() const
    {
        pkmn::ribbons_t ribbons;
        ribbons.hoenn = 0;
        ribbons.sinnoh.ribbons1 = 0;
        ribbons.sinnoh.ribbons2 = 0;
        ribbons.sinnoh.ribbons3 = 0;
        ribbons.unova = 0;
        ribbons.kalos = 0;

        return ribbons;
    }

    // No Super Training in Generation I
    pkmn::super_training_medals_t pokemon_gen1impl::get_super_training_medals() const
    {
        return pkmn::super_training_medals_t(0);
    }

    // No Pokérus in Generation I
    pkmn::pokerus_t pokemon_gen1impl::get_pokerus() const
    {
        pkmn::pokerus_t pokerus;
        pokerus.strain = pkmn::pokerus_t::A;
        pokerus.num_days = 0;

        return pokerus;
    }

    // Met dates not recorded in Generation I
    pkmn::datetime_t pokemon_gen1impl::get_met_date(PKMN_UNUSED(bool as_egg)) const
    {
        return pkmn::datetime_t();
    }

    /*
     * Setting Non-battle info
     */

    // No contests in Generation I
    void pokemon_gen1impl::set_contest_stats(PKMN_UNUSED(const pkmn::contest_stats_t &contest_stats))
    {
        /* NOP */
    }

    // No markings in Generation I
    void pokemon_gen1impl::set_markings(PKMN_UNUSED(const pkmn::markings_t &markings))
    {
        /* NOP */
    }

    // No ribbons in Generation I
    void pokemon_gen1impl::set_ribbons(PKMN_UNUSED(const pkmn::ribbons_t &ribbons))
    {
        /* NOP */
    }

    // No Super Training in Generation I
    void pokemon_gen1impl::set_super_training_medals(PKMN_UNUSED(const pkmn::super_training_medals_t &super_training_medals))
    {
        /* NOP */
    }

    // No Pokérus in Generation I
    void pokemon_gen1impl::set_pokerus(PKMN_UNUSED(const pkmn::pokerus_t &pokerus))
    {
        /* NOP */
    }

    // Met dates not recorded in Generation I
    void pokemon_gen1impl::set_met_date(PKMN_UNUSED(pkmn::datetime_t &date), PKMN_UNUSED(bool as_egg))
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
    int pokemon_gen1impl::get_met_level() const
    {
        return _raw.level;
    }

    /*
     * Setting Trainer Info
     */

    void pokemon_gen1impl::set_nickname(const pkmn::pkstring &nickname)
    {
        if(nickname.length() > 10)
            throw std::runtime_error("Nicknames have a maximum of 10 characters.");

        _nickname = nickname;
    }

    void pokemon_gen1impl::set_trainer_name(const pkmn::pkstring &trainer_name)
    {
        if(trainer_name.length() > 7)
            throw std::runtime_error("Trainer names have a maximum of 7 characters.");

        _otname = trainer_name;
    }

    // All trainers are male in Generation I
    void pokemon_gen1impl::set_trainer_gender(PKMN_UNUSED(const pkmn::pkstring &gender))
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
    void pokemon_gen1impl::set_trainer_secret_id(PKMN_UNUSED(uint16_t id))
    {
        /* NOP */
    }

    // Ball not recorded in Generation I
    void pokemon_gen1impl::set_ball(PKMN_UNUSED(const pkmn::pkstring &ball))
    {
        /* NOP */
    }

    // Original game not recorded in Generation I
    void pokemon_gen1impl::set_original_game(PKMN_UNUSED(const pkmn::pkstring &game))
    {
        /* NOP */
    }

    // Met level not recorded in Generation I
    void pokemon_gen1impl::set_met_level(PKMN_UNUSED(int level))
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
    int pokemon_gen1impl::get_friendship() const
    {
        return 0;
    }

    int pokemon_gen1impl::get_level() const
    {
        return _raw.level;
    }

    // Stored in Base-256 in three bytes
    uint32_t pokemon_gen1impl::get_experience() const
    {
        return conversions::import_gb_experience((uint8_t*)_raw.pc.exp);
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

    pkmn::dict<pkmn::pkstring, int> pokemon_gen1impl::get_stats() const
    {
        pkmn::dict<pkmn::pkstring, int> stats;
        stats["HP"]      = _raw.max_hp;
        stats["Attack"]  = _raw.atk;
        stats["Defense"] = _raw.def;
        stats["Speed"]   = _raw.spd;
        stats["Special"] = _raw.spcl;

        return stats;
    }

    pkmn::dict<pkmn::pkstring, int> pokemon_gen1impl::get_EVs() const
    {
        pkmn::dict<pkmn::pkstring, int> EVs;
        EVs["HP"]      = BYTESWAP16(_raw.pc.ev_hp);
        EVs["Attack"]  = BYTESWAP16(_raw.pc.ev_atk);
        EVs["Defense"] = BYTESWAP16(_raw.pc.ev_def);
        EVs["Speed"]   = BYTESWAP16(_raw.pc.ev_spd);
        EVs["Special"] = BYTESWAP16(_raw.pc.ev_spcl);

        return EVs;
    }

    pkmn::dict<pkmn::pkstring, int> pokemon_gen1impl::get_IVs() const
    {
        return conversions::import_gb_IVs(_raw.pc.iv_data);
    }

    /*
     * Setting Individual Stat Info
     */

    // No personality in Generation I
    void pokemon_gen1impl::set_personality(PKMN_UNUSED(uint32_t personality))
    {
        /* NOP */
    }

    // No friendship in Generation I
    void pokemon_gen1impl::set_friendship(PKMN_UNUSED(int friendship))
    {
        /* NOP */
    }

    // NOTE: this changes experience and stats
    void pokemon_gen1impl::set_level(int level)
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
    void pokemon_gen1impl::set_gender(PKMN_UNUSED(const pkmn::pkstring &gender))
    {
        /* NOP */
    }

    // No natures in Generation I
    void pokemon_gen1impl::set_nature(PKMN_UNUSED(const pkmn::pkstring &nature_name))
    {
        /* NOP */
    }

    // No abilities in Generation I
    void pokemon_gen1impl::set_ability(PKMN_UNUSED(const pkmn::pkstring &ability))
    {
        /* NOP */
    }

    // No alternate forms in Generation I
    void pokemon_gen1impl::set_form(PKMN_UNUSED(const pkmn::pkstring &form))
    {
        /* NOP */
    }

    // No shininess in Generation I
    void pokemon_gen1impl::set_shiny(PKMN_UNUSED(bool value))
    {
        /* NOP */
    }

    // NOTE: this affects stats
    void pokemon_gen1impl::set_EV(const pkmn::pkstring &stat, int value)
    {
        if(stat == "Special Attack" or stat == "Special Defense")
            throw std::runtime_error("This stat is not present in Generation I.");

        if(value < 0 or value > 65535)
            throw std::runtime_error("EV must be 0-65535.");

        switch(database::get_stat_id(stat)) // Will throw if stat is invalid
        {
            case Stats::HP:
                _raw.pc.ev_hp = BYTESWAP16(value);
                break;

            case Stats::ATTACK:
                _raw.pc.ev_atk = BYTESWAP16(value);
                break;

            case Stats::DEFENSE:
                _raw.pc.ev_def = BYTESWAP16(value);
                break;

            case Stats::SPEED:
                _raw.pc.ev_spd = BYTESWAP16(value);
                break;

            default: // Stats::SPECIAL
                _raw.pc.ev_spcl = BYTESWAP16(value);
                break;
        }

        _set_stats();
    }

    // NOTE: this affects stats and other IV's
    void pokemon_gen1impl::set_IV(const pkmn::pkstring &stat, int value)
    {
        if(stat == "Special Attack" or stat == "Special Defense")
            throw std::runtime_error("This stat is not present in Generation I.");

        if(value < 0 or value > 15)
            throw std::runtime_error("IV must be 0-15.");

        // Will throw if stat is invalid
        conversions::export_gb_IV(stat, value, _raw.pc.iv_data);

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

    void pokemon_gen1impl::set_status(const pkmn::pkstring &status)
    {
        if(not conversions::reverse_retro_statuses.has_key(status))
            throw std::runtime_error("Invalid status given.");

        _raw.pc.status = conversions::reverse_retro_statuses.at(status);
    }

    // No held items in Generation I
    void pokemon_gen1impl::set_held_item(PKMN_UNUSED(const pkmn::pkstring &item_name))
    {
        /* NOP */
    }

    /*
     * Getting Move Info
     */

    pkmn::move_entry_t pokemon_gen1impl::get_move(int pos) const
    {
        if(pos < 1 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        try
        {
            return _pokedex->get_move_entry(_raw.pc.moves[pos-1]);
        }
        catch(...)
        {
            return _pokedex->get_move_entry(Moves::INVALID);
        }
    }

    void pokemon_gen1impl::get_moves(pkmn::moveset_t &moves) const
    {
        moves.clear();
        for(size_t i = 0; i < 4; i++)
            moves.push_back(get_move(i+1));
    }

    int pokemon_gen1impl::get_move_PP(int pos) const
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        return _raw.pc.move_pps[pos-1];
    }

    void pokemon_gen1impl::get_move_PPs(std::vector<int> &move_PPs) const
    {
        move_PPs.clear();
        for(size_t i = 0; i < 4; i++)
            move_PPs.push_back(_raw.pc.move_pps[i]);
    }

    void pokemon_gen1impl::set_move(const pkmn::pkstring &move_name, int pos)
    {
        if(pos < 1 or pos > 4)
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

    void pokemon_gen1impl::set_move_PP(int PP, int pos)
    {
        if(pos < 1 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        _raw.pc.move_pps[pos-1] = PP;
    }

    /*
     * Database Info
     */

    // Original game not recorded in Generation I
    int pokemon_gen1impl::get_original_game_id() const
    {
        return _version_id;
    }

    // No abilities in Generation I
    int pokemon_gen1impl::get_ability_id() const
    {
        return Abilities::NONE;
    }

    // No held items in Generation I
    int pokemon_gen1impl::get_item_id() const
    {
        return Items::NONE;
    }

    // No natures in Generation I
    int pokemon_gen1impl::get_nature_id() const
    {
        return Natures::NONE;
    }

    const void* pokemon_gen1impl::get_native()
    {
        return &_raw;
    }

    void pokemon_gen1impl::_set_experience(uint32_t exp)
    {
        conversions::export_gb_experience(_raw.pc.exp, exp);
        _raw.pc.level = database::get_level(_species_id, exp);
        _raw.level    = _raw.pc.level;
    }

    void pokemon_gen1impl::_set_level(int level)
    {
        _raw.pc.level = level;
        _raw.level = level;

        uint32_t exp = database::get_experience(_species_id, level);
        conversions::export_gb_experience(_raw.pc.exp, exp);
    }

    void pokemon_gen1impl::_set_stats()
    {
        pkmn::dict<pkmn::pkstring, int> stats = _pokedex_entry.base_stats;
        pkmn::dict<pkmn::pkstring, int> EVs   = get_EVs();
        pkmn::dict<pkmn::pkstring, int> IVs   = get_IVs();

        _raw.max_hp = BYTESWAP16(calculations::get_retro_stat("HP", stats["HP"], _raw.level,
                                                   EVs["HP"], IVs["HP"]));
        _raw.pc.current_hp = _raw.max_hp;
        _raw.atk  = BYTESWAP16(calculations::get_retro_stat("Attack", stats["Attack"], _raw.level,
                                                 EVs["Attack"], IVs["Attack"]));
        _raw.def  = BYTESWAP16(calculations::get_retro_stat("Defense", stats["Defense"], _raw.level,
                                                 EVs["Defense"], IVs["Defense"]));
        _raw.spd  = BYTESWAP16(calculations::get_retro_stat("Speed", stats["Speed"], _raw.level,
                                                 EVs["Speed"], IVs["Speed"]));
        _raw.spcl  = BYTESWAP16(calculations::get_retro_stat("Special", stats["Special"], _raw.level,
                                                  EVs["Special"], IVs["Special"]));
    }
}
