/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/format.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/move.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/team_pokemon.hpp>
#include <pkmn/database.hpp>
#include <pkmn/types/prng.hpp>

#include "copy_sptrs.hpp"

#include "team_pokemon_impl.hpp"
#include "team_pokemon_gen1impl.hpp"
#include "team_pokemon_gen2impl.hpp"
#include "team_pokemon_gen3impl.hpp"

namespace pkmn
{
    team_pokemon::sptr team_pokemon::make(unsigned int id, unsigned int game, unsigned int level,
                                          unsigned int move1, unsigned int move2,
                                          unsigned int move3, unsigned int move4)
    {
        base_pokemon::sptr base = base_pokemon::make(id, game);

        if(base->get_generation() < 1 or base->get_generation() > 6) throw std::runtime_error("Gen must be 1-6.");

        switch(base->get_generation())
        {
            case 1:
                return sptr(new team_pokemon_gen1impl(base, game,
                                                      move1, move2, move3, move4));

            case 2:
                return sptr(new team_pokemon_gen2impl(base, game,
                                                      move1, move2, move3, move4));

            case 3:
                return sptr(new team_pokemon_gen3impl(base, game,
                                                      move1, move2, move3, move4));
        }
    }

    team_pokemon::sptr team_pokemon::make(const pkmn::pkstring &name, const pkmn::pkstring &game, unsigned int level,
                                          const pkmn::pkstring &move1, const pkmn::pkstring &move2,
                                          const pkmn::pkstring &move3, const pkmn::pkstring &move4)
    {
        return make(database::get_species_id(name),
                    database::get_version_id(game),
                    level,
                    database::get_move_id(move1),
                    database::get_move_id(move2),
                    database::get_move_id(move3),
                    database::get_move_id(move4));
    }

    pkmn::shared_ptr<SQLite::Database> team_pokemon_impl::_db;

    team_pokemon_impl::team_pokemon_impl(base_pokemon::sptr base):
        team_pokemon(),
        _base(base),
        _prng(prng::make(database::get_generation(base->get_game_id()))),
        _species_id(base->get_species_id()),
        _game_id(base->get_game_id())
    {
        if(!_db) _db = pkmn::shared_ptr<SQLite::Database>(new SQLite::Database(get_database_path()));
    }

    pkmn::pkstring team_pokemon_impl::get_game() const
    {
        return database::get_version_name(_game_id);
    }

    unsigned int team_pokemon_impl::get_generation() const
    {
        return database::get_generation(_game_id);
    }

    base_pokemon::sptr team_pokemon_impl::get_base_pokemon(bool copy) const
    {
        return copy ? copy_base_pokemon(_base) : _base;
    }

    pkmn::pkstring team_pokemon_impl::get_species_name() const
    {
        return _base->get_species();
    }

    pkmn::pkstring team_pokemon_impl::get_original_game() const
    {
        return database::get_version_name(get_original_game_id());
    }

    pkmn::pkstring_pair_t team_pokemon_impl::get_types() const
    {
        return _base->get_types();
    }

    pkmn::dict<pkmn::pkstring, unsigned int> team_pokemon_impl::get_base_stats() const
    {
        return _base->get_base_stats();
    }

    void team_pokemon_impl::set_original_game(const pkmn::pkstring &game)
    {   
        set_original_game(database::get_version_id(game));
    }

    int team_pokemon_impl::get_attribute(const pkmn::pkstring &attribute) const
    {
        return _attributes.at(attribute);
    }

    pkmn::dict<pkmn::pkstring, int> team_pokemon_impl::get_attributes() const
    {
        return _attributes;
    }

    bool team_pokemon_impl::has_attribute(const pkmn::pkstring &attribute) const
    {
        return _attributes.has_key(attribute);
    }

    void team_pokemon_impl::set_attribute(const pkmn::pkstring &attribute, int value)
    {
        _attributes[attribute] = value;
    }

    pkmn::pkstring team_pokemon_impl::get_icon_path() const
    {
        return _base->get_icon_path(get_gender() == "Male");
    }

    pkmn::pkstring team_pokemon_impl::get_sprite_path() const
    {
        return _base->get_sprite_path(get_gender() == "Male", is_shiny());
    }

    // NOTE: Could affect IV's and stats
    void team_pokemon_impl::set_form(const pkmn::pkstring &form)
    {
        _base->set_form(form);
    }

    // NOTE: Could affect IV's and stats
    void team_pokemon_impl::set_form(unsigned int form)
    {
        _base->set_form(form);
    }

    unsigned int team_pokemon_impl::get_pokemon_id() const
    {
        return _base->get_pokemon_id();
    }

    unsigned int team_pokemon_impl::get_species_id() const
    {
        return _species_id;
    }

    unsigned int team_pokemon_impl::get_game_id() const
    {
        return _game_id;
    }

    unsigned int team_pokemon_impl::get_form_id() const
    {
        return _base->get_form_id();
    }
} /* namespace pkmn */
