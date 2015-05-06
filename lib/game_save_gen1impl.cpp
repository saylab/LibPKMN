/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/enums.hpp>
#include <pkmn/database.hpp>
#include <pkmn/conversions/items.hpp>
#include <pkmn/conversions/misc.hpp>
#include <pkmn/conversions/pokemon.hpp>
#include <pkmn/conversions/text.hpp>
#include <pkmn/native/checksum.hpp>

#include "game_save_gen1impl.hpp"

namespace fs = boost::filesystem;

namespace pkmn
{
    game_save_gen1impl::game_save_gen1impl(const pkmn::pkstring &filename):
        game_save_impl(filename)
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
        _version_id = (_data[GEN1_PIKACHU_FRIENDSHIP] == 0) ? Versions::RED
                                                            : Versions::YELLOW;

        load();
    }

    void game_save_gen1impl::load()
    {
        _item_bag = reinterpret_cast<native::gen1_item_bag_t*>(&_data[GEN1_ITEM_BAG]);
        _item_pc = reinterpret_cast<native::gen1_item_pc_t*>(&_data[GEN1_ITEM_PC]);
        _pokemon_party = reinterpret_cast<native::gen1_pokemon_party_t*>(&_data[GEN1_POKEMON_PARTY]);
        _pokemon_pc = reinterpret_cast<native::gen1_pokemon_pc_t*>(&_data[GEN1_POKEMON_PC]);

        _trainer = trainer::make(Versions::YELLOW,
                                 conversions::import_gen1_text(&_data[GEN1_PLAYER_NAME], 7),
                                 Genders::MALE);
        conversions::import_gen1_bag(_trainer->get_bag(), _item_bag);

        pokemon_team_t team(6);
        for(size_t i = 0; i < _pokemon_party->count; i++)
        {
            team[i] = conversions::import_gen1_pokemon(_pokemon_party->party[i],
                                                       _pokemon_party->nicknames[i],
                                                       _pokemon_party->otnames[i],
                                                       get_game());
        }
        _trainer->set_party(team);

        _trainer->set_id(*reinterpret_cast<uint16_t*>(&_data[GEN1_PLAYER_ID]));

        _trainer->set_money(conversions::import_gb_money(&_data[GEN1_MONEY]));
    }

    void game_save_gen1impl::_write_data()
    {
        conversions::export_gen1_bag(_trainer->get_bag(), _item_bag);
        *reinterpret_cast<uint16_t*>(&_data[GEN1_PLAYER_ID]) = _trainer->get_public_id();
        conversions::export_gb_money(_trainer->get_money(), &_data[GEN1_MONEY]);
        native::set_gen1_save_checksum(_data);

        pokemon_team_t team = _trainer->get_party();
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
    }
} /* namespace pkmn */
