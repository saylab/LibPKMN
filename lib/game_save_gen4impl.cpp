/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/filesystem.hpp>

#include "game_save_gen4impl.hpp"

#include "conversions/items.hpp"
#include "conversions/text.hpp"

namespace fs = boost::filesystem;

namespace pkmn
{
    game_save_gen4impl::game_save_gen4impl(pokelib_sptr ptr,
                                           const std::string &filename): game_save_impl(filename)
    {
    }

    game_save_gen4impl::game_save_gen4impl(const std::string &filename,
                                           gen4_games_t game):
        game_save_impl(filename),
        _game_type(game),
        _game_id(libpkmn_version_ids[game])
    {
        load();
    }

    #define GEN4_DATA(arr,name)           (&arr[gen4_offsets[name][_game_type]])
    #define GEN4_DATA_CAST(arr,name,type) (reinterpret_cast<type*>(GEN4_DATA(arr,name)))

    void game_save_gen4impl::load()
    {
        _blocks[A]  = GEN4_DATA(_data, gen4_blockA);
        _blocks[B]  = GEN4_DATA(_data, gen4_blockB);
        _footers[A] = GEN4_DATA_CAST(_data, gen4_footerA, gen4_footer_t);
        _footers[B] = GEN4_DATA_CAST(_data, gen4_footerB, gen4_footer_t);

        _party      = GEN4_DATA_CAST(_blocks[A], gen4_party, nds_pokemon_party_t);
        _items      = GEN4_DATA_CAST(_blocks[A], gen4_items, void);

        _trainer = trainer::make(_game_id,
                                 conversions::import_gen4_text(GEN4_DATA_CAST(_data, gen4_name, uint16_t), 8),
                                 *GEN4_DATA(_data, gen4_gender) ? Genders::FEMALE : Genders::MALE);

        for(size_t i = 0; i < _party->count; i++)
        {
            _trainer->set_pokemon((i+1), conversions::import_nds_pokemon(_party->party[i],
                                                                         _game_id));
        }
        conversions::import_gen4_bag(_trainer->get_bag(), _items);

        _trainer->set_id(*GEN4_DATA_CAST(_data, gen4_tid, uint32_t));
        _trainer->set_money(*GEN4_DATA_CAST(_data, gen4_money, uint32_t));
    }

    void game_save_gen4impl::save_as(const std::string &filename)
    {
        //TODO: actual saving stuff
        _filepath = fs::path(filename);
    }
} /* namespace pkmn */
