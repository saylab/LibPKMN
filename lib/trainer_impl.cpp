/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/assign.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/trainer.hpp>
#include <pkmn/database.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/prng.hpp>

#include "trainer_impl.hpp"
#include "io/trsql.hpp"

namespace fs = boost::filesystem;

namespace pkmn
{
    trainer::sptr trainer::make(int game, const pkmn::pkstring &name, int gender)
    {
        return sptr(new trainer_impl(game, name, gender));
    }

    trainer::sptr trainer::make(const pkmn::pkstring &game, const pkmn::pkstring &name, const pkmn::pkstring &gender)
    {
        return make(database::get_version_id(game),
                    name,
                    ((gender == "Female") ? Genders::FEMALE : Genders::MALE));
    }

    trainer::sptr trainer::make(const pkmn::pkstring &filename)
    {
        if(fs::extension(fs::path(filename)) == ".trsql" and io::trsql::valid(filename))
            return io::trsql::from(filename);
        else
            throw std::runtime_error("Invalid file.");
    }

    void trainer::export_to(trainer::sptr tr, const pkmn::pkstring &filename)
    {
        pkmn::database_sptr output = io::trsql::to(tr, filename);
        if(not io::trsql::valid(filename))
        {
            //fs::remove(fs::path(filename));
            throw std::runtime_error("Failed to export to TRSQL.");
        }
    }

    trainer_impl::trainer_impl(int game, const pkmn::pkstring &name, int gender): trainer()
    {
        _game_id = game;
        _generation = database::get_generation(_game_id);
        _trainer_name = name;
        _gender_id = gender;
        _money = 0;

        prng::sptr _rand_gen = prng::make(_generation);

        /*
         * Public vs. secret ID's weren't a factor in Generations I-II
         */
        _trainer_id = (_generation < 3) ? (_rand_gen->lcrng() % 65535)
                                        : _rand_gen->lcrng();

        _party = pokemon_team_t(6);
        for(int i = 0; i < 6; i++)
        {
            _party[i] = pokemon::make(Species::NONE, _game_id, 1, Moves::NONE, Moves::NONE,
                                      Moves::NONE, Moves::NONE);
        }

        _bag = bag::make(_game_id);
    }

    pkmn::pkstring trainer_impl::get_game() const {return database::get_version_name(_game_id);}

    int trainer_impl::get_generation() const {return _generation;}

    pkmn::pkstring trainer_impl::get_name() const {return _trainer_name;}

    int trainer_impl::get_money() const {return _money;}

    pkmn::pkstring trainer_impl::get_gender() const
    {
        return (_gender_id == Genders::MALE) ? "Male" : "Female";
    }

    uint32_t trainer_impl::get_id() const
    {
        return (_generation < 3) ? _tid.public_id
                                 : _trainer_id;
    }

    uint16_t trainer_impl::get_public_id() const {return _tid.public_id;}

    uint16_t trainer_impl::get_secret_id() const
    {
        return (_generation < 3) ? 0
                                 : _tid.secret_id;
    }

    void trainer_impl::set_name(const pkmn::pkstring &name)
    {
        _trainer_name = (name.std_string().length() >= 1
                         and name.std_string().length() <= 7) ? name
                                                                : _trainer_name;
    }

    void trainer_impl::set_money(int money)
    {
        if(money < 0 or money > 999999)
            throw std::runtime_error("Money amount must be 0-999999.");

        _money = money;
    }

    void trainer_impl::set_gender(const pkmn::pkstring &gender)
    {
        if(gender == "Male") _gender_id = Genders::MALE;
        else if(gender == "Female") _gender_id = Genders::FEMALE;
    }

    void trainer_impl::set_id(uint32_t id) {_trainer_id = id;}

    void trainer_impl::set_public_id(uint16_t id) {_tid.public_id = id;}

    void trainer_impl::set_secret_id(uint16_t id) {_tid.secret_id = id;}

    pokemon::sptr trainer_impl::get_pokemon(int pos)
    {
        if(pos < 1 or pos > 6)
            throw std::runtime_error("Position must be 1-6.");
        else
            return _party[pos-1];
    }

    void trainer_impl::set_pokemon(int pos, pokemon::sptr pkmn)
    {
        //Check for valid position, don't do anything otherwise
        if(pos >= 1 and pos <= 6)
        {
            //TODO: more through check (items, forms, etc)
            if(database::get_generation(pkmn->get_game_id()) <= _generation)
                _party[pos-1] = pkmn;
        }
    }

    void trainer_impl::remove_pokemon(int pos)
    {
        int actual_pos = (pos > 6) ? 5 : (pos == 0) ? 0 : (pos-1);

        pokemon::sptr blank_pokemon = pokemon::make(Species::NONE, _game_id, 0, Moves::NONE,
                                           Moves::NONE, Moves::NONE, Moves::NONE);

        set_pokemon(actual_pos, blank_pokemon);

        //Move over any non-blank Pokemon in later positions
        for(int i = (actual_pos+1); i < 6; i++)
        {
            if(_party[i]->get_species_id() == Species::NONE) break;
            else
            {
                _party[i-1] = _party[i];
                _party[i] = blank_pokemon;
            }
        }
    }

    const pokemon_team_t& trainer_impl::get_party() const
    {
        return _party;
    }

    void trainer_impl::set_party(const pokemon_team_t &party)
    {
        if(party.size() != 6)
            throw std::runtime_error("Party must be of size 6.");
        else
            _party = party;
    }

    bag::sptr trainer_impl::get_bag() const {return _bag;}

    int trainer_impl::get_game_id() const {return _game_id;}
} /* namespace pkmn */
