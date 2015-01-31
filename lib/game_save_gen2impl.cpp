/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <fstream>

#include <pkmn/enums.hpp>

#include <pkmn/conversions/items.hpp>
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

        uint8_t game = crystal ? gen2_games::CRYSTAL : gen2_games::GS;
        _player_id_offset = offsets[game][gen2_offsets::PLAYER_ID];
        _player_name_offset = offsets[game][gen2_offsets::PLAYER_NAME];
        _rival_name_offset = offsets[game][gen2_offsets::RIVAL_NAME];
        _daylight_savings_offset = offsets[game][gen2_offsets::DAYLIGHT_SAVINGS];
        _time_played_offset = offsets[game][gen2_offsets::TIME_PLAYED];
        _money_offset = offsets[game][gen2_offsets::MONEY];
        _item_bag_offset = offsets[game][gen2_offsets::ITEM_BAG];
        _current_pokemon_box_num_offset = offsets[game][gen2_offsets::CURRENT_POKEMON_BOX_NUM];
        _pc_box_names_offset = offsets[game][gen2_offsets::PC_BOX_NAMES];
        _pokemon_party_offset = offsets[game][gen2_offsets::POKEMON_PARTY];
        _pokedex_owned_offset = offsets[game][gen2_offsets::POKEDEX_OWNED];
        _pokedex_seen_offset = offsets[game][gen2_offsets::POKEDEX_SEEN];
        _current_pokemon_box_offset = offsets[game][gen2_offsets::CURRENT_POKEMON_BOX];
        _player_gender_offset = offsets[game][gen2_offsets::PLAYER_GENDER];
        _pokemon_pc_offset = offsets[game][gen2_offsets::POKEMON_PC];

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

        uint8_t* raw_money = &_data[_money_offset];
        _trainer->set_money(   (raw_money[0] >> 4) * 100000
                            + ((raw_money[0] >> 0) & 0x0F) * 10000
                            +  (raw_money[1] >> 4) * 1000
                            + ((raw_money[1] >> 0) & 0x0F) * 100
                            +  (raw_money[2] >> 4) * 10
                            + ((raw_money[2] >> 0) & 0x0F));
    }

    void game_save_gen2impl::save_as(const pkmn::pkstring &filename)
    {
        _filepath = fs::path(filename);

        conversions::export_gen2_bag(_trainer->get_bag(), _item_bag);

        pokemon_team_t team;
        _trainer->get_party(team);
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

        *reinterpret_cast<uint16_t*>(&_data[_player_id_offset]) = _trainer->get_public_id();

        uint32_t money = _trainer->get_money();
        uint8_t* raw_money = &_data[_money_offset];
        raw_money[0] = ((money / 100000) << 4) | ((money % 100000) / 10000);
        raw_money[1] = (((money % 10000) / 1000) << 4) | ((money % 1000) / 100);
        raw_money[2] = (((money % 100) / 10) << 4) | (money % 10);

        //Set new checksums
        uint32_t checksum1 = 0;
        uint32_t checksum2 = 0;

        if(_version_id == Versions::CRYSTAL)
        {
            //Checksum 1
            for(size_t i = 0x2009; i <= 0x2B82; i++) checksum1 += _data[i];

            //Checksum 2
            for(size_t i = 0x1209; i <= 0x1D82; i++) checksum2 += _data[i];
        }
        else
        {
            //Checksum 1
            for(size_t i = 0x2009; i <= 0x2D68; i++) checksum1 += _data[i];

            //Checksum 2
            for(size_t i = 0x0C6B; i <= 0x17EC; i++) checksum2 += _data[i];
            for(size_t i = 0x3D96; i <= 0x3F3F; i++) checksum2 += _data[i];
            for(size_t i = 0x7E39; i <= 0x7E6C; i++) checksum2 += _data[i];
        }

        _data[_checksum1_offset] = checksum1;
        _data[_checksum2_offset] = checksum2;

        std::ofstream ofile(filename.const_char());
        ofile.write((char*)&_data, _data.size());
        ofile.close();

        _filepath = fs::path(filename);
    };

    bool game_save_gen2impl::check()
    {
        return (_version_id == Versions::CRYSTAL) ? crystal_check(_data)
                                                  : gs_check(_data);
    }
} /* namespace pkmn */
