/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <fstream>

#include <pkmn/enums.hpp>
#include <pkmn/database.hpp>

#include <pkmn/conversions/items.hpp>
#include <pkmn/conversions/pokemon.hpp>
#include <pkmn/conversions/text.hpp>
#include <pkmn/native/checksum.hpp>

#include "game_save_gen3impl.hpp"
 
namespace fs = boost::filesystem;

namespace pkmn
{
    game_save_gen3impl::game_save_gen3impl(const pkmn::pkstring &filename, uint16_t game_id):
        game_save_impl(filename)
    {
        _version_id = game_id;
        if(game_id == Versions::RUBY) _game = RS;
        else if(game_id == Versions::FIRERED) _game = FRLG;
        else _game = EMERALD;

        load();
    }
    
    void game_save_gen3impl::load()
    {
        _raw_save = gen3_get_main_save(&_data[0]);

        //Unscramble raw save so sections are in correct order
        _save = new native::gen3_save_t;
        gen3_crypt_save_sections(_raw_save, _save);

        _pokemon_party = reinterpret_cast<native::gen3_pokemon_party_t*>(&(_save->section1.data8[section1_offsets[_game][POKEMON_PARTY]]));
        _item_storage = reinterpret_cast<void*>(&(_save->section1.data8[section1_offsets[_game][ITEM_STORAGE]]));

        _security_key = _save->section0.data32[SECURITY_KEY1/4];

        _trainer = trainer::make(_version_id,
                                 conversions::import_gen3_text(_save->trainer_info.name, 7),
                                 _save->trainer_info.gender ? Genders::FEMALE : Genders::MALE);

        for(size_t i = 0; i < _pokemon_party->count; i++)
        {
            _trainer->set_pokemon((i+1), conversions::import_gen3_pokemon(_pokemon_party->party[i],
                                                                          get_game()));
        }
        conversions::import_gen3_bag(_trainer->get_bag(), _item_storage,
                                     (reinterpret_cast<uint16_t*>(&_security_key)[1]));

        _trainer->set_id(_save->trainer_info.tid);

        _trainer->set_money(_save->section1.data32[MONEY/4] ^ _security_key);
    }

    pkmn::datetime_t game_save_gen3impl::get_time_played() const
    {
        return _save->trainer_info.time_played;
    }

    void game_save_gen3impl::set_time_played(pkmn::datetime_t &datetime)
    {
        _save->trainer_info.time_played = datetime;
    }

    void game_save_gen3impl::_write_data()
    {
        conversions::export_gen3_bag(_trainer->get_bag(), _item_storage, _security_key);
        _save->trainer_info.tid = _trainer->get_id();
        _save->section1.data32[MONEY/4] = _trainer->get_money() ^ _security_key;
        native::set_gen3_save_checksums(_save);

        pokemon_team_t team = _trainer->get_party();
        _pokemon_party->count = 0;
        for(size_t i = 0; i < 6; i++)
        {
            if(team[i]->get_species_id() == Species::NONE) break;
            else
            {
                _pokemon_party->count++;
                conversions::export_gen3_pokemon(team[i], _pokemon_party->party[i]);
            }
        }

        gen3_crypt_save_sections(_save, _raw_save);
    }
} /* namespace pkmn */
