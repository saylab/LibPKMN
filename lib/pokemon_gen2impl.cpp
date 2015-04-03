/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/algorithm/string.hpp>

#include <pkmn/calculations.hpp>
#include <pkmn/database.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/trainer.hpp>
#include <pkmn/conversions/items.hpp>
#include <pkmn/conversions/misc.hpp>
#include <pkmn/conversions/pokemon.hpp>
#include <pkmn/conversions/text.hpp>
#include <pkmn/types/prng.hpp>

#include "pokemon_gen2impl.hpp"
#include "conversions/utils.hpp"

namespace pkmn
{
    pokemon_gen2impl::pokemon_gen2impl(uint16_t species, uint16_t version,
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
        if(_none or _invalid)
        {
            memset(&_raw, 0x0, sizeof(pkmn::native::gen2_party_pokemon_t));
        }
        else
        {
            _raw.pc.species = database::get_pokemon_game_index(_species_id, _version_id);
            _raw.pc.held_item = 0;
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
            _raw.pc.friendship = uint8_t(_pokedex_entry.base_friendship);
            _raw.pc.pokerus = pkmn::pokerus_t();
            if(_version_id == Versions::CRYSTAL)
            {
                _raw.pc.caught_data = ((level & 0x3F) << 8);
                // Trainer male by default, can be set
                // TODO: met location
            }
            _set_level(level);
            _raw.status = 0x00;
            _raw.unused = 0x00; // TODO: check in-game data to see if there is any expected value
            _set_stats(); // Will populate rest of party portion of struct
        }
    }

    pokemon_gen2impl::pokemon_gen2impl(const pkmn::native::gen2_pc_pokemon_t &raw,
                                       uint8_t version):
        pokemon_impl(database::get_pokemon_id(raw.species, version),
                     version),
        _nickname(UPPERCASE_SPECIES_NAME(raw.species, Versions::GOLD)),
        _otname("LIBPKMN")
    {
        _raw.pc = raw;
        _none = false;
        try
        {
            PKMN_UNUSED(uint16_t pokemon_id) = database::get_pokemon_id(_raw.pc.species, Versions::GOLD);
            _invalid = false;
        }
        catch(...)
        {
            _invalid = true;
        }

        _set_stats(); // Will populate rest of party portion in struct
    }

    pokemon_gen2impl::pokemon_gen2impl(const pkmn::native::gen2_pc_pokemon_t &raw,
                                       const pkmn::pkstring &nickname,
                                       const pkmn::pkstring &otname,
                                       uint8_t version):
        pokemon_impl(database::get_pokemon_id(raw.species, version),
                     version),
        _nickname(nickname),
        _otname(otname)
    {
        _raw.pc = raw;
        _none = false;
        try
        {
            PKMN_UNUSED(uint16_t pokemon_id) = database::get_pokemon_id(_raw.pc.species, Versions::GOLD);
            _invalid = false;
        }
        catch(...)
        {
            _invalid = true;
        }

        _set_stats(); // Will populate rest of party portion in struct
    }

    pokemon_gen2impl::pokemon_gen2impl(const pkmn::native::gen2_party_pokemon_t &raw,
                                       uint8_t version):
        pokemon_impl(database::get_pokemon_id(raw.pc.species, version),
                     version),
        _raw(raw),
        _nickname(UPPERCASE_SPECIES_NAME(raw.pc.species, Versions::GOLD)),
        _otname("LIBPKMN")
    {
        _none = false;
        try
        {
            PKMN_UNUSED(uint16_t pokemon_id) = database::get_pokemon_id(_raw.pc.species, Versions::GOLD);
            _invalid = false;
        }
        catch(...)
        {
            _invalid = true;
        }
    }

    pokemon_gen2impl::pokemon_gen2impl(const pkmn::native::gen2_party_pokemon_t &raw,
                                       const pkmn::pkstring &nickname,
                                       const pkmn::pkstring &otname,
                                       uint8_t version):
        pokemon_impl(database::get_pokemon_id(raw.pc.species, version),
                     version),
        _raw(raw),
        _nickname(nickname),
        _otname(otname)
    {
        _none = false;
        try
        {
            PKMN_UNUSED(uint16_t pokemon_id) = database::get_pokemon_id(_raw.pc.species, Versions::GOLD);
            _invalid = false;
        }
        catch(...)
        {
            _invalid = true;
        }
    }

    pokemon_gen2impl::pokemon_gen2impl(SQLite::Database &db)
    {
        CONNECT_TO_DB(pokemon_impl::_db);

        SQLite::Statement pkmn_query(db, "SELECT * FROM pkmn");
        pkmn_query.executeStep();

        _species_id = database::get_species_id(int(pkmn_query.getColumn(0))); // pokemon_id

        std::ostringstream form_querystream;
        form_querystream << "SELECT id FROM pokemon_forms WHERE pokemon_id=" << _species_id;
        _form_id = _db->execAndGet(form_querystream.str().c_str());

        _version_id = pkmn_query.getColumn(1); // game_id
        _none = _invalid = false;
        _pokedex = pokedex::make(database::get_version_name(_version_id));
        _pokedex_entry = _pokedex->get_pokemon_entry(_species_id, _form_id);

        _nickname = pkmn_query.getColumn(2); // nickname
        _otname = pkmn_query.getColumn(3); // otname

        _raw.pc.species = database::get_pokemon_game_index(pkmn_query.getColumn(0), Versions::CRYSTAL); // pokemon_id
        _raw.pc.held_item = database::get_item_game_index(pkmn_query.getColumn(28), Versions::CRYSTAL); // held_item_id
        _raw.pc.moves[0] = pkmn_query.getColumn(29); // move1_id
        _raw.pc.moves[1] = pkmn_query.getColumn(30); // move2_id
        _raw.pc.moves[2] = pkmn_query.getColumn(31); // move3_id
        _raw.pc.moves[3] = pkmn_query.getColumn(32); // move4_id
        _raw.pc.ot_id = pkmn_query.getColumn(5); // ot_id
        _raw.pc.ev_hp = pkmn_query.getColumn(16); // ev_hp
        _raw.pc.ev_atk = pkmn_query.getColumn(17); // ev_attack
        _raw.pc.ev_def = pkmn_query.getColumn(18); // ev_defense
        _raw.pc.ev_spd = pkmn_query.getColumn(19); // ev_speed
        _raw.pc.ev_spcl = pkmn_query.getColumn(20); // ev_spatk
        conversions::export_gb_IV("Attack", pkmn_query.getColumn(23), _raw.pc.iv_data); // iv_attack
        conversions::export_gb_IV("Defense", pkmn_query.getColumn(24), _raw.pc.iv_data); // iv_defense
        conversions::export_gb_IV("Speed", pkmn_query.getColumn(25), _raw.pc.iv_data); // iv_speed
        conversions::export_gb_IV("Special", pkmn_query.getColumn(26), _raw.pc.iv_data); // iv_spatk
        conversions::export_gb_IV("HP", pkmn_query.getColumn(22), _raw.pc.iv_data); // iv_hp
        _raw.pc.move_pps[0] = pkmn_query.getColumn(33); // move1_pp
        _raw.pc.move_pps[1] = pkmn_query.getColumn(34); // move2_pp
        _raw.pc.move_pps[2] = pkmn_query.getColumn(35); // move3_pp
        _raw.pc.move_pps[3] = pkmn_query.getColumn(36); // move4_pp
        _raw.pc.friendship = pkmn_query.getColumn(10); // friendship
        _raw.pc.pokerus = pkmn_query.getColumn(45); // pokerus

        _raw.pc.caught_data = (uint8_t(pkmn_query.getColumn(8)) & 0x3F) << 8; // met_level
        _raw.pc.caught_data |= ((uint8_t(pkmn_query.getColumn(4)) & 0x1) << 7); // ot_female

        _set_experience(pkmn_query.getColumn(12)); // experience
        _set_stats();

        SQLite::Statement attributes_query(db, "SELECT * FROM pkmn_attributes");
        while(attributes_query.executeStep())
            _attributes.insert(attributes_query.getColumn(0),  // name
                               attributes_query.getColumn(1)); // value
    }

    pokemon_gen2impl::pokemon_gen2impl(const pokemon_gen2impl &other):
        pokemon_impl(other),
        _raw(other._raw),
        _nickname(other._nickname),
        _otname(other._otname) {};

    pokemon_gen2impl &pokemon_gen2impl::operator=(const pokemon_gen2impl &other)
    {
        pokemon_impl::operator=(other);

        _raw      = other._raw;
        _nickname = other._nickname;
        _otname   = other._otname;

        return *this;
    }

    /*
     * Getting Non-battle Info
     */

    // No contests in Generation II
    pkmn::contest_stats_t pokemon_gen2impl::get_contest_stats() const
    {
        return pkmn::contest_stats_t();
    }

    // No markings in Generation II
    pkmn::markings_t pokemon_gen2impl::get_markings() const
    {
        return pkmn::markings_t();
    }

    // No ribbons in Generation II
    pkmn::ribbons_t pokemon_gen2impl::get_ribbons() const
    {
        return pkmn::ribbons_t();
    }

    // No Super Training in Generation II
    pkmn::super_training_medals_t pokemon_gen2impl::get_super_training_medals() const
    {
        return pkmn::super_training_medals_t();
    }

    pkmn::pokerus_t pokemon_gen2impl::get_pokerus() const
    {
        return _raw.pc.pokerus;
    }

    /*
     * Setting Non-battle Info
     */

    // No contests in Generation II
    void pokemon_gen2impl::set_contest_stats(PKMN_UNUSED(const pkmn::contest_stats_t &contest_stats))
    {
        /* NOP */
    }

    // No markings in Generation II
    void pokemon_gen2impl::set_markings(PKMN_UNUSED(const pkmn::markings_t &markings))
    {
        /* NOP */
    }

    // No ribbons in Generation II
    void pokemon_gen2impl::set_ribbons(PKMN_UNUSED(const pkmn::ribbons_t &ribbons))
    {
        /* NOP */
    }

    // No Super Training in Generation II
    void pokemon_gen2impl::set_super_training_medals(PKMN_UNUSED(const pkmn::super_training_medals_t &super_training_medals))
    {
        /* NOP */
    }

    void pokemon_gen2impl::set_pokerus(const pkmn::pokerus_t &pokerus)
    {
        pkmn::pokerus_t _pokerus = pokerus;
        _raw.pc.pokerus = _pokerus;
    }

    /*
     * Getting Trainer Info
     */

    pkmn::pkstring pokemon_gen2impl::get_nickname() const
    {
        return _nickname;
    }

    pkmn::pkstring pokemon_gen2impl::get_trainer_name() const
    {
        return _otname;
    }

    // All trainers are male in Gold/Silver
    pkmn::pkstring pokemon_gen2impl::get_trainer_gender() const
    {
        return (_raw.pc.caught_data  &0x80) ? "Female" : "Male";
    }

    // Trainer ID is only 16 bits, all publicly exposed, in Generation II
    uint32_t pokemon_gen2impl::get_trainer_id() const
    {
        return _raw.pc.ot_id;
    }

    // Trainer ID is only 16 bits, all publicly exposed, in Generation II
    uint16_t pokemon_gen2impl::get_trainer_public_id() const
    {
        return _raw.pc.ot_id;
    }

    // Trainer ID is only 16 bits, all publicly exposed, in Generation II
    uint16_t pokemon_gen2impl::get_trainer_secret_id() const
    {
        return 0;
    }

    // Ball not recorded in Generation II
    pkmn::pkstring pokemon_gen2impl::get_ball() const
    {
        return database::get_ball_name(Balls::POKE_BALL);
    }

    // Original game not recorded in Generation II
    pkmn::pkstring pokemon_gen2impl::get_original_game() const
    {
        return get_game();
    }

    uint8_t pokemon_gen2impl::get_met_level() const
    {
        return (_version_id == Versions::CRYSTAL) ? ((_raw.pc.caught_data  &0x3F00) >> 8)
                                                  : _raw.pc.level;
    }

    /*
     * Setting Trainer Info
     */

    void pokemon_gen2impl::set_nickname(const pkmn::pkstring &nickname)
    {
        if(nickname.length() > 10)
            throw std::runtime_error("Nicknames can have a maximum of 10 characters in Generation II.");

        _nickname = nickname;
    }

    void pokemon_gen2impl::set_trainer_name(const pkmn::pkstring &trainer_name)
    {
        if(trainer_name.length() > 7)
            throw std::runtime_error("Trainer names can have a maximum of 7 characters in Generation II.");

        _otname = trainer_name;
    }

    // All trainers male in Gold/Silver
    void pokemon_gen2impl::set_trainer_gender(const pkmn::pkstring &gender)
    {
        if(gender.std_string() != "Male" and gender.std_string() != "Female")
            throw std::runtime_error("Gender must be male or female.");

        if(_version_id == Versions::CRYSTAL)
        {
            if(gender == "Male") _raw.pc.caught_data &= ~(0x80);
            else                 _raw.pc.caught_data |= 0x80;
        }
    }

    // Trainer ID is only 16 bits, all publicly exposed, in Generation II
    void pokemon_gen2impl::set_trainer_id(uint32_t id)
    {
        _raw.pc.ot_id = uint16_t(id);
    }

    // Trainer ID is only 16 bits, all publicly exposed, in Generation II
    void pokemon_gen2impl::set_trainer_public_id(PKMN_UNUSED(uint16_t id))
    {
        _raw.pc.ot_id = id;
    }

    // Trainer ID is only 16 bits, all publicly exposed, in Generation II
    void pokemon_gen2impl::set_trainer_secret_id(PKMN_UNUSED(uint16_t id))
    {
        /* NOP */
    }

    // Ball not recorded in Generation II
    void pokemon_gen2impl::set_ball(PKMN_UNUSED(const pkmn::pkstring &ball))
    {
        /* NOP */
    }

    // Use original game to check if caught data needs to be set (Crystal only)
    void pokemon_gen2impl::set_original_game(const pkmn::pkstring &game)
    {
        if(database::get_generation(game) > 2)
            throw std::runtime_error("Original game must be from Generation I-II.");

        if(database::get_version_id(game) == Versions::CRYSTAL)
            _reset_caught_data();
        else
            _raw.pc.caught_data = 0;
    }

    // Met level only tracked in Crystal
    void pokemon_gen2impl::set_met_level(uint8_t level)
    {
        /*
         * Pokémon in Generation I-II cannot be level 1.
         *
         * http://bulbapedia.bulbagarden.net/wiki/Experience#Experience_underflow_glitch
         */
        if(level < 2 or level > 100)
            throw std::runtime_error("Level must be 2-100.");

        if(_version_id == Versions::CRYSTAL)
            _raw.pc.caught_data |= ((level & 0x3F) << 8);
    }

    /*
     * Getting Individual Stat Info
     */

    // No personality in Generation II
    uint32_t pokemon_gen2impl::get_personality() const
    {
        return 0;
    }

    uint8_t pokemon_gen2impl::get_friendship() const
    {
        return _raw.pc.friendship;
    }

    uint8_t pokemon_gen2impl::get_level() const
    {
        return _raw.pc.level;
    }

    // Stored in Base-256 in three bytes
    uint32_t pokemon_gen2impl::get_experience() const
    {
        return (65536*_raw.pc.exp[0]) + (256*_raw.pc.exp[1]) * _raw.pc.exp[2];
    }

    pkmn::pkstring pokemon_gen2impl::get_gender() const
    {
        float chance_male   = _pokedex_entry.chance_male;
        float chance_female = _pokedex_entry.chance_female;
        uint8_t ivATK = conversions::import_gb_IVs(_raw.pc.iv_data)["Attack"];

        if((chance_male + chance_female) == 0.0) return "Genderless";
        else if(chance_male == 1.0) return "Male";
        else if(chance_female == 1.0) return "Female";
        else
        {
            /*
             * Gender is determined by gender ratio and Attack IV
             * If the Attack IV is below a certain number, the Pokemon will be female
             */
            if(chance_male      == 0.875) return (ivATK > 1)  ? "Male" : "Female";
            else if(chance_male == 0.75)  return (ivATK > 3)  ? "Male" : "Female";
            else if(chance_male == 0.5)   return (ivATK > 7)  ? "Male" : "Female";
            else                          return (ivATK > 11) ? "Male" : "Female";
        }
    }

    // No natures in Generation II
    pkmn::nature_t pokemon_gen2impl::get_nature() const
    {
        return pkmn::nature_t();
    }

    // No abilities in Generation II
    pkmn::pkstring pokemon_gen2impl::get_ability() const
    {
        return "None";
    }

    bool pokemon_gen2impl::is_shiny() const
    {
        pkmn::dict<pkmn::pkstring, uint16_t> IVs = conversions::import_gb_IVs(_raw.pc.iv_data);

        uint8_t ivATK  = IVs["Attack"];
        uint8_t ivDEF  = IVs["Defense"];
        uint8_t ivSPD  = IVs["Speed"];
        uint8_t ivSPCL = IVs["Special"];

        return (ivSPD == 10 and ivDEF == 10 and ivSPCL == 10 and 
                (ivATK == 2 or ivATK == 3 or ivATK == 6 or
                 ivATK == 7 or ivATK == 10 or ivATK == 11 or
                 ivATK == 14 or ivATK == 15) 
               );
    }

    pkmn::dict<pkmn::pkstring, uint16_t> pokemon_gen2impl::get_stats() const
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

    pkmn::dict<pkmn::pkstring, uint16_t> pokemon_gen2impl::get_EVs() const
    {
        pkmn::dict<pkmn::pkstring, uint16_t> EVs;
        EVs["HP"]      = _raw.pc.ev_hp;
        EVs["Attack"]  = _raw.pc.ev_atk;
        EVs["Defense"] = _raw.pc.ev_def;
        EVs["Speed"]   = _raw.pc.ev_spd;
        EVs["Special"] = _raw.pc.ev_spcl;

        return EVs;
    }

    pkmn::dict<pkmn::pkstring, uint16_t> pokemon_gen2impl::get_IVs() const
    {
        return conversions::import_gb_IVs(_raw.pc.iv_data);
    }

    /*
     * Setting Individual Stat Info
     */

    // No personality in Generation II
    void pokemon_gen2impl::set_personality(PKMN_UNUSED(uint32_t personality))
    {
        /* NOP */
    }

    void pokemon_gen2impl::set_friendship(uint8_t friendship)
    {
        _raw.pc.friendship = friendship;
    }

    // NOTE: this changes experience and stats
    void pokemon_gen2impl::set_level(uint8_t level)
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
    void pokemon_gen2impl::set_experience(unsigned int experience)
    {
        _set_experience(experience);
        _set_stats();
    }

    // NOTE: this affects IV's, which affects shininess
    void pokemon_gen2impl::set_gender(const pkmn::pkstring &gender)
    {
        float chance_male   = _pokedex_entry.chance_male;
        float chance_female = _pokedex_entry.chance_female;
        uint8_t new_value;

        // Sanity check
        if(((chance_male + chance_female) == 0) or
            (chance_male == 1.0) or (chance_female == 1.0))
            throw std::runtime_error("This species's gender cannot be changed.");
        else if(gender.std_string() != "Male" and gender.std_string() != "Female")
            throw std::runtime_error("Gender can only be \"Male\" or \"Female\".");
        else
        {
            /*
             * Gender is determined by the Attack IV. Below a certain
             * threshold, the Pokémon is female. As such, if the user
             * sets the gender to female, give the maximum IV value.
             */
            if(chance_male == 0.875)
                new_value = (gender == "Male") ? 15 : 1;
            else if(chance_male == 0.75)
                new_value = (gender == "Male") ? 15 : 3;
            else if(chance_male == 0.5)
                new_value = (gender == "Male") ? 15 : 7;
            else
                new_value = (gender == "Male") ? 15 : 11;
        }

        conversions::export_gb_IV("Attack", new_value, _raw.pc.iv_data);
    }

    // No natures in Generation II
    void pokemon_gen2impl::set_nature(PKMN_UNUSED(const pkmn::pkstring &nature_name))
    {
        /* NOP */
    }

    // No abilities in Generation II
    void pokemon_gen2impl::set_ability(PKMN_UNUSED(const pkmn::pkstring &ability))
    {
        /* NOP */
    }

    void pokemon_gen2impl::set_form(const pkmn::pkstring &form)
    {
        // Note: this will affect other things
        if(_species_id == Species::UNOWN)
        {
            uint16_t new_form = database::get_form_id(database::get_species_name(_species_id), form);

            // Get the closest IV value, will probably raise Special IV
            while(_form_id != new_form)
            {
                _raw.pc.iv_data++;

                pkmn::dict<pkmn::pkstring, uint16_t> IVs = conversions::import_gb_IVs(_raw.pc.iv_data);

                pkmn::pkstring unown_form = calculations::get_gen2_unown_form(IVs["Attack"],
                                                                              IVs["Defense"],
                                                                              IVs["Special"],
                                                                              IVs["Speed"]);
                _form_id = database::get_form_id("Unown", unown_form);
            }

            _pokedex_entry = _pokedex->get_pokemon_entry(Species::UNOWN, _form_id);

            _set_stats();
        }
        else throw std::runtime_error("This Pokémon has no alternate forms.");
    }

    void pokemon_gen2impl::set_shiny(bool value)
    {
        /*
         * Shininess depends on certain IV values. This will inevitably
         * affect other things, but we will set the best possible stats
         * that will allow shininess.
         */
        if(value)
        {
            conversions::export_gb_IV("Attack",  15, _raw.pc.iv_data);
            conversions::export_gb_IV("Defense", 10, _raw.pc.iv_data);
            conversions::export_gb_IV("Speed",   10, _raw.pc.iv_data);
            conversions::export_gb_IV("Special", 10, _raw.pc.iv_data);
        }
        else
        {
            conversions::export_gb_IV("Attack",  15, _raw.pc.iv_data);
            conversions::export_gb_IV("Defense", 11, _raw.pc.iv_data);
            conversions::export_gb_IV("Speed",   11, _raw.pc.iv_data);
            conversions::export_gb_IV("Special", 11, _raw.pc.iv_data);
        }
    }

    // NOTE: this affects stats
    void pokemon_gen2impl::set_EV(const pkmn::pkstring &stat, uint16_t value)
    {
        if(stat == "Special Attack" or stat == "Special Defense")
            throw std::runtime_error("Special Attack and Special Defense use the"
                                     "Special EV in Generation II.");

        if(value > 65535)
            throw std::runtime_error("EV's have a maximum value of 65535 in Generation II.");

        switch(database::get_stat_id(stat)) // Will throw if stat_name is invalid
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

    // NOTE: this affects stats, other IV's, and Unown's form
    void pokemon_gen2impl::set_IV(const pkmn::pkstring &stat, uint16_t value)
    {
        if(stat == "Special Attack" or stat == "Special Defense")
            throw std::runtime_error("Special Attack and Special Defense use the"
                                     "Special IV in Generation II.");

        if(value > 15)
            throw std::runtime_error("IV's have a maximum value of 15 in Generation II.");

        // Will throw if stat_name is invalid
        conversions::export_gb_IV(stat, value, _raw.pc.iv_data);

        if(_species_id == Species::UNOWN)
        {
            pkmn::dict<pkmn::pkstring, uint16_t> IVs = conversions::import_gb_IVs(_raw.pc.iv_data);

            pkmn::pkstring unown_form = calculations::get_gen2_unown_form(IVs["Attack"],
                                                                          IVs["Defense"],
                                                                          IVs["Special"],
                                                                          IVs["Speed"]);

            _form_id = database::get_form_id("Unown", unown_form);
            _pokedex_entry = _pokedex->get_pokemon_entry(Species::UNOWN, _form_id);
        }

        _set_stats();
    }

    /*
     * Battle Stat Info
     */

    pkmn::pkstring pokemon_gen2impl::get_status() const
    {
        return conversions::retro_statuses.at(_raw.status, "OK");
    }

    pkmn::item_entry_t pokemon_gen2impl::get_held_item() const
    {
        try
        {
            return _pokedex->get_item_entry(get_item_id());
        }
        catch(...)
        {
            return _pokedex->get_item_entry(Items::INVALID);
        }
    }

    void pokemon_gen2impl::set_status(const pkmn::pkstring &status)
    {
        if(not conversions::reverse_retro_statuses.has_key(status))
            throw std::runtime_error("Invalid status given.");

        _raw.status = conversions::reverse_retro_statuses.at(status);
    }

    void pokemon_gen2impl::set_held_item(const pkmn::pkstring &item_name)
    {
        _raw.pc.held_item = database::get_item_game_index(item_name,
                                                          database::get_version_name(_version_id));
    }

    pkmn::move_entry_t pokemon_gen2impl::get_move(uint8_t pos) const
    {
        if(pos == 0 or pos > 4)
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

    void pokemon_gen2impl::get_moves(pkmn::moveset_t &moves) const
    {
        moves.clear();
        for(size_t i = 0; i < 4; i++) moves.push_back(get_move(i+1));
    }

    uint8_t pokemon_gen2impl::get_move_PP(uint8_t pos) const
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        return _raw.pc.move_pps[pos-1];
    }

    void pokemon_gen2impl::get_move_PPs(std::vector<uint8_t> &move_PPs) const
    {
        move_PPs.clear();
        for(size_t i = 0; i < 4; i++)
            move_PPs.push_back(_raw.pc.move_pps[i]);
    }

    void pokemon_gen2impl::set_move(const pkmn::pkstring &move_name, uint8_t pos)
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

    void pokemon_gen2impl::set_move_PP(uint8_t PP, uint8_t pos)
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

    uint16_t pokemon_gen2impl::get_original_game_id() const
    {
        return _version_id;
    }

    // No abilities in Generation II
    uint16_t pokemon_gen2impl::get_ability_id() const
    {
        return Abilities::NONE;
    }

    uint16_t pokemon_gen2impl::get_item_id() const
    {
        return database::get_item_id(_raw.pc.held_item, _version_id);
    }

    // No natures in Generation II
    uint16_t pokemon_gen2impl::get_nature_id() const
    {
        return Natures::NONE;
    }

    const void* pokemon_gen2impl::get_native()
    {
        return &_raw;
    }

    void pokemon_gen2impl::_set_experience(uint32_t exp)
    {
        _raw.pc.exp[0] = (exp /= 65536);
        _raw.pc.exp[1] = (exp /= 256);
        _raw.pc.exp[2] =  exp;

        _raw.pc.level = database::get_level(_species_id, exp);
    }

    void pokemon_gen2impl::_set_level(uint8_t level)
    {
        _raw.pc.level = level;

        uint32_t exp = database::get_experience(_species_id, level);
        _raw.pc.exp[0] = (exp /= 65536);
        _raw.pc.exp[1] = (exp /= 256);
        _raw.pc.exp[2] =  exp;
    }

    void pokemon_gen2impl::_set_stats()
    {
        pkmn::dict<pkmn::pkstring, uint16_t> stats = _pokedex_entry.base_stats;
        pkmn::dict<pkmn::pkstring, uint16_t> IVs = get_IVs();

        _raw.max_hp = calculations::get_retro_stat("HP", stats["HP"], _raw.pc.level,
                                                   _raw.pc.ev_hp, IVs["HP"]);
        _raw.current_hp = _raw.max_hp;
        _raw.atk   = calculations::get_retro_stat("Attack", stats["Attack"], _raw.pc.level,
                                                  _raw.pc.ev_atk, IVs["Attack"]);
        _raw.def   = calculations::get_retro_stat("Defense", stats["Defense"], _raw.pc.level,
                                                  _raw.pc.ev_def, IVs["Defense"]);
        _raw.spd   = calculations::get_retro_stat("Speed", stats["Speed"], _raw.pc.level,
                                                  _raw.pc.ev_spd, IVs["Speed"]);
        _raw.spatk = calculations::get_retro_stat("Special Attack", stats["Special Attack"], _raw.pc.level,
                                                  _raw.pc.ev_spcl, IVs["Special"]);
        _raw.spdef = calculations::get_retro_stat("Special Defense", stats["Special Defense"], _raw.pc.level,
                                                  _raw.pc.ev_spcl, IVs["Special"]);
    }

    void pokemon_gen2impl::_reset_caught_data()
    {
        if(_version_id == Versions::CRYSTAL) _raw.pc.caught_data = ((_raw.pc.level  &0x3F) << 8);
        else _raw.pc.caught_data = 0;
    }
}
