/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/enums.hpp>
#include <pkmn/database.hpp>

#include "game_save_gen1impl.hpp"

namespace fs = boost::filesystem;

namespace pkmn
{
    game_save_gen1impl::game_save_gen1impl(const std::string &filename): game_save_impl(filename)
    {
        /*
         * The only real distinction between the Generation I games is the introduction
         * of friendship. The starter Pikachu has a unique friendship value (0-255) that
         * is stored at a known offset in the save, so we'll use this to determine if it's
         * Red/Blue or Yellow. The only limitation of this is that if the trainer's Pikachu
         * despises it and has a friendship value of 0, LibPKMN will consider it a Red save.
         *
         * There is no known way to distinguish between a Red save and Blue save.
         */
        _game_id = (_data[gen1_offsets::PIKACHU_FRIENDSHIP] == 0) ? Versions::RED
                                                                  : Versions::YELLOW;

        load();
    }

    void game_save_gen1impl::load()
    {
        _item_bag = reinterpret_cast<gen1_item_bag_t*>(&_data[gen1_offsets::ITEM_BAG]);
        _item_pc = reinterpret_cast<gen1_item_pc_t*>(&_data[gen1_offsets::ITEM_PC]);
        _pokemon_party = reinterpret_cast<gen1_pokemon_party_t*>(&_data[gen1_offsets::POKEMON_PARTY]);
        _pokemon_pc = reinterpret_cast<gen1_pokemon_pc_t*>(&_data[gen1_offsets::POKEMON_PC]);

        _trainer = trainer::make(Versions::YELLOW,
                                 conversions::import_gen1_text(&_data[gen1_offsets::PLAYER_NAME], 7),
                                 Genders::MALE);
        conversions::import_gen1_bag(_trainer->get_bag(), _item_bag);

        pokemon_team_t team(6);
        for(size_t i = 0; i < _pokemon_party->count; i++)
        {
            team[i] = conversions::import_gen1_pokemon(_pokemon_party->party[i],
                                                       _pokemon_party->nicknames[i],
                                                       _pokemon_party->otnames[i]);
        }
        _trainer->set_party(team);

        _trainer->set_id(*reinterpret_cast<uint16_t*>(&_data[gen1_offsets::PLAYER_ID]));

        uint8_t* raw_money = &_data[gen1_offsets::MONEY];
        _trainer->set_money(   (raw_money[0] >> 4) * 100000
                            + ((raw_money[0] >> 0) & 0x0F) * 10000
                            +  (raw_money[1] >> 4) * 1000
                            + ((raw_money[1] >> 0) & 0x0F) * 100
                            +  (raw_money[2] >> 4) * 10
                            + ((raw_money[2] >> 0) & 0x0F));
    }

    void game_save_gen1impl::save_as(const std::string &filename)
    {
        _filepath = fs::path(filename);

        conversions::export_gen1_bag(_trainer->get_bag(), _item_bag);

        pokemon_team_t team;
        _trainer->get_party(team);
        _pokemon_party->count = 0;
        for(size_t i = 0; i < 6; i++)
        {
            if(team[i]->get_species_id() == Species::NONE) break;
            else
            {
                _pokemon_party->count++;
                conversions::export_gen1_pokemon(team[i], _pokemon_party->party[i],
                                                 _pokemon_party->nicknames[i],
                                                 _pokemon_party->otnames[i]);
            }
        }

        *reinterpret_cast<uint16_t*>(&_data[gen1_offsets::PLAYER_ID]) = _trainer->get_public_id();

        uint32_t money = _trainer->get_money();
        uint8_t* raw_money = &_data[gen1_offsets::MONEY];
        raw_money[0] = ((money / 100000) << 4) | ((money % 100000) / 10000);
        raw_money[1] = (((money % 10000) / 1000) << 4) | ((money % 1000) / 100);
        raw_money[2] = (((money % 100) / 10) << 4) | (money % 10);

        //Set new checksum
        uint8_t checksum = 255;
        for(size_t i = 0x2598; i < gen1_offsets::CHECKSUM; i++) checksum -= _data[i];
        _data[gen1_offsets::CHECKSUM] = checksum;

        std::ofstream ofile(filename.c_str());
        ofile.write((char*)&_data, _data.size());
        ofile.close();
        
        _filepath = fs::path(filename);
    };

    bool game_save_gen1impl::check() {return gen1_check(_data);}
} /* namespace pkmn */
