/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/filesystem.hpp>

#include <pkmn/conversions/items.hpp>
#include <pkmn/conversions/pokemon.hpp>
#include <pkmn/conversions/text.hpp>
#include <pkmn/native/gen4_save.hpp>

#include "game_save_gen4impl.hpp"

namespace fs = boost::filesystem;

namespace pkmn
{
    game_save_gen4impl::game_save_gen4impl(const pkmn::pkstring &filename,
                                           gen4_games_t game,
                                           bool small):
        game_save_impl(filename),
        _game_type(game),
        _game_id(libpkmn_version_ids[game]),
        _small(small)
    {
        load();
    }

    #define GEN4_DATA(arr,name)                 (&arr[gen4_offsets[name][_game_type]])
    #define GEN4_DATA_CAST(arr,name,type)       (reinterpret_cast<type*>(GEN4_DATA(arr,name)))
    #define GEN4_DATA_CONST_CAST(arr,name,type) (reinterpret_cast<const type*>(GEN4_DATA(arr,name)))

    void game_save_gen4impl::load()
    {
        uint8_t* data2 = &_data[0x40000];

        uint8_t* blockA1 = GEN4_DATA(_data, GEN4_BLOCKA);
        native::gen4_footer_t* footerA1 = GEN4_DATA_CAST(_data, GEN4_FOOTERA, native::gen4_footer_t);

        uint8_t* blockB1 = GEN4_DATA(_data, GEN4_BLOCKB);
        native::gen4_footer_t* footerB1 = GEN4_DATA_CAST(_data, GEN4_FOOTERB, native::gen4_footer_t);

        if(_small)
        {
            _blocks[A] = blockA1;
            _blocks[B] = blockB1;
            _footers[A] = footerA1;
            _footers[B] = footerB1;
        }
        else
        {
            uint8_t* blockA2 = GEN4_DATA(data2, GEN4_BLOCKA);
            native::gen4_footer_t* footerA2 = GEN4_DATA_CAST(data2, GEN4_FOOTERA, native::gen4_footer_t);

            uint8_t* blockB2 = GEN4_DATA(data2, GEN4_BLOCKB);
            native::gen4_footer_t* footerB2 = GEN4_DATA_CAST(data2, GEN4_FOOTERB, native::gen4_footer_t);

            if(_game_type == HGSS)
            {
                if(footerA1->hgss.save_num >= footerA2->hgss.save_num)
                {
                    _blocks[A] = blockA1;
                    _footers[A] = footerA1;
                }
                else
                {
                    _blocks[A] = blockA2;
                    _footers[A] = footerA2;
                }

                if(footerB1->hgss.save_num >= footerB2->hgss.save_num)
                {
                    _blocks[B] = blockB1;
                    _footers[B] = footerB1;
                }
                else
                {
                    _blocks[B] = blockB2;
                    _footers[B] = footerB2;
                }
            }
            else
            {
                if(footerA1->dppt.save_general >= footerA2->dppt.save_general)
                {
                    _blocks[A] = blockA1;
                    _footers[A] = footerA1;
                }
                else
                {
                    _blocks[A] = blockA2;
                    _footers[A] = footerA2;
                }

                if(footerB1->dppt.save_storage >= footerB2->dppt.save_storage)
                {
                    _blocks[B] = blockB1;
                    _footers[B] = footerB1;
                }
                else
                {
                    _blocks[B] = blockB2;
                    _footers[B] = footerB2;
                }
            }
        }

        _party   = GEN4_DATA_CAST(_blocks[A], GEN4_PARTY, native::nds_pokemon_party_t);
        _items   = GEN4_DATA_CAST(_blocks[A], GEN4_ITEMS, void);

        _trainer = trainer::make(_game_id,
                                 conversions::import_gen4_text(GEN4_DATA_CAST(_data, GEN4_NAME, uint16_t), 8),
                                 *GEN4_DATA(_data, GEN4_GENDER) ? Genders::FEMALE : Genders::MALE);

        for(size_t i = 0; i < _party->count; i++)
        {
            _trainer->set_pokemon((i+1), conversions::import_nds_pokemon(_party->party[i],
                                                                         get_game()));
        }
        conversions::import_gen4_bag(_trainer->get_bag(), _items);

        _trainer->set_id(*GEN4_DATA_CAST(_data, GEN4_TID, uint32_t));
        _trainer->set_money(*GEN4_DATA_CAST(_data, GEN4_MONEY, uint32_t));
    }

    pkmn::datetime_t game_save_gen4impl::get_time_played() const
    {
        return *GEN4_DATA_CONST_CAST(_data, GEN4_TIMEPLAYED, uint32_t);
    }

    void game_save_gen4impl::set_time_played(pkmn::datetime_t &datetime)
    {
        *GEN4_DATA_CAST(_data, GEN4_TIMEPLAYED, uint32_t) = datetime;
    }

    void game_save_gen4impl::_write_data()
    {
    }
} /* namespace pkmn */
