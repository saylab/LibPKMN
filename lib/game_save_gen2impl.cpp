/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <fstream>

#include <pkmn/enums.hpp>

#include <pkmn/conversions/items.hpp>
#include <pkmn/conversions/misc.hpp>
#include <pkmn/conversions/pokemon.hpp>
#include <pkmn/conversions/text.hpp>

#include "game_save_gen2impl.hpp"

namespace fs = boost::filesystem;

namespace pkmn
{
    game_save_gen2impl::game_save_gen2impl(const pkmn::pkstring& filename, bool crystal):
        game_save_impl(filename)
    {
        _version_id = crystal ? Versions::CRYSTAL : Versions::GOLD;

        int game = crystal ? gen2_games::CRYSTAL : gen2_games::GS;
        _player_id_offset = offsets[game][GEN2_PLAYER_ID];
        _player_name_offset = offsets[game][GEN2_PLAYER_NAME];
        _rival_name_offset = offsets[game][GEN2_RIVAL_NAME];
        _daylight_savings_offset = offsets[game][GEN2_DAYLIGHT_SAVINGS];
        _time_played_offset = offsets[game][GEN2_TIME_PLAYED];
        _money_offset = offsets[game][GEN2_MONEY];
        _item_bag_offset = offsets[game][GEN2_ITEM_BAG];
        _current_pokemon_box_num_offset = offsets[game][GEN2_CURRENT_POKEMON_BOX_NUM];
        _pc_box_names_offset = offsets[game][GEN2_PC_BOX_NAMES];
        _pokemon_party_offset = offsets[game][GEN2_POKEMON_PARTY];
        _pokedex_owned_offset = offsets[game][GEN2_POKEDEX_OWNED];
        _pokedex_seen_offset = offsets[game][GEN2_POKEDEX_SEEN];
        _current_pokemon_box_offset = offsets[game][GEN2_CURRENT_POKEMON_BOX];
        _player_gender_offset = offsets[game][GEN2_PLAYER_GENDER];
        _pokemon_pc_offset = offsets[game][GEN2_POKEMON_PC];

        load();
    }

    void game_save_gen2impl::load()
    {
        _item_bag = reinterpret_cast<native::gen2_item_bag_t*>(&_data[_item_bag_offset]);
        _item_pc = reinterpret_cast<native::gen2_item_pc_t*>(&_data[_item_pc_offset]);
        _pokemon_party = reinterpret_cast<native::gen2_pokemon_party_t*>(&_data[_pokemon_party_offset]);
        _pokemon_pc = reinterpret_cast<native::gen2_pokemon_pc_t*>(&_data[_pokemon_pc_offset]);

        _trainer = trainer::make(_version_id,
                                 conversions::import_gen2_text(&_data[_player_name_offset], 7),
                                 _data[_player_gender_offset] ? Genders::FEMALE : Genders::MALE);

        for(size_t i = 0; i < _pokemon_party->count; i++)
        {
            _trainer->set_pokemon((i+1), conversions::import_gen2_pokemon(_pokemon_party->party[i],
                                                                          _pokemon_party->nicknames[i],
                                                                          _pokemon_party->otnames[i],
                                                                          get_game()));
        }
        conversions::import_gen2_bag(_trainer->get_bag(), _item_bag);

        _trainer->set_id(*reinterpret_cast<uint16_t*>(&_data[_player_id_offset]));

        _trainer->set_money(conversions::import_gb_money(&_data[_money_offset]));
    }

    void game_save_gen2impl::_write_data()
    {
        conversions::export_gen2_bag(_trainer->get_bag(), _item_bag);
        *reinterpret_cast<uint16_t*>(&_data[_player_id_offset]) = _trainer->get_public_id();
        conversions::export_gb_money(_trainer->get_money(), &_data[_money_offset]);
        native::set_gen2_save_checksums(_data, (_version_id == Versions::CRYSTAL));

        pokemon_team_t team = _trainer->get_party();
        _pokemon_party->count = 0;
        for(size_t i = 0; i < 6; i++)
        {
            if(team[i]->get_species_id() == Species::NONE) break;
            else
            {
                _pokemon_party->count++;
                conversions::export_gen2_pokemon(team[i], _pokemon_party->party[i],
                                                 _pokemon_party->nicknames[i],
                                                 _pokemon_party->otnames[i]);
            }
        }
    }
} /* namespace pkmn */
