/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
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
#include "pokemon_ndsimpl.hpp"
#include "conversions/utils.hpp"

namespace pkmn
{
    pokemon_ndsimpl::pokemon_ndsimpl(uint16_t species, uint16_t version,
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
            memset(&_raw, 0x0, sizeof(pkmn::native::nds_party_pokemon_t));
        }
        else
        {
            _raw.pc.personality = _prng->lcrng();
            _blockA->species = database::get_pokemon_game_index(_species_id, _version_id);
            _blockA->held_item = Items::NONE;
            _blockA->ot_id = _prng->lcrng();
            // Experience set by level
            _blockA->friendship = 70;
            _blockA->ability = (_raw.pc.personality % 2) ?
                                    database::get_ability_id(_pokedex_entry.abilities.second)
                                  : database::get_ability_id(_pokedex_entry.abilities.first);
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

            if(_version_id == Versions::PLATINUM)
            {
                _blockB->eggmet_plat = 2008; // Distant land
                _blockB->met_plat = 2008; // Distant land
            }
            _blockC->hometown = database::get_version_game_index(_version_id);
            // TODO: check for expected values in "unknown" fields
            // TODO: met dates
            if(_version_id == Versions::DIAMOND or _version_id == Versions::PEARL)
            {
                _blockD->eggmet_dp = 2008; // Distant land
                _blockD->met_dp = 2008; // Distant land
            }
            // TODO: Pokerus
            if(_version_id != Versions::HEARTGOLD and _version_id != Versions::SOULSILVER)
                _blockD->ball = Balls::LUXURY_BALL;
            _blockD->metlevel_otgender = (level | ~(1<<31));
            _blockD->encounter_info = 0; // Special event
            if(_version_id == Versions::HEARTGOLD or _version_id == Versions::SOULSILVER)
                _blockD->ball_hgss = Balls::LUXURY_BALL;

            _set_level(level);
            _set_stats();
        }

        if(get_generation() == 4)
        {
            conversions::export_gen4_text(PKSTRING_UPPERCASE(_pokedex_entry.species_name),
                                          _blockC->nickname, 10);
            conversions::export_gen4_text("LIBPKMN",
                                          _blockD->otname, 7);
        }
        else
        {
            conversions::export_modern_text(_pokedex_entry.species_name,
                                            _blockC->nickname, 10);
            conversions::export_modern_text("LibPKMN",
                                            _blockD->otname, 7);
        }
    }

    pokemon_ndsimpl::pokemon_ndsimpl(const pkmn::native::nds_pc_pokemon_t& raw,
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
            uint16_t pokemon_id = database::get_pokemon_id(_blockA->species, Versions::HEARTGOLD);
            _invalid = false;
        }
        catch(const std::exception& e)
        {
            _invalid = true;
        }

        _set_stats();
    }

    pokemon_ndsimpl::pokemon_ndsimpl(const pkmn::native::nds_party_pokemon_t& raw,
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
            uint16_t pokemon_id = database::get_pokemon_id(_blockA->species, Versions::HEARTGOLD);
            _invalid = false;
        }
        catch(const std::exception& e)
        {
            _invalid = true;
        }

        _set_stats();
    }

    pokemon_ndsimpl::pokemon_ndsimpl(const pokemon_ndsimpl& other):
        pokemon_impl(other),
        _raw(other._raw)
    {
        _blockA = &(_raw.pc.blocks.blockA);
        _blockB = &(_raw.pc.blocks.blockB);
        _blockC = &(_raw.pc.blocks.blockC);
        _blockD = &(_raw.pc.blocks.blockD);
        // TODO: set form
    }

    pokemon_ndsimpl& pokemon_ndsimpl::operator=(const pokemon_ndsimpl& other)
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

    pkmn::contest_stats_t pokemon_ndsimpl::get_contest_stats() const
    {
        return _blockA->contest_stats;
    }

    pkmn::markings_t pokemon_ndsimpl::get_markings() const
    {
        return _blockA->markings;
    }

    pkmn::ribbons_t pokemon_ndsimpl::get_ribbons() const
    {
        pkmn::ribbons_t ribbons;
        ribbons.hoenn   = _blockB->hoenn_ribbons;
        ribbons.sinnoh.ribbons1 = _blockA->sinnoh_ribbons1;
        if(get_generation() == 4) ribbons.sinnoh.ribbons2 = _blockA->sinnoh_ribbons2;
        else                      ribbons.unova   = _blockA->unova_ribbons;
        ribbons.sinnoh.ribbons3 = _blockC->sinnoh_ribbons3;

        return ribbons;
    }

    // No Super Training in Generation IV-V
    pkmn::super_training_medals_t pokemon_ndsimpl::get_super_training_medals() const
    {
        return pkmn::super_training_medals_t();
    }

    /*
     * Setting Non-Battle Info
     */

    void pokemon_ndsimpl::set_contest_stats(const pkmn::contest_stats_t& contest_stats)
    {
        _blockA->contest_stats = contest_stats;
    }

    void pokemon_ndsimpl::set_markings(const pkmn::markings_t& markings)
    {
        pkmn::markings_t _markings = markings;
        _blockA->markings = _markings;
    }

    void pokemon_ndsimpl::set_ribbons(const pkmn::ribbons_t& ribbons)
    {
        pkmn::ribbons_t _ribbons = ribbons;
        _blockB->hoenn_ribbons   = _ribbons.hoenn;
        _blockA->sinnoh_ribbons1 = _ribbons.sinnoh.ribbons1;
        if(get_generation() == 4) _blockA->sinnoh_ribbons2 = _ribbons.sinnoh.ribbons2;
        else                      _blockA->unova_ribbons   = _ribbons.unova;
        _blockC->sinnoh_ribbons3 = _ribbons.sinnoh.ribbons3;
    }

    // No Super Training in Generation IV-V
    void pokemon_ndsimpl::set_super_training_medals(const pkmn::super_training_medals_t &super_training_medals)
    {
        /* NOP */
    }

    /*
     * Getting Trainer Info
     */

    pkmn::pkstring pokemon_ndsimpl::get_nickname() const
    {
        return (get_generation() == 4) ? conversions::import_gen4_text(_blockC->nickname, 10)
                                       : conversions::import_modern_text(_blockC->nickname, 10);
    }

    pkmn::pkstring pokemon_ndsimpl::get_trainer_name() const
    {
        return (get_generation() == 4) ? conversions::import_gen4_text(_blockD->otname, 7)
                                       : conversions::import_modern_text(_blockD->otname, 7);
    }

    pkmn::pkstring pokemon_ndsimpl::get_trainer_gender() const
    {
        return (_blockD->metlevel_otgender&  (1<<31)) ? "Female" : "Male";
    }

    uint32_t pokemon_ndsimpl::get_trainer_id() const
    {
        return _blockA->ot_id;
    }

    uint16_t pokemon_ndsimpl::get_trainer_public_id() const
    {
        return _blockA->ot_pid;
    }

    uint16_t pokemon_ndsimpl::get_trainer_secret_id() const
    {
        return _blockA->ot_sid;
    }

    pkmn::pkstring pokemon_ndsimpl::get_ball() const
    {
        if(_version_id == Versions::HEARTGOLD or _version_id == Versions::SOULSILVER)
            return database::get_ball_name(_blockD->ball_hgss);
        else return database::get_ball_name(_blockD->ball);
    }

    pkmn::pkstring pokemon_ndsimpl::get_original_game() const
    {
        return database::get_version_name(get_original_game_id());
    }

    uint8_t pokemon_ndsimpl::get_met_level() const
    {
        return _blockD->metlevel_otgender&  ~(1<<7);
    }

    /*
     * Setting Trainer Info
     */

    void pokemon_ndsimpl::set_nickname(const pkmn::pkstring& nickname)
    {
        if(nickname.length() > 10)
            throw std::runtime_error("Nicknames can have a maximum of 10 characters in Generation IV-V.");

        if(get_generation() == 4)
            conversions::export_gen4_text(nickname, _blockC->nickname, 10);
        else
            conversions::export_modern_text(nickname, _blockC->nickname, 10);

        if(nickname == _pokedex_entry.species_name)
            _blockB->iv_isegg_isnicknamed &= ~(1<<31);
        else
            _blockB->iv_isegg_isnicknamed |= (1<<31);
    }

    void pokemon_ndsimpl::set_trainer_name(const pkmn::pkstring& trainer_name)
    {
        if(trainer_name.length() > 7)
            throw std::runtime_error("Trainer names can have a maximum of 7 characters in Generation IV-V.");

        if(get_generation() == 4)
            conversions::export_gen4_text(trainer_name, _blockD->otname, 7);
        else
            conversions::export_modern_text(trainer_name, _blockD->otname, 7);
    }

    void pokemon_ndsimpl::set_trainer_gender(const pkmn::pkstring& gender)
    {
        if(gender.std_string() != "Male" and gender.std_string() != "Female")
            throw std::runtime_error("Gender must be male or female.");

        if(gender == "Male")
            _blockD->metlevel_otgender &= ~(1<<31);
        else
            _blockD->metlevel_otgender |= (1<<31);
    }

    void pokemon_ndsimpl::set_trainer_id(uint32_t id)
    {
        _blockA->ot_id = id;
    }

    void pokemon_ndsimpl::set_trainer_public_id(uint16_t id)
    {
        _blockA->ot_pid = id;
    }

    void pokemon_ndsimpl::set_trainer_secret_id(uint16_t id)
    {
        _blockA->ot_sid = id;
    }

    void pokemon_ndsimpl::set_ball(const pkmn::pkstring& ball)
    {
        // TODO: generation check
        if(_version_id == Versions::HEARTGOLD or _version_id == Versions::SOULSILVER)
            _blockD->ball_hgss = database::get_ball_id(ball);
        else _blockD->ball = database::get_ball_id(ball);
    }

    void pokemon_ndsimpl::set_original_game(const pkmn::pkstring& game)
    {
        uint8_t generation = database::get_generation(game);
        if(generation < 3 or generation > get_generation())
            throw std::runtime_error("Invalid game."); // TODO: more detail

        _blockC->hometown = database::get_version_game_index(game);
    }

    // Met level of 0 means Pokemon was hatched
    void pokemon_ndsimpl::set_met_level(uint8_t level)
    {
        if(level > 100)
            throw std::runtime_error("Level must be 0-100.");

        _blockD->metlevel_otgender &= (1<<7);
        _blockD->metlevel_otgender |= (level&  ~(1<<7));
    }

    /*
     * Getting Individual Stat Info
     */

    uint32_t pokemon_ndsimpl::get_personality() const
    {
        return _raw.pc.personality;
    }

    uint8_t pokemon_ndsimpl::get_friendship() const
    {
        return _blockA->friendship;
    }

    uint8_t pokemon_ndsimpl::get_level() const
    {
        return _raw.level;
    }

    uint32_t pokemon_ndsimpl::get_experience() const
    {
        return _blockA->exp;
    }

    pkmn::pkstring pokemon_ndsimpl::get_gender() const
    {
        if(_blockB->form_encounterinfo & (1<<1))      return "Female";
        else if(_blockB->form_encounterinfo & (1<<2)) return "Genderless";
        else                                          return "Male";
    }

    pkmn::nature_t pokemon_ndsimpl::get_nature() const
    {
        if(get_generation() == 5) return pkmn::nature_t(_blockB->nature);
        else return pkmn::nature_t(_raw.pc.personality % 24);
    }

    pkmn::pkstring pokemon_ndsimpl::get_ability() const
    {
        return database::get_ability_name(_blockA->ability);
    }

    pkmn::pkstring pokemon_ndsimpl::get_form() const
    {
        return database::get_form_name(_form_id);
    }

    bool pokemon_ndsimpl::is_shiny() const
    {
        return calculations::get_modern_shiny(_raw.pc.personality,
                                              _blockA->ot_sid,
                                              _blockA->ot_pid);
    }

    pkmn::dict<pkmn::pkstring, uint16_t> pokemon_ndsimpl::get_stats() const
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

    pkmn::dict<pkmn::pkstring, uint16_t> pokemon_ndsimpl::get_EVs() const
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

    pkmn::dict<pkmn::pkstring, uint16_t> pokemon_ndsimpl::get_IVs() const
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
    void pokemon_ndsimpl::set_personality(uint32_t personality)
    {
        _raw.pc.personality = personality;
    }

    void pokemon_ndsimpl::set_friendship(uint8_t friendship)
    {
        _blockA->friendship = friendship;
    }

    // NOTE: this changes experience and stats
    void pokemon_ndsimpl::set_level(uint8_t level)
    {
        if(level < 1 or level > 100)
            throw std::runtime_error("Level must be 1-100.");

        _set_level(level);
        _set_stats();
    }

    // NOTE: this affects level and stats
    void pokemon_ndsimpl::set_experience(uint32_t experience)
    {
        _set_experience(experience);
        _set_stats();
    }

    void pokemon_ndsimpl::set_gender(const pkmn::pkstring& gender)
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
                _blockB->form_encounterinfo &= ~(1<<1);
            else
                _blockB->form_encounterinfo |= (1<<1);
        }
    }

    // NOTE: this affects many things in Generation IV
    void pokemon_ndsimpl::set_nature(const pkmn::pkstring& nature_name)
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

    void pokemon_ndsimpl::set_ability(const pkmn::pkstring& ability)
    {
        std::ostringstream query_stream;
        query_stream << "SELECT generation_id FROM abilities WHERE id="
                     << database::get_ability_id(ability);
        if(uint8_t(_db->execAndGet(query_stream.str().c_str())) > get_generation())
            throw std::runtime_error("Invalid generation.");

        // TODO: check generation
        _blockA->ability = database::get_ability_id(ability);
        if(ability == _pokedex_entry.hidden_ability) _blockB->gen5_info |= 1;
    }

    void pokemon_ndsimpl::set_form(const pkmn::pkstring& form)
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
                _blockB->form_encounterinfo &= ~0xF;
                _blockB->form_encounterinfo |= (form_index << 3);
                break;

            case Species::PICHU:
                if(_version_id == Versions::HEARTGOLD or _version_id == Versions::SOULSILVER)
                {
                    _blockB->form_encounterinfo &= ~0xF;
                    _blockB->form_encounterinfo |= (form_index << 3);
                }
                else throw std::runtime_error("Pichu can only change form in HeartGold/SoulSilver.");
                break;

            case Species::ROTOM:
            case Species::SHAYMIN:
                if(_version_id != Versions::DIAMOND and _version_id != Versions::PEARL)
                {
                    _blockB->form_encounterinfo &= ~0xF;
                    _blockB->form_encounterinfo |= (form_index << 3);
                }
                else throw std::runtime_error(str(boost::format("%s cannot change forms in Diamond/Pearl.")
                                                  % _pokedex_entry.species_name));
                break;

            case Species::GIRATINA:
                if(_version_id != Versions::DIAMOND and _version_id != Versions::PEARL)
                {
                    _blockB->form_encounterinfo &= ~0xF;
                    _blockB->form_encounterinfo |= (form_index << 3);
                    set_held_item(form_index == 0 ? "None" : "Griseous Orb");
                }
                else throw std::runtime_error("Giratina cannot change forms in Diamond/Pearl.");
                break;

            case Species::DARMANITAN:
                _blockB->form_encounterinfo &= ~0xF;
                _blockB->form_encounterinfo |= (form_index << 3);
                set_ability("Zen Mode");
                break;

            case Species::TORNADUS:
            case Species::THUNDURUS:
            case Species::LANDORUS:
            case Species::KELDEO:
                if(_version_id != Versions::BLACK and _version_id != Versions::WHITE)
                {
                    _blockB->form_encounterinfo &= ~0xF;
                    _blockB->form_encounterinfo |= (form_index << 3);
                }
                else throw std::runtime_error(str(boost::format("%s cannot change forms in Black/White.")
                                                  % _pokedex_entry.species_name));
                break;

            case Species::KYUREM:
                if(_version_id != Versions::BLACK and _version_id != Versions::WHITE)
                {
                    _blockB->form_encounterinfo &= ~0xF;
                    _blockB->form_encounterinfo |= (form_index << 3);
                    set_held_item(form_index == 0 ? "None" : "DNA Splicers");
                }
                else throw std::runtime_error("Kyurem cannot change forms in Black/White.");
                break;

            case Species::GENESECT:
                _blockB->form_encounterinfo &= ~0xF;
                _blockB->form_encounterinfo |= (form_index << 3);
                set_held_item(conversions::genesect_form_items[form_id]);
                break;
        }

        _form_id       = form_id;
        _pokedex_entry = _pokedex->get_pokemon_entry(_species_id, _form_id);
    }

    /*
     * NOTE: this affects personality and probably trainer ID
     *
     * Source: http://www.smogon.com/ingame/rng/pid_iv_creation#how_shiny
     */
    void pokemon_ndsimpl::set_shiny(bool value)
    {
        uint8_t num1_tid = count_ones((_blockA->ot_pid >> 3));
        if(num1_tid == 1 or num1_tid == 3)
        {
            if(_blockA->ot_pid & (1<<15))
                _blockA->ot_pid &= ~(1<<15);
            else
                _blockA->ot_pid |= (1<<15);
        }

        uint8_t num1_sid = count_ones((_blockA->ot_sid >> 3));
        if(num1_sid == 1 or num1_sid == 3)
        {
            if(_blockA->ot_sid & (1<<15))
                _blockA->ot_sid &= ~(1<<15);
            else
                _blockA->ot_sid |= (1<<15);
        }

        uint8_t hid = (_raw.pc.personality & 0xFFFF0000) >> 16; 
        uint8_t num1_hid = count_ones(hid >> 3); 
        if(num1_hid == 1 or num1_hid == 3)
        {
            if(hid & (1<<15))
                hid &= ~(1<<15);
            else
                hid |= (1<<15);
        }

        uint8_t lid = (_raw.pc.personality & 0xFFFF);
        uint8_t num1_lid = count_ones(lid >> 3); 
        if(num1_lid == 1 or num1_lid == 3)
        {
            if(lid & (1<<15))
                lid &= ~(1<<15);
            else
                lid |= (1<<15);
        }

        _raw.pc.personality = ((hid << 16) | lid);
    }

    // NOTE: this affects stats
    void pokemon_ndsimpl::set_EV(const pkmn::pkstring& stat, uint16_t value)
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
    void pokemon_ndsimpl::set_IV(const pkmn::pkstring& stat, uint16_t value)
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

    pkmn::pkstring pokemon_ndsimpl::get_status() const
    {
        return conversions::modern_statuses.at(_raw.status, "OK");
    }

    pkmn::item_entry_t pokemon_ndsimpl::get_held_item() const
    {
        try
        {
            return _pokedex->get_item_entry(database::get_item_id(_blockA->held_item, _version_id));
        }
        catch(const std::exception& e)
        {
            return _pokedex->get_item_entry(Items::INVALID);
        }
    }

    void pokemon_ndsimpl::set_status(const pkmn::pkstring& status)
    {
        if(not conversions::reverse_retro_statuses.has_key(status))
            throw std::runtime_error("Invalid status given.");

        _raw.status = conversions::reverse_retro_statuses.at(status);
    }

    void pokemon_ndsimpl::set_held_item(const pkmn::pkstring& item_name)
    {
        _blockA->held_item = database::get_item_game_index(item_name,
                                                           database::get_version_name(_version_id));
    }

    /*
     * Getting Move Info
     */

    pkmn::move_entry_t pokemon_ndsimpl::get_move(uint8_t pos) const
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        try
        {
            return _pokedex->get_move_entry(_blockB->moves[pos-1]);
        }
        catch(const std::exception& e)
        {
            return _pokedex->get_move_entry(Moves::INVALID);
        }
    }

    void pokemon_ndsimpl::get_moves(pkmn::moveset_t& moves) const
    {
        moves.clear();
        for(size_t i = 0; i < 4; i++) moves.push_back(get_move(i+1));
    }

    uint8_t pokemon_ndsimpl::get_move_PP(uint8_t pos) const
    {
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        return _blockB->move_pps[pos-1];
    }

    void pokemon_ndsimpl::get_move_PPs(std::vector<uint8_t>& move_PPs) const
    {
        move_PPs.clear();
        for(size_t i = 0; i < 4; i++) move_PPs.push_back(_blockB->move_pps[i+1]); 
    }

    /*
     * Setting Move Info
     */

    void pokemon_ndsimpl::set_move(const pkmn::pkstring& move_name, uint8_t pos)
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

    void pokemon_ndsimpl::set_move_PP(uint8_t PP, uint8_t pos)
    {   
        if(pos == 0 or pos > 4)
            throw std::runtime_error("Move position must be 1-4.");

        if(PP <= database::get_move_pp(_blockB->moves[pos-1])) _blockB->move_pps[pos-1] = PP; 
        else throw std::runtime_error("This move PP is invalid.");
    }

    /*
     * Database Info
     */

    uint16_t pokemon_ndsimpl::get_original_game_id() const
    {
        return database::get_version_id(_blockC->hometown);
    }

    uint16_t pokemon_ndsimpl::get_ability_id() const
    {
        return _blockA->ability;
    }

    uint16_t pokemon_ndsimpl::get_item_id() const
    {
        return database::get_item_id(_blockA->held_item, _version_id);
    }

    uint16_t pokemon_ndsimpl::get_nature_id() const
    {
        if(get_generation() == 4) return (_raw.pc.personality % 24);
        else return _blockB->nature;
    }

    const void* pokemon_ndsimpl::get_native()
    {
        return& _raw;
    }

    void pokemon_ndsimpl::_set_experience(const uint32_t exp)
    {
        _blockA->exp = exp;
        _raw.level = database::get_level(_species_id, exp);
    }

    void pokemon_ndsimpl::_set_level(const uint8_t level)
    {
        _raw.level = level;
        _blockA->exp = database::get_experience(_species_id, level);
    }

    void pokemon_ndsimpl::_set_stats()
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
    void pokemon_ndsimpl::_set_default_gender()
    {
        _blockB->form_encounterinfo &= ~(3<<1); // Clear gender flags

        float chance_male   = _pokedex_entry.chance_male;
        float chance_female = _pokedex_entry.chance_female;

        if(chance_male + chance_female == 0.0) _blockB->form_encounterinfo |= (1<<2);
        else if(chance_male == 1.0) return;
        else if(chance_female == 1.0) _blockB->form_encounterinfo |= (1<<1);
        else
        {
            /*
             * Gender is determined by (personality & 0xFF).
             * If the personality is below a certain number, the Pokemon will be female.
             */
            uint8_t truncated_pid = (_raw.pc.personality & 0xFF);
            if((chance_female == 0.125 and truncated_pid <= 30) or
               (chance_female == 0.25 and truncated_pid <= 63) or
               (chance_female == 0.5 and truncated_pid <= 126) or
               (truncated_pid <= 190))
            {
                _blockB->form_encounterinfo |= (1<<1);
            }
        }
    }

}
