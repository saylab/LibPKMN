/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <sstream>

#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/pocket.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database.hpp>
#include <pkmn/types/dict.hpp>

#include "internal.hpp"
#include "bag_impl.hpp"
#include "copy_sptrs.hpp"

namespace pkmn
{
    bag::sptr bag::make(int game)
    {
        return sptr(new bag_impl(game));
    }

    bag::sptr bag::make(const pkmn::pkstring &game)
    {
        //FRLG and HGSS are stored without spaces
        if(game == "Black 2" or game == "White 2") return make(database::get_version_id(game));
        else
        {
            pkmn::pkstring actual_game = boost::erase_all_copy(game.std_string(), " ");
            return make(database::get_version_id(actual_game));
        }
    }

    pkmn::shared_ptr<SQLite::Database> bag_impl::_db;

    bag_impl::bag_impl(int game): bag()
    {
        CONNECT_TO_DB(_db);

        _game_id = game;
        _generation = database::get_generation(_game_id);
        _pockets = pkmn::dict<pkmn::pkstring, pocket::sptr>();

        switch(game)
        {
            case Versions::RED:
            case Versions::BLUE:
            case Versions::YELLOW:
                _pockets["Items"] = pocket::make(_game_id, "Items", 20);
                break;

            case Versions::GOLD:
            case Versions::SILVER:
            case Versions::CRYSTAL:
                _pockets["Items"] = pocket::make(_game_id, "Items", 20);
                _pockets["Balls"] = pocket::make(_game_id, "Balls", 12);
                _pockets["KeyItems"] = pocket::make(_game_id, "KeyItems", 26);
                _pockets["TM/HM"] = pocket::make(_game_id, "TM/HM", 57);
                break;

            case Versions::RUBY:
            case Versions::SAPPHIRE:
                _pockets["Items"] = pocket::make(_game_id, "Items", 20);
                _pockets["Poke Balls"] = pocket::make(_game_id, "Poke Balls", 16);
                _pockets["TMs and HMs"] = pocket::make(_game_id, "TMs and HMs", 64);
                _pockets["Berries"] = pocket::make(_game_id, "Berries", 46);
                _pockets["Key Items"] = pocket::make(_game_id, "Key Items", 20);
                break;

            case Versions::EMERALD:
                _pockets["Items"] = pocket::make(_game_id, "Items", 30);
                _pockets["Poke Balls"] = pocket::make(_game_id, "Poke Balls", 16);
                _pockets["TMs and HMs"] = pocket::make(_game_id, "TMs and HMs", 64);
                _pockets["Berries"] = pocket::make(_game_id, "Berries", 46);
                _pockets["Key Items"] = pocket::make(_game_id, "Key Items", 30);
                break;

            case Versions::FIRERED:
            case Versions::LEAFGREEN:
                _pockets["Items"] = pocket::make(_game_id, "Items", 42);
                _pockets["Key Items"] = pocket::make(_game_id, "Key Items", 30);
                _pockets["Poke Balls"] = pocket::make(_game_id, "Poke Balls", 13);
                _pockets["TM Case"] = pocket::make(_game_id, "TM Case", 58);
                _pockets["Berry Pouch"] = pocket::make(_game_id, "Berry Pouch", 43);
                break;

            case Versions::DIAMOND:
            case Versions::PEARL:
            case Versions::PLATINUM:
                _pockets["Items"] = pocket::make(_game_id, "Items", 165);
                _pockets["Medicine"] = pocket::make(_game_id, "Medicine", 40);
                _pockets["Poke Balls"] = pocket::make(_game_id, "Poke Balls", 15);
                _pockets["TMs and HMs"] = pocket::make(_game_id, "TMs and HMs", 100);
                _pockets["Berries"] = pocket::make(_game_id, "Berries", 64);
                _pockets["Mail"] = pocket::make(_game_id, "Mail", 12);
                _pockets["Battle Items"] = pocket::make(_game_id, "Battle Items", 30);
                _pockets["Key Items"] = pocket::make(_game_id, "Key Items", 50);
                break;

            case Versions::HEARTGOLD:
            case Versions::SOULSILVER:
                _pockets["Items"] = pocket::make(_game_id, "Items", 165);
                _pockets["Medicine"] = pocket::make(_game_id, "Medicine", 40);
                _pockets["Poke Balls"] = pocket::make(_game_id, "Poke Balls", 24);
                _pockets["TMs and HMs"] = pocket::make(_game_id, "TMs and HMs", 100);
                _pockets["Berries"] = pocket::make(_game_id, "Berries", 64);
                _pockets["Mail"] = pocket::make(_game_id, "Mail", 12);
                _pockets["Battle Items"] = pocket::make(_game_id, "Battle Items", 30);
                _pockets["Key Items"] = pocket::make(_game_id, "Key Items", 43);
                break;

            case Versions::BLACK:
            case Versions::WHITE:
            case Versions::BLACK_2:
            case Versions::WHITE_2:
            case Versions::X:
            case Versions::Y:
                _pockets["Items"] = pocket::make(_game_id, "Items", 310);
                _pockets["Medicine"] = pocket::make(_game_id, "Medicine", 48);
                _pockets["TMs and HMs"] = pocket::make(_game_id, "TMs and HMs", 109);
                _pockets["Berries"] = pocket::make(_game_id, "Berries", 64);
                _pockets["Key Items"] = pocket::make(_game_id, "Key Items", 83);

            default:
                break;
        }
    }

    bag_impl::bag_impl(const bag_impl& other) :
        _game_id(other._game_id),
        _generation(other._generation)
    {
        std::vector<pkmn::pkstring> pocket_names = other._pockets.keys();
        std::vector<pocket::sptr> pockets = other._pockets.vals();

        for(size_t i = 0; i < pocket_names.size(); i++)
        {
            _pockets[pocket_names[i]] = copy_pocket(pockets[i]);
        }
    }

    bag_impl& bag_impl::operator=(const bag_impl& other)
    {
        _game_id = other._game_id;
        _generation = other._generation;

        std::vector<pkmn::pkstring> pocket_names = other._pockets.keys();
        std::vector<pocket::sptr> pockets = other._pockets.vals();

        for(size_t i = 0; i < pocket_names.size(); i++)
        {
            _pockets[pocket_names[i]] = copy_pocket(pockets[i]);
        }

        return *this;
    }

    pkmn::pkstring bag_impl::get_game() const {return database::get_version_name(_game_id);}

    int bag_impl::get_generation() const {return _generation;}

    void bag_impl::add_item(const pkmn::pkstring &item_name, int amount)
    {
        add_item(database::get_item_id(item_name), amount);
    }

    void bag_impl::add_item(int item_id, int amount)
    {
        _pockets[_get_pocket_name(item_id)]->add_item(item_id, amount);
    }

    void bag_impl::remove_item(const pkmn::pkstring &item_name, int amount)
    {
        remove_item(database::get_item_id(item_name), amount);
    }

    void bag_impl::remove_item(int item_id, int amount)
    {
        _pockets[_get_pocket_name(item_id)]->remove_item(item_id, amount);
    }

    int bag_impl::get_item_amount(const pkmn::pkstring &item_name) const
    {
        return get_item_amount(database::get_item_id(item_name));
    }

    int bag_impl::get_item_amount(int item_id) const
    {
        return _pockets[_get_pocket_name(item_id)]->get_item_amount(item_id);
    }

    pocket::sptr bag_impl::get_pocket(const pkmn::pkstring &name) const {return _pockets[name];}

    pkmn::dict<pkmn::pkstring, pocket::sptr> bag_impl::get_pockets() const {return _pockets;}

    int bag_impl::get_game_id() const {return _game_id;}

    //Determine correct pocket for given item
    pkmn::pkstring bag_impl::_get_pocket_name(int item_id) const
    {
        std::ostringstream query_stream;
        query_stream << "SELECT name FROM pocket_names WHERE version_group_id="
                     << database::get_version_group_id(_game_id) << " AND pocket_id="
                     << "(SELECT pocket_id FROM item_categories WHERE id="
                     << database::get_item_category_id(item_id) << ")";

        return (const char*)(_db->execAndGet(query_stream.str().c_str()));
    }
} /* namespace pkmn */
