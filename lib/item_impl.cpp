/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <boost/format.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/item.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database.hpp>

#include "item_impl.hpp"

namespace pkmn
{
    item::sptr item::make(unsigned int id, unsigned int game)
    {
        return sptr(new item_impl(id, game));
    }

    item::sptr item::make(const pkmn::pkstring &name, const pkmn::pkstring &game)
    {
        return make(database::get_item_id(name), database::get_version_id(game));
    }

    pkmn::shared_ptr<SQLite::Database> item_impl::_db;

    item_impl::item_impl(unsigned int id, unsigned int game):
        item(),
        _item_id(id),
        _game_id(game),
        _generation(database::get_generation(game)),
        _game_index(0),
        _tmhm(false)
    {
        if(!_db) _db = pkmn::shared_ptr<SQLite::Database>(new SQLite::Database(get_database_path().const_char()));

        // Confirm item exists in given generation
        std::ostringstream query_stream;
        query_stream << "SELECT game_index FROM item_game_indices WHERE item_id="
                     << _item_id << " AND generation_id=" << _generation;
        SQLite::Statement query(*_db, query_stream.str().c_str());
        if((_valid = query.executeStep()))
        {
            _game_index = query.getColumn(0);
            _tmhm = ((id >= Items::TM01 and id <= Items::HM08)
                      or (id >= Items::TM93 and id <= Items::TM95)
                       or (id >= Items::TM96 and id <= Items::TM100));
        }
        else if(_item_id != Items::NONE) _item_id = Items::INVALID;
    }

    pkmn::pkstring item_impl::get_game() const {return database::get_version_name(_game_id);}

    unsigned int item_impl::get_generation() const {return _generation;}

    pkmn::pkstring item_impl::get_name() const
    {
        if(_tmhm)
        {
            unsigned int machine_id;
            if(_item_id >= Items::TM01 and _item_id <= Items::TM92) machine_id = _item_id - 304;
            else machine_id = 101 + (_item_id - Items::HM01);

            std::ostringstream query_stream;
            query_stream << "SELECT move_id FROM machines WHERE machine_number=" << machine_id
                         << " AND version_group_id=" << database::get_version_group_id(_game_id);
            unsigned int move_id = _db->execAndGet(query_stream.str().c_str());

            return str(boost::format("%s (%s)") % database::get_item_name(_item_id)
                                                % database::get_move_name(move_id));
        }
        else return database::get_item_name(_item_id);
    }

    pkmn::pkstring item_impl::get_description() const
    {
        if(_tmhm)
        {
            unsigned int machine_id;
            if(_item_id >= Items::TM01 and _item_id <= Items::TM92) machine_id = _item_id - 304;
            else machine_id = 101 + (_item_id - Items::HM01);

            std::ostringstream query_stream;
            query_stream << "SELECT move_id FROM machines WHERE machine_number=" << machine_id
                         << " AND version_group_id=" << database::get_version_group_id(_game_id);
            unsigned int move_id = _db->execAndGet(query_stream.str().c_str());

            return str(boost::format("%s - %s") % database::get_move_name(move_id)
                                                % database::get_move_description(move_id, _game_id));
        }
        else return database::get_item_description(_item_id, _game_id);
    }

    pkmn::pkstring item_impl::get_category() const
    {
        return database::get_item_category_name(_item_id);
    }

    unsigned int item_impl::get_game_index() const {return _game_index;}

    unsigned int item_impl::get_item_id() const {return _item_id;}

    unsigned int item_impl::get_category_id() const
    {
        return database::get_item_category_id(_item_id);
    }

    unsigned int item_impl::get_game_id() const {return _game_id;}
} /* namespace pkmn */
