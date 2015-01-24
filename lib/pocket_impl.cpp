/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <string>

#include "pocket_impl.hpp"

#include <pkmn/database.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/pocket.hpp>
#include <pkmn/pokedex.hpp>

namespace pkmn
{
    pocket::sptr pocket::make(uint16_t game, const pkmn::pkstring& name, uint16_t size)
    {
        return sptr(new pocket_impl(game, name, size));
    }

    pocket::sptr pocket::make(const pkmn::pkstring& game, const pkmn::pkstring& name, uint16_t size)
    {
        return make(database::get_version_id(game), name, size);
    }

    pocket_impl::pocket_impl(uint16_t game, const pkmn::pkstring& name, uint16_t size):
        pocket(),
        _version_id(game),
        _generation(database::get_generation(game)),
        _pocket_size(size),
        _pocket_name(name),
        _pokedex(pokedex::make(database::get_version_name(game))) {};

    pkmn::pkstring pocket_impl::get_game() const
    {
        return database::get_version_name(_version_id);
    }

    uint16_t pocket_impl::get_generation() const
    {
        return _generation;
    }

    pkmn::pkstring pocket_impl::get_name() const
    {
        return _pocket_name;
    }

    uint16_t pocket_impl::size() const
    {
        return _pocket_size;
    }

    void pocket_impl::add_item(const pkmn::pkstring& item_name, uint16_t amount)
    {
        add_item(database::get_item_id(item_name), amount);
    }

    void pocket_impl::add_item(uint16_t item_id, uint16_t amount)
    {
        if(database::get_item_game_index(item_id, _version_id) != 0)
        {
            if(_item_dict.has_key(item_id) or _item_dict.size() < _pocket_size)
                _item_dict[item_id] = std::min(int(amount), 99);
        }
    }

    void pocket_impl::remove_item(const pkmn::pkstring& item_name, uint16_t amount)
    {
        remove_item(database::get_item_id(item_name), amount);
    }

    void pocket_impl::remove_item(uint16_t item_id, uint16_t amount)
    {
        if(_item_dict.has_key(item_id))
        {
            if(amount < _item_dict[item_id]) _item_dict[item_id] -= amount;
            else _item_dict.erase(item_id);
        }
    }

    uint16_t pocket_impl::get_item_amount(const pkmn::pkstring& item_name) const
    {
        return get_item_amount(database::get_item_id(item_name));
    }

    uint16_t pocket_impl::get_item_amount(uint16_t item_id) const
    {
        return _item_dict.at(item_id, 0);
    }

    /*
     * Return list of items with more intuitive interface without exposing
     * internal memory structure.
     */
    void pocket_impl::get_item_list(item_list_t& item_list) const
    {
        item_list.clear();

        std::vector<uint16_t> keys = _item_dict.keys();
        std::vector<uint8_t> vals = _item_dict.vals();

        for(size_t i = 0; i < keys.size(); i++)
        {
            item_list.push_back(std::make_pair(_pokedex->get_item_entry(keys[i]),
                                               vals[i]));
        }
    }

    uint16_t pocket_impl::get_game_id() const
    {
        return _version_id;
    }
} /* namespace pkmn */
