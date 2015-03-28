/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include <pkmn/calculations.hpp>
#include <pkmn/database.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/conversions/text.hpp>
#include <pkmn/types/prng.hpp>

#include "internal.hpp"
#include "pokemon_gen6impl.hpp"
#include "conversions/utils.hpp"

namespace pkmn
{
    pokemon_gen6impl::pokemon_gen6impl(uint16_t species, uint16_t version,
                                       uint8_t level,
                                       uint8_t move1, uint8_t move2,
                                       uint8_t move3, uint8_t move4):
        pokemon_impl(species, version)
    {
        _blockA = &(_raw.pc.blocks.blockA);
        _blockB = &(_raw.pc.blocks.blockB);
        _blockC = &(_raw.pc.blocks.blockC);
        _blockD = &(_raw.pc.blocks.blockD);

        /*
         * Populate native struct
         */
        if(_none or _invalid)
        {
            memset(&_raw, 0x0, sizeof(pkmn::native::gen6_party_pokemon_t));
        }
        else
        {
            _blockA->personality = _prng->lcrng();
            _blockA->species = database::get_pokemon_game_index(_species_id, _version_id);
            _blockA->held_item = Items::NONE;
            _blockA->ot_id = pkmn::trainer::LIBPKMN_TRAINER_ID;
            // Experience set my level
            _blockA->ability = (_blockA->personality % 2) ?
                                  database::get_ability_id(_pokedex_entry.abilities.second)
                                : database::get_ability_id(_pokedex_entry.abilities.first);
            _blockA->ability_num = _blockA->personality % 2;
            _blockA->nature = _blockA->personality % 24;
            _blockA->form_encounterinfo = 1; // Fateful encounter
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
            _blockA->pokerus = pkmn::pokerus_t();

            conversions::export_modern_text(_pokedex_entry.species_name,
                                            _blockB->nickname, 12);
            _blockB->moves[0] = move1;
            _blockB->moves[1] = move2;
            _blockB->moves[2] = move3;
            _blockB->moves[3] = move4;
            for(size_t i = 0; i < 4; i++)
                _blockB->move_pps[i] = database::get_move_pp(_blockB->moves[i]);
            _blockB->iv_isegg_isnicknamed = _prng->lcrng();
            _blockB->iv_isegg_isnicknamed &= ~(2<<30); // Not an egg, not nicknamed
            _set_default_gender();

            conversions::export_modern_text("LibPKMN",
                                            _blockD->otname, 7);
            _blockD->ot_friendship = _pokedex_entry.base_friendship;
            // TODO: met dates
            _blockD->eggmet_location = 2008; // Distant land
            _blockD->met_location = 2008; // Distant land
            _blockD->ball = Balls::LUXURY_BALL;
            _blockD->metlevel_otgender = (level | ~(1<<31));
            _blockD->ot_game = database::get_version_game_index(_version_id);

            // TODO: distinguish country vs. language
            _blockD->ot_country = 0x2; // English
            // TODO: regions
            _blockD->ot_language = 0x2; // English

            _blockC->not_ot_gender = (_blockD->metlevel_otgender | (1<<31)) ? 1 : 0;
            _blockC->not_ot_friendship = _blockD->ot_friendship;

            _set_level(level);
            _set_stats();
        }
    }

    pokemon_gen6impl::pokemon_gen6impl(const pkmn::native::gen6_pc_pokemon_t &raw,
                                       uint8_t version):
        pokemon_impl(database::get_pokemon_id(raw.blocks.blockA.species, version),
                     version)
    {
        _raw.pc = raw;
        _blockA = &(_raw.pc.blocks.blockA);
        _blockB = &(_raw.pc.blocks.blockB);
        _blockC = &(_raw.pc.blocks.blockC);
        _blockD = &(_raw.pc.blocks.blockD);
        // TODO: set form
        _none = false;
        try
        {
            PKMN_UNUSED(uint16_t pokemon_id) = database::get_pokemon_id(_blockA->species, Versions::X);
            _invalid = false;
        }
        catch(...)
        {
            _invalid = true;
        }

        _set_stats();
    }

    pokemon_gen6impl::pokemon_gen6impl(const pkmn::native::gen6_party_pokemon_t &raw,
                                       uint8_t version):
        pokemon_impl(database::get_pokemon_id(raw.pc.blocks.blockA.species, version),
                     version),
        _raw(raw)
    {
        _blockA = &(_raw.pc.blocks.blockA);
        _blockB = &(_raw.pc.blocks.blockB);
        _blockC = &(_raw.pc.blocks.blockC);
        _blockD = &(_raw.pc.blocks.blockD);
        // TODO: set form
        _none = false;
        try
        {
            PKMN_UNUSED(uint16_t pokemon_id) = database::get_pokemon_id(_blockA->species, Versions::HEARTGOLD);
            _invalid = false;
        }
        catch(...)
        {
            _invalid = true;
        }

        _set_stats();
    }

    pokemon_gen6impl::pokemon_gen6impl(const pokemon_gen6impl &other):
        pokemon_impl(other),
        _raw(other._raw)
    {
        _blockA = &(_raw.pc.blocks.blockA);
        _blockB = &(_raw.pc.blocks.blockB);
        _blockC = &(_raw.pc.blocks.blockC);
        _blockD = &(_raw.pc.blocks.blockD);
        // TODO: set form
    }

    pokemon_gen6impl &pokemon_gen6impl::operator=(const pokemon_gen6impl &other)
    {
        pokemon_impl::operator=(other);

        _blockA = &(_raw.pc.blocks.blockA);
        _blockB = &(_raw.pc.blocks.blockB);
        _blockC = &(_raw.pc.blocks.blockC);
        _blockD = &(_raw.pc.blocks.blockD);

        return *this;
    }

    /*
     * Getting Non-Battle Info
     */

    pkmn::contest_stats_t pokemon_gen6impl::get_contest_stats() const
    {
        return _blockA->contest_stats;
    }

    pkmn::markings_t pokemon_gen6impl::get_markings() const
    {
        return _blockA->markings;
    }

    pkmn::ribbons_t pokemon_gen6impl::get_ribbons() const
    {
        pkmn::ribbons_t ribbons;
        uint64_t ribbonint = *reinterpret_cast<uint64_t*>(&_blockA->ribbons[0]);
        ribbonint >>= 16;
        ribbons.kalos = ribbonint;

        return ribbons;
    }

    pkmn::super_training_medals_t pokemon_gen6impl::get_super_training_medals() const
    {
        return _blockA->super_training_medals;
    }

    pkmn::pokerus_t pokemon_gen6impl::get_pokerus() const
    {
        return _blockA->pokerus;
    }

    /*
     * Setting Non-Battle Info
     */

    void pokemon_gen6impl::set_contest_stats(const pkmn::contest_stats_t &contest_stats)
    {
        _blockA->contest_stats = contest_stats;
    }

    void pokemon_gen6impl::set_markings(const pkmn::markings_t &markings)
    {
        pkmn::markings_t _markings = markings;
        _blockA->markings = _markings;
    }

    void pokemon_gen6impl::set_ribbons(const pkmn::ribbons_t &ribbons)
    {
        pkmn::ribbons_t _ribbons = ribbons;
        uint64_t ribbonint = _ribbons.kalos;
        ribbonint <<= 16;
        memcpy(&_blockA->ribbons[0], &ribbonint, 6);
    }

    void pokemon_gen6impl::set_super_training_medals(const pkmn::super_training_medals_t &super_training_medals)
    {
        pkmn::super_training_medals_t _super_training_medals = super_training_medals;
        _blockA->super_training_medals = _super_training_medals;
    }

    void pokemon_gen6impl::set_pokerus(const pkmn::pokerus_t &pokerus)
    {
        pkmn::pokerus_t _pokerus = pokerus;
        _blockA->pokerus = _pokerus;
    }

    /*
     * Getting Trainer Info
     */

    pkmn::pkstring pokemon_gen6impl::get_nickname() const
    {
        return conversions::import_modern_text(_blockB->nickname, 12);
    }

    pkmn::pkstring pokemon_gen6impl::get_trainer_name() const
    {
        return conversions::import_modern_text(_blockD->otname, 7);
    }

    pkmn::pkstring pokemon_gen6impl::get_trainer_gender() const
    {
        return (_blockD->metlevel_otgender  &(1<<31)) ? "Female" : "Male";
    }

    uint32_t pokemon_gen6impl::get_trainer_id() const
    {
        return _blockA->ot_id;
    }

    uint16_t pokemon_gen6impl::get_trainer_public_id() const
    {
        return _blockA->ot_pid;
    }

    uint16_t pokemon_gen6impl::get_trainer_secret_id() const
    {
        return _blockA->ot_sid;
    }

    pkmn::pkstring pokemon_gen6impl::get_ball() const
    {
        return database::get_ball_name(_blockD->ball);
    }

    pkmn::pkstring pokemon_gen6impl::get_original_game() const
    {
        return database::get_version_name(get_original_game_id());
    }

    uint8_t pokemon_gen6impl::get_met_level() const
    {
        return _blockD->metlevel_otgender  &~(1<<7);
    }

    /*
     * Setting Trainer Info
     */

    void pokemon_gen6impl::set_nickname(const pkmn::pkstring &nickname)
    {
        if(nickname.length() > 12)
            throw std::runtime_error("Nicknames can have a maximum of 10 characters in Generation VI.");

        conversions::export_modern_text(nickname, _blockB->nickname, 12);

        if(nickname == _pokedex_entry.species_name)
            _blockB->iv_isegg_isnicknamed &= ~(1<<31);
        else
            _blockB->iv_isegg_isnicknamed |= (1<<31);
    }

    void pokemon_gen6impl::set_trainer_name(const pkmn::pkstring &trainer_name)
    {
        if(trainer_name.length() > 7)
            throw std::runtime_error("Trainer names can have a maximum of 7 characters in Generation IV-V.");

        conversions::export_modern_text(trainer_name, _blockD->otname, 7);
    }

    void pokemon_gen6impl::set_trainer_gender(const pkmn::pkstring &gender)
    {
        if(gender.std_string() != "Male" and gender.std_string() != "Female")
            throw std::runtime_error("Gender must be male or female.");

        if(gender == "Male")
            _blockD->metlevel_otgender &= ~(1<<31);
        else
            _blockD->metlevel_otgender |= (1<<31);
    }

    void pokemon_gen6impl::set_trainer_id(uint32_t id)
    {
        _blockA->ot_id = id;
    }

    void pokemon_gen6impl::set_trainer_public_id(uint16_t id)
    {
        _blockA->ot_pid = id;
    }

    void pokemon_gen6impl::set_trainer_secret_id(uint16_t id)
    {
        _blockA->ot_sid = id;
    }

    void pokemon_gen6impl::set_ball(const pkmn::pkstring &ball)
    {
        _blockD->ball = database::get_ball_id(ball);
    }

    void pokemon_gen6impl::set_original_game(const pkmn::pkstring &game)
    {
        uint8_t generation = database::get_generation(game);
        if(generation < 3 or generation > get_generation())
            throw std::runtime_error("Invalid game."); // TODO: more detail

        _blockD->ot_game = database::get_version_game_index(game);
    }

    // Met level of 0 means Pokemon was hatched
    void pokemon_gen6impl::set_met_level(uint8_t level)
    {
        if(level > 100)
            throw std::runtime_error("Level must be 0-100.");

        _blockD->metlevel_otgender &= (1<<7);
        _blockD->metlevel_otgender |= (level  &~(1<<7));
    }

    /*
     * Getting Individual Stat Info
     */

    uint32_t pokemon_gen6impl::get_personality() const
    {
        return _blockA->personality;
    }

    uint8_t pokemon_gen6impl::get_friendship() const
    {
        return _blockC->not_ot_friendship;
    }

    uint8_t pokemon_gen6impl::get_level() const
    {
        return _raw.level;
    }

    uint32_t pokemon_gen6impl::get_experience() const
    {
        return _blockA->exp;
    }

    pkmn::pkstring pokemon_gen6impl::get_gender() const
    {
        if(_blockA->form_encounterinfo  &(1<<1))      return "Female";
        else if(_blockA->form_encounterinfo  &(1<<2)) return "Genderless";
        else                                          return "Male";
    }

    pkmn::nature_t pokemon_gen6impl::get_nature() const
    {
        return pkmn::nature_t(_blockA->nature);
    }

    pkmn::pkstring pokemon_gen6impl::get_ability() const
    {
        return database::get_ability_name(_blockA->ability);
    }

    pkmn::pkstring pokemon_gen6impl::get_form() const
    {
        return database::get_form_name(_form_id);
    }

    bool pokemon_gen6impl::is_shiny() const
    {
        return calculations::get_modern_shiny(_blockA->personality,
                                              _blockA->ot_sid,
                                              _blockA->ot_pid);
    }

    pkmn::dict<pkmn::pkstring, uint16_t> pokemon_gen6impl::get_stats() const
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

    pkmn::dict<pkmn::pkstring, uint16_t> pokemon_gen6impl::get_EVs() const
    {
        pkmn::dict<pkmn::pkstring, uint16_t> EVs;
        EVs["HP"]              = _blockA->ev_hp;
        EVs["Attack"]          = _blockA->ev_atk;
        EVs["Defense"]         = _blockA->ev_def;
        EVs["Speed"]           = _blockA->ev_spd;
        EVs["Special Attack"]  = _blockA->ev_spatk;
        EVs["Special Defense"] = _blockA->ev_spdef;

        return EVs;
    }

    pkmn::dict<pkmn::pkstring, uint16_t> pokemon_gen6impl::get_IVs() const
    {
        pkmn::dict<pkmn::pkstring, uint16_t> IVs;
        IVs["HP"]              = conversions::get_modern_IV(Stats::HP,      _blockB->iv_isegg_isnicknamed);
        IVs["Attack"]          = conversions::get_modern_IV(Stats::ATTACK,  _blockB->iv_isegg_isnicknamed);
        IVs["Defense"]         = conversions::get_modern_IV(Stats::DEFENSE, _blockB->iv_isegg_isnicknamed);
        IVs["Speed"]           = conversions::get_modern_IV(Stats::SPEED,   _blockB->iv_isegg_isnicknamed);
        IVs["Special Attack"]  = conversions::get_modern_IV(Stats::SPECIAL, _blockB->iv_isegg_isnicknamed);
        IVs["Special Defense"] = conversions::get_modern_IV(Stats::SPECIAL, _blockB->iv_isegg_isnicknamed);

        return IVs;
    }

    /*
     * Getting Individual Stat Info
     */

    // NOTE: this affects many things
    void pokemon_gen6impl::set_personality(uint32_t personality)
    {
        _blockA->personality = personality;
    }

    void pokemon_gen6impl::set_friendship(uint8_t friendship)
    {
        _blockC->not_ot_friendship = friendship;
    }

    // NOTE: this changes experience and stats
    void pokemon_gen6impl::set_level(uint8_t level)
    {
        if(level < 1 or level > 100)
            throw std::runtime_error("Level must be 1-100.");

        _set_level(level);
        _set_stats();
    }

    // NOTE: this affects level and stats
    void pokemon_gen6impl::set_experience(uint32_t experience)
    {
        _set_experience(experience);
        _set_stats();
    }

    void pokemon_gen6impl::set_gender(const pkmn::pkstring &gender)
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
            if(gender.std_string() == "Male")
                _blockA->form_encounterinfo &= ~(1<<1);
            else
                _blockA->form_encounterinfo |= (1<<1);
        }
    }

    // NOTE: this affects many things in Generation IV
    void pokemon_gen6impl::set_nature(const pkmn::pkstring &nature_name)
    {
        _blockA->nature = database::get_nature_id(nature_name);
    }

    void pokemon_gen6impl::set_ability(const pkmn::pkstring &ability)
    {
        std::ostringstream query_stream;
        query_stream << "SELECT generation_id FROM abilities WHERE id="
                     << database::get_ability_id(ability);
        if(uint8_t(_db->execAndGet(query_stream.str().c_str())) > get_generation())
            throw std::runtime_error("Invalid generation.");

        _blockA->ability = database::get_ability_id(ability);
        // TODO: can we get away with not setting ability_num?
    }

    void pokemon_gen6impl::set_form(const pkmn::pkstring &form)
    {
        uint16_t form_id    = database::get_form_id(_pokedex_entry.species_name, form);
        uint8_t  form_index = database::get_form_game_index(form_id);

        switch(_species_id)
        {
            // All of these can change with no restrictions or side effects
            case Species::UNOWN:
            case Species::CASTFORM:
            case Species::DEOXYS:
            case Species::BURMY:
            case Species::WORMADAM:
            case Species::SHELLOS:
            case Species::GASTRODON:
            case Species::BASCULIN:
            case Species::DEERLING:
            case Species::SAWSBUCK:
            case Species::MELOETTA:
                _blockA->form_encounterinfo &= ~0xF;
                _blockA->form_encounterinfo |= (form_index << 3);
                break;

            case Species::ROTOM:
            case Species::SHAYMIN:
                _blockA->form_encounterinfo &= ~0xF;
                _blockA->form_encounterinfo |= (form_index << 3);
                break;

            case Species::GIRATINA:
                _blockA->form_encounterinfo &= ~0xF;
                _blockA->form_encounterinfo |= (form_index << 3);
                set_held_item(form_index == 0 ? "None" : "Griseous Orb");
                break;

            case Species::DARMANITAN:
                _blockA->form_encounterinfo &= ~0xF;
                _blockA->form_encounterinfo |= (form_index << 3);
                set_ability("Zen Mode");
                break;

            case Species::TORNADUS:
            case Species::THUNDURUS:
            case Species::LANDORUS:
            case Species::KELDEO:
                _blockA->form_encounterinfo &= ~0xF;
                _blockA->form_encounterinfo |= (form_index << 3);
                break;

            case Species::KYUREM:
                _blockA->form_encounterinfo &= ~0xF;
                _blockA->form_encounterinfo |= (form_index << 3);
                set_held_item(form_index == 0 ? "None" : "DNA Splicers");
                break;

            case Species::GENESECT:
                _blockA->form_encounterinfo &= ~0xF;
                _blockA->form_encounterinfo |= (form_index << 3);
                set_held_item(conversions::genesect_form_items[form_id]);
                break;

            // TODO: Generation VI forms
        }

        _form_id       = form_id;
        _pokedex_entry = _pokedex->get_pokemon_entry(_species_id, _form_id);
    }

    /*
     * NOTE: this affects personality and probably trainer ID
     *
     * Source: http://www.smogon.com/ingame/rng/pid_iv_creation#how_shiny
     */
    void pokemon_gen6impl::set_shiny(bool value)
    {
        if(value)
        {
            if(not is_shiny())
            {
                _blockA->ot_id = pkmn::trainer::LIBPKMN_TRAINER_ID;
                _blockA->personality = pkmn::trainer::LIBPKMN_TRAINER_ID;
            }
        }
        else
        {
            while(is_shiny())
                _blockA->personality = _prng->lcrng();
        }
    }

    // NOTE: this affects stats
    void pokemon_gen6impl::set_EV(const pkmn::pkstring &stat, uint16_t value)
    {
        if(stat == "Special")
            throw std::runtime_error("The Special value is only in Generations I-II.");
        if(value > 255)
            throw std::runtime_error("EV's have a maximum value of 255 in Generations IV-V.");

        uint16_t sum_of_rest;

        switch(database::get_stat_id(stat)) // Will throw if stat is invalid
        {
            case Stats::HP:
                sum_of_rest = _blockA->ev_atk + _blockA->ev_def + _blockA->ev_spd + _blockA->ev_spatk
                            + _blockA->ev_spdef;
                if((sum_of_rest + value) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                             % (510 - sum_of_rest)));

                _blockA->ev_hp = value;
                break;

            case Stats::ATTACK:
                sum_of_rest = _blockA->ev_hp + _blockA->ev_def + _blockA->ev_spd + _blockA->ev_spatk
                            + _blockA->ev_spdef;
                if((sum_of_rest + value) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                                 % (510 - sum_of_rest)));

                _blockA->ev_atk = value;
                break;

            case Stats::DEFENSE:
                sum_of_rest = _blockA->ev_hp + _blockA->ev_atk + _blockA->ev_spd + _blockA->ev_spatk
                            + _blockA->ev_spdef;
                if((sum_of_rest + value) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                                 % (510 - sum_of_rest)));

                _blockA->ev_def = value;
                break;

            case Stats::SPEED:
                sum_of_rest = _blockA->ev_hp + _blockA->ev_atk + _blockA->ev_def + _blockA->ev_spatk
                            + _blockA->ev_spdef;
                if((sum_of_rest + value) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                                 % (510 - sum_of_rest)));

                _blockA->ev_spd = value;
                break;

            case Stats::SPECIAL_ATTACK:
                sum_of_rest = _blockA->ev_hp + _blockA->ev_atk + _blockA->ev_def + _blockA->ev_spd
                            + _blockA->ev_spdef;
                if((sum_of_rest + value) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                                 % (510 - sum_of_rest)));

                _blockA->ev_spatk = value;
                break;

            default: // Stats::SPECIAL_DEFENSE
                sum_of_rest = _blockA->ev_hp + _blockA->ev_atk + _blockA->ev_def + _blockA->ev_spd
                            + _blockA->ev_spatk;
                if((sum_of_rest + value) > 510)
                    throw std::runtime_error(str(boost::format("The maximum possible value to set is %d")
                                                 % (510 - sum_of_rest)));

                _blockA->ev_spdef = value;
                break;
        }

        _set_stats();
    }

    // NOTE: this affects stats
    void pokemon_gen6impl::set_IV(const pkmn::pkstring &stat, uint16_t value)
    {
        if(stat == "Special")
            throw std::runtime_error("The Special value is only in Generations I-II.");
        if(value > 31)
            throw std::runtime_error("IV's have a maximum value of 31 in Generations IV-V.");

        // Will throw if stat is invalid
        conversions::set_modern_IV(database::get_stat_id(stat), _blockB->iv_isegg_isnicknamed, value);

        _set_stats();
    }

    /*
     * Battle Stat Info
     */

    pkmn::pkstring pokemon_gen6impl::get_status() const
    {
        return conversions::modern_statuses.at(_raw.status, "OK");
    }

    pkmn::item_entry_t pokemon_gen6impl::get_held_item() const
    {
        try
        {
            return _pokedex->get_item_entry(database::get_item_id(_blockA->held_item, _version_id));
        }
        catch(...)
        {
            return _pokedex->get_item_entry(Items::INVALID);
        }
    }

    void pokemon_gen6impl::set_status(const pkmn::pkstring &status)
    {
        if(not conversions::reverse_retro_statuses.has_key(status))
            throw std::runtime_error("Invalid status given.");

        _raw.status = conversions::reverse_retro_statuses.at(status);
    }

    void pokemon_gen6impl::set_held_item(const pkmn::pkstring &item_name)
    {
        _blockA->held_item = database::get_item_game_index(item_name,
                                                           database::get_version_name(_version_id));
    }

    /*
     * Getting Move Info
     */

    pkmn::move_entry_t pokemon_gen6impl::get_move(uint8_t pos) const
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        try
        {
            return _pokedex->get_move_entry(_blockB->moves[pos-1]);
        }
        catch(...)
        {
            return _pokedex->get_move_entry(Moves::INVALID);
        }
    }

    void pokemon_gen6impl::get_moves(pkmn::moveset_t &moves) const
    {
        moves.clear();
        for(size_t i = 0; i < 4; i++) moves.push_back(get_move(i+1));
    }

    uint8_t pokemon_gen6impl::get_move_PP(uint8_t pos) const
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        return _blockB->move_pps[pos-1];
    }

    void pokemon_gen6impl::get_move_PPs(std::vector<uint8_t> &move_PPs) const
    {
        move_PPs.clear();
        for(size_t i = 0; i < 4; i++) move_PPs.push_back(_blockB->move_pps[i+1]); 
    }

    /*
     * Setting Move Info
     */

    void pokemon_gen6impl::set_move(const pkmn::pkstring &move_name, uint8_t pos)
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

    void pokemon_gen6impl::set_move_PP(uint8_t PP, uint8_t pos)
    {   
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        if(PP <= database::get_move_pp(_blockB->moves[pos-1])) _blockB->move_pps[pos-1] = PP; 
        else throw std::runtime_error("This move PP is invalid.");
    }

    /*
     * Database Info
     */

    uint16_t pokemon_gen6impl::get_original_game_id() const
    {
        return database::get_version_id(_blockD->ot_game);
    }

    uint16_t pokemon_gen6impl::get_ability_id() const
    {
        return _blockA->ability;
    }

    uint16_t pokemon_gen6impl::get_item_id() const
    {
        return database::get_item_id(_blockA->held_item, _version_id);
    }

    uint16_t pokemon_gen6impl::get_nature_id() const
    {
        return _blockA->nature;
    }

    const void* pokemon_gen6impl::get_native()
    {
        return &_raw;
    }

    void pokemon_gen6impl::_set_experience(const uint32_t exp)
    {
        _blockA->exp = exp;
        _raw.level = database::get_level(_species_id, exp);
    }

    void pokemon_gen6impl::_set_level(const uint8_t level)
    {
        _raw.level = level;
        _blockA->exp = database::get_experience(_species_id, level);
    }

    void pokemon_gen6impl::_set_stats()
    {
        pkmn::dict<pkmn::pkstring, uint16_t> stats = _pokedex_entry.base_stats;
        pkmn::dict<pkmn::pkstring, uint16_t> IVs = get_IVs();
        pkmn::nature_t nature = get_nature();

        _raw.max_hp = calculations::get_modern_stat("HP", stats["HP"], _raw.level,
                                                    nature, _blockA->ev_hp, IVs["HP"]);
        _raw.current_hp = _raw.max_hp;
        _raw.atk = calculations::get_modern_stat("Attack", stats["Attack"], _raw.level,
                                                 nature, _blockA->ev_atk, IVs["Attack"]);
        _raw.def = calculations::get_modern_stat("Defense", stats["Defense"], _raw.level,
                                                 nature, _blockA->ev_def, IVs["Defense"]);
        _raw.spd = calculations::get_modern_stat("Speed", stats["Speed"], _raw.level,
                                                 nature, _blockA->ev_spd, IVs["Speed"]);
        _raw.spatk = calculations::get_modern_stat("Special Attack", stats["Special Attack"],
                                                   _raw.level, nature, _blockA->ev_spatk,
                                                   IVs["Special Attack"]);
        _raw.spdef = calculations::get_modern_stat("Special Defense", stats["Special Defense"],
                                                   _raw.level, nature, _blockA->ev_spdef,
                                                   IVs["Special Defense"]);
    }

    // Source: http://www.smogon.com/ingame/rng/pid_iv_creation#finding_gender_from_pid
    void pokemon_gen6impl::_set_default_gender()
    {
        _blockA->form_encounterinfo &= ~(3<<1); // Clear gender flags

        float chance_male   = _pokedex_entry.chance_male;
        float chance_female = _pokedex_entry.chance_female;

        if(chance_male + chance_female == 0.0) _blockA->form_encounterinfo |= (1<<2);
        else if(chance_male == 1.0) return;
        else if(chance_female == 1.0) _blockA->form_encounterinfo |= (1<<1);
        else
        {
            /*
             * Gender is determined by (personality  &0xFF).
             * If the personality is below a certain number, the Pokemon will be female.
             */
            uint8_t truncated_pid = (_blockA->personality & 0xFF);
            if((chance_female == 0.125 and truncated_pid <= 30) or
               (chance_female == 0.25 and truncated_pid <= 63) or
               (chance_female == 0.5 and truncated_pid <= 126) or
               (truncated_pid <= 190))
            {
                _blockA->form_encounterinfo |= (1<<1);
            }
        }
    }

}
