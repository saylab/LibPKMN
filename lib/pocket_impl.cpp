/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <string>

#include <boost/foreach.hpp>

#include <pkmn/database.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/pocket.hpp>
#include <pkmn/pokedex.hpp>

#include "pocket_impl.hpp"

namespace pkmn
{
    pocket::sptr pocket::make(int game, const pkmn::pkstring &name, int size)
    {
        return sptr(new pocket_impl(game, name, size));
    }

    pocket::sptr pocket::make(const pkmn::pkstring &game, const pkmn::pkstring &name, int size)
    {
        return make(database::get_version_id(game), name, size);
    }

    pocket_impl::pocket_impl(int game, const pkmn::pkstring &name, int size):
        pocket(),
        _version_id(game),
        _generation(database::get_generation(game)),
        _pocket_size(size),
        _pocket_name(name),
        _pokedex(pokedex::make(database::get_version_name(game)))
    {};

    pkmn::pkstring pocket_impl::get_game() const
    {
        return database::get_version_name(_version_id);
    }

    int pocket_impl::get_generation() const
    {
        return _generation;
    }

    pkmn::pkstring pocket_impl::get_name() const
    {
        return _pocket_name;
    }

    int pocket_impl::get_size() const
    {
        return _pocket_size;
    }

    int pocket_impl::get_item_amount(int item_id) const
    {
        return get_item_amount(database::get_item_name(item_id));
    }

    int pocket_impl::get_item_amount(const pkmn::pkstring &item_name) const
    {
        BOOST_FOREACH(const bag_slot_t &bag_slot, _item_list)
        {
            if(bag_slot.first.name == item_name)
                return bag_slot.second;
        }

        return 0;
    }

    void pocket_impl::set_item_amount(int item_id, int amount)
    {
        set_item_amount(database::get_item_name(item_id), amount);
    }

    void pocket_impl::set_item_amount(const pkmn::pkstring &item_name, int amount)
    {
        for(int i = 0; i < _item_list.size(); i++)
        {
            if(_item_list[i].first.name == item_name)
            {
                if(amount > 0)
                    _item_list[i].second = std::min(amount, 99);
                else
                    _item_list.erase(_item_list.begin()+i);
            }
        }

        if(_item_list.size() < _pocket_size)
            _item_list.push_back(std::make_pair(_pokedex->get_item_entry(item_name),
                                                std::min(amount, 99)));
    }

    const item_list_t& pocket_impl::get_item_list() const
    {
        return _item_list;
    }

    int pocket_impl::get_game_id() const
    {
        return _version_id;
    }
} /* namespace pkmn */
