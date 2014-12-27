/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database.hpp>

#include "base_pokemon_gen2impl.hpp"

namespace fs = boost::filesystem;

namespace pkmn
{
    base_pokemon_gen2impl::base_pokemon_gen2impl(unsigned int id, unsigned int game):
        base_pokemon_impl(id, game)
    {
        // Get final part of images path
        switch(_game_id)
        {
            case Versions::GOLD:
                _images_game_string = "gold";
                break;
            case Versions::SILVER:
                _images_game_string = "silver";
                break;
            case Versions::CRYSTAL:
                _images_game_string = "crystal";
                break;
            default: // It should never get here
                _images_game_string = "crystal";
                break;
        }

        _sprite_dir = fs::path(_images_dir / "generation-2" / _images_game_string);
        _shiny_sprite_dir = _sprite_dir;

        switch(id)
        {
            case Species::NONE: // None, should only be used for empty slots at end of party
                SET_POKEBALL_IMAGE();
                break;

            case Species::INVALID: // Invalid, aka Missingno. equivalents
                SET_SUBSTITUTE_IMAGE();
                break;

            default:
                SET_IMAGES_PATHS(_images_default_basename);

                /*
                 * Even though most attributes are queried from the database when called, stats take a long time when
                 * doing a lot at once, so grab these upon instantiation
                 */
                std::ostringstream query_stream;
                query_stream << "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" << _pokemon_id
                             << " AND stat_id IN (3,5)";
                SQLite::Statement query(*_db, query_stream.str().c_str());
                query.executeStep();
                _special_attack = int(query.getColumn(0));
                query.executeStep();
                _special_defense = int(query.getColumn(0));
                _set_old_values();

                break;
        }

        _form_id = _species_id;
    }

    // No gender differences in Generation 2
    bool base_pokemon_gen2impl::has_gender_differences() const {return false;}

    // No abilities in Generation 2
    pkmn::pkstring_pair_t base_pokemon_gen2impl::get_abilities() const
    {
        pkmn::pkstring_pair_t abilities;
        abilities.first = "None";
        abilities.second = "None";

        return abilities;
    }

    pkmn::pkstring base_pokemon_gen2impl::get_hidden_ability() const {return "None";}

    pkmn::dict<pkmn::pkstring, unsigned int> base_pokemon_gen2impl::get_base_stats() const
    {
        pkmn::dict<pkmn::pkstring, unsigned int> stats;
        stats["HP"] = _hp;
        stats["Attack"] = _attack;
        stats["Defense"] = _defense;
        stats["Special Attack"] = _special_attack;
        stats["Special Defense"] = _special_defense;
        stats["Speed"] = _speed;

        return stats;
    }

    pkmn::dict<pkmn::pkstring, unsigned int> base_pokemon_gen2impl::get_ev_yields() const
    {
        pkmn::dict<pkmn::pkstring, unsigned int> stats = get_base_stats();
        pkmn::dict<pkmn::pkstring, unsigned int> ev_yields;
        ev_yields["HP"] = stats["HP"];
        ev_yields["Attack"] = stats["Attack"];
        ev_yields["Defense"] = stats["Defense"];
        ev_yields["Speed"] = stats["Speed"];

        /*
         * Special was a single stat in Generation 1, but it was separated into
         * Special Attack and Special Defense in Generation 2. For backwards
         * compatibility, Pokemon still have a Special EV, which just matches
         * Pokemon's Special Attack stat.
         */
        ev_yields["Special"] = stats["Special Attack"];

        return ev_yields;
    }

    // No forms in Generation 2 
    void base_pokemon_gen2impl::set_form(unsigned int form)
    {
        if(_species_id == Species::UNOWN)
        {
            if(form == Forms::Unown::A)
            {
                _form_id = form;
                set_form("A");
            }
            else if(form >= Forms::Unown::B and form <= Forms::Unown::Z)
            {
                _form_id = form;
                std::string letter;
                letter += (form - 9935); // Will become ASCII value for letter
                set_form(letter);
            }
            else throw std::runtime_error("Invalid form.");
        }
        else if(form != _species_id) throw std::runtime_error("Invalid form.");
    }

    void base_pokemon_gen2impl::set_form(const pkmn::pkstring &form)
    {
        if(_species_id == Species::UNOWN)
        {
            char letter = boost::algorithm::to_lower_copy(form.std_string())[0];
            if(letter >= 'A' and letter <= 'Z')
            {
                SET_IMAGES_PATHS(str(boost::format("201-%c.png") % letter))
            }
            else throw std::runtime_error("Invalid form.");
        }
        else throw std::runtime_error("Invalid form.");

        unown_signal();
    }

    // Gender doesn't matter for icons in Generation 2
    pkmn::pkstring base_pokemon_gen2impl::get_icon_path(bool is_male) const
    {
        return _male_icon_path.string();
    }

    // Gender doesn't matter for sprites in Generation 2
    pkmn::pkstring base_pokemon_gen2impl::get_sprite_path(bool is_male, bool is_shiny) const
    {
        if(is_shiny) return _male_shiny_sprite_path.string();
        else return _male_sprite_path.string();
    }

    // Set Unown's form without sending IV signal to team_pokemon_gen2impl
    void base_pokemon_gen2impl::_set_unown_form()
} /* namespace pkmn */
