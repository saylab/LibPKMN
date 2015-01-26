/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/enums.hpp>
#include <pkmn/types/nature.hpp>
#include <pkmn/database.hpp>

namespace pkmn
{
    nature_t::nature_t(uint16_t id)
    {
        _id = id;
        _name = database::get_nature_name(id);

        _populate_dict();
    }

    nature_t::nature_t(const pkmn::pkstring &name)
    {
        _name = name;
        _id = database::get_nature_id(name);

        _populate_dict();
    }

    pkmn::pkstring nature_t::get_name() {return _name;}

    const float& nature_t::operator[](const pkmn::pkstring &key) const
    {
        return _modifiers[key];
    }    

    const float& nature_t::operator[](const uint16_t &key) const
    {
        return _modifiers[database::get_nature_name(key)];
    }    

    uint16_t nature_t::get_nature_id() {return _id;}

    void nature_t::_populate_dict()
    {
        _modifiers["Attack"] = database::get_nature_stat_mod(_id, Stats::ATTACK);
        _modifiers["Defense"] = database::get_nature_stat_mod(_id, Stats::DEFENSE);
        _modifiers["Special Attack"] = database::get_nature_stat_mod(_id, Stats::SPECIAL_ATTACK);
        _modifiers["Special Defense"] = database::get_nature_stat_mod(_id, Stats::SPECIAL_DEFENSE);
        _modifiers["Speed"] = database::get_nature_stat_mod(_id, Stats::SPEED);
    }

} /* namespace pkmn */
