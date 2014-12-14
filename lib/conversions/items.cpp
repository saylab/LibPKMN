/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifdef _MSC_VER
#pragma warning(disable:4800)
#endif

#include <string>
#include <vector>

#include <pkmn/enums.hpp>
#include <pkmn/bag.hpp>
#include <pkmn/item.hpp>
#include <pkmn/pocket.hpp>
#include <pkmn/database.hpp>

#include "items.hpp"
#include "../library_bridge.hpp"

namespace pkmn
{
    namespace conversions
    {
        void import_gen1_bag(bag::sptr libpkmn_bag, const gen1_item_bag_t* raw_bag)
        {
            pocket::sptr item_pocket = libpkmn_bag->get_pocket("Items");

            for(size_t i = 0; i < raw_bag->count; i++)
            {
                item_pocket->add_item(database::get_item_id(raw_bag->items[i].index, Versions::YELLOW),
                                      raw_bag->items[i].count);
            }
        }

        void export_gen1_bag(bag::sptr libpkmn_bag, gen1_item_bag_t* raw_bag)
        {
            pocket::sptr item_pocket = libpkmn_bag->get_pocket("Items");

            item_list_t item_list;
            item_pocket->get_item_list(item_list);
            raw_bag->count = item_list.size();

            for(size_t i = 0; i < item_list.size(); i++)
            {
                raw_bag->items[i].index = database::get_item_game_index(item_list[i].first->get_item_id(), Versions::YELLOW);
                raw_bag->items[i].count = item_list[i].second;
            }
        }

        void import_gen2_bag(bag::sptr libpkmn_bag, const gen2_item_bag_t* raw_bag)
        {
            pocket::sptr item_pocket = libpkmn_bag->get_pocket("Items");
            pocket::sptr ball_pocket = libpkmn_bag->get_pocket("Balls");
            pocket::sptr keyitem_pocket = libpkmn_bag->get_pocket("KeyItems");
            pocket::sptr tmhm_pocket = libpkmn_bag->get_pocket("TM/HM");

            for(size_t i = 0; i < raw_bag->item_pocket.count; i++)
            {
                item_pocket->add_item(database::get_item_id(raw_bag->item_pocket.items[i].index, Versions::CRYSTAL),
                                      raw_bag->item_pocket.items[i].count);
            }
            for(size_t i = 0; i < raw_bag->ball_pocket.count; i++)
            {
                ball_pocket->add_item(database::get_item_id(raw_bag->ball_pocket.items[i].index, Versions::CRYSTAL),
                                      raw_bag->ball_pocket.items[i].count);
            }
            for(size_t i = 0; i < raw_bag->key_item_pocket.count; i++)
            {
                item_pocket->add_item(database::get_item_id(raw_bag->key_item_pocket.items[i].index, Versions::CRYSTAL),
                                      raw_bag->key_item_pocket.items[i].count);
            }
            for(size_t i = 0; i < 50; i++)
            {
                if(raw_bag->tmhm_pocket.tm[i] > 0)
                {
                    tmhm_pocket->add_item((Items::TM01+i), raw_bag->tmhm_pocket.tm[i]);
                }
            }
            for(size_t i = 0; i < 7; i++)
            {
                if(raw_bag->tmhm_pocket.hm[i] > 0)
                {
                    tmhm_pocket->add_item((Items::HM01+i), raw_bag->tmhm_pocket.hm[i]);
                }
            }
        }

        void export_gen2_bag(bag::sptr libpkmn_bag, gen2_item_bag_t* raw_bag)
        {
            pocket::sptr tmhm_pocket = libpkmn_bag->get_pocket("TM/HM");
            for(size_t i = 0; i < 50; i++)
            {
                raw_bag->tmhm_pocket.tm[i] = tmhm_pocket->get_item_amount((Items::TM01+i));
            }
            for(size_t i = 0; i < 7; i++)
            {
                raw_bag->tmhm_pocket.hm[i] = tmhm_pocket->get_item_amount((Items::HM01+i));
            }

            item_list_t item_list;
            libpkmn_bag->get_pocket("Items")->get_item_list(item_list);
            raw_bag->item_pocket.count = item_list.size();
            for(size_t i = 0; i < item_list.size(); i++)
            {
                raw_bag->item_pocket.items[i].count = item_list[i].second;
                raw_bag->item_pocket.items[i].index = database::get_item_game_index(item_list[i].first->get_item_id(),
                                                                                   Versions::CRYSTAL);
            }

            item_list_t keyitem_list;
            libpkmn_bag->get_pocket("KeyItems")->get_item_list(keyitem_list);
            raw_bag->key_item_pocket.count = keyitem_list.size();
            for(size_t i = 0; i < item_list.size(); i++)
            {
                raw_bag->key_item_pocket.items[i].count = keyitem_list[i].second;
                raw_bag->key_item_pocket.items[i].index = database::get_item_game_index(keyitem_list[i].first->get_item_id(),
                                                                                       Versions::CRYSTAL);
            }

            item_list_t ball_list;
            libpkmn_bag->get_pocket("Balls")->get_item_list(ball_list);
            raw_bag->ball_pocket.count = ball_list.size();
            for(size_t i = 0; i < item_list.size(); i++)
            {
                raw_bag->ball_pocket.items[i].count = ball_list[i].second;
                raw_bag->ball_pocket.items[i].index = database::get_item_game_index(ball_list[i].first->get_item_id(),
                                                                                   Versions::CRYSTAL);
            }
        }

        void import_gen3_bag(bag::sptr libpkmn_bag, const void* raw_bag, const uint16_t security_key)
        {
            bool is_frlg = (libpkmn_bag->get_game_id() == Versions::FIRERED or
                            libpkmn_bag->get_game_id() == Versions::LEAFGREEN);

            pocket::sptr item_pocket = libpkmn_bag->get_pocket("Items");
            pocket::sptr keyitem_pocket = libpkmn_bag->get_pocket("Key Items");
            pocket::sptr ball_pocket = libpkmn_bag->get_pocket("Poke Balls");
            pocket::sptr tmhm_pocket = libpkmn_bag->get_pocket(is_frlg ? "TM Case" : "TMs and HMs");
            pocket::sptr berry_pocket = libpkmn_bag->get_pocket(is_frlg ? "Berry Pouch" : "Berries");

            const modern_item_t *raw_item_pocket, *raw_keyitem_pocket, *raw_ball_pocket, *raw_tmhm_pocket, *raw_berry_pocket;

            switch(libpkmn_bag->get_game_id())
            {
                case Versions::RUBY:
                case Versions::SAPPHIRE:
                    raw_item_pocket = reinterpret_cast<const rs_item_storage_t*>(raw_bag)->items;
                    raw_keyitem_pocket = reinterpret_cast<const rs_item_storage_t*>(raw_bag)->key_items;
                    raw_ball_pocket = reinterpret_cast<const rs_item_storage_t*>(raw_bag)->balls;
                    raw_tmhm_pocket = reinterpret_cast<const rs_item_storage_t*>(raw_bag)->tms_hms;
                    raw_berry_pocket = reinterpret_cast<const rs_item_storage_t*>(raw_bag)->berries;
                    break;

                case Versions::EMERALD:
                    raw_item_pocket = reinterpret_cast<const e_item_storage_t*>(raw_bag)->items;
                    raw_keyitem_pocket = reinterpret_cast<const e_item_storage_t*>(raw_bag)->key_items;
                    raw_ball_pocket = reinterpret_cast<const e_item_storage_t*>(raw_bag)->balls;
                    raw_tmhm_pocket = reinterpret_cast<const e_item_storage_t*>(raw_bag)->tms_hms;
                    raw_berry_pocket = reinterpret_cast<const e_item_storage_t*>(raw_bag)->berries;
                    break;

                case Versions::FIRERED:
                case Versions::LEAFGREEN:
                    raw_item_pocket = reinterpret_cast<const frlg_item_storage_t*>(raw_bag)->items;
                    raw_keyitem_pocket = reinterpret_cast<const frlg_item_storage_t*>(raw_bag)->key_items;
                    raw_ball_pocket = reinterpret_cast<const frlg_item_storage_t*>(raw_bag)->balls;
                    raw_tmhm_pocket = reinterpret_cast<const frlg_item_storage_t*>(raw_bag)->tms_hms;
                    raw_berry_pocket = reinterpret_cast<const frlg_item_storage_t*>(raw_bag)->berries;
                    break;

                default:
                    return;
            }

            for(size_t i = 0; i < item_pocket->size(); i++)
            {
                if(raw_item_pocket[i].index == 0 or raw_item_pocket[i].count == 0) break;
                else item_pocket->add_item(database::get_item_id(raw_item_pocket[i].index,
                                                                 item_pocket->get_game_id()),
                                           (raw_item_pocket[i].count ^ security_key));
            }
            for(size_t i = 0; i < keyitem_pocket->size(); i++)
            {
                if(raw_keyitem_pocket[i].index == 0 or raw_keyitem_pocket[i].count == 0) break;
                else keyitem_pocket->add_item(database::get_item_id(raw_keyitem_pocket[i].index,
                                                                    keyitem_pocket->get_game_id()),
                                              (raw_keyitem_pocket[i].count ^ security_key));
            }
            for(size_t i = 0; i < ball_pocket->size(); i++)
            {
                if(raw_ball_pocket[i].index == 0 or raw_ball_pocket[i].count == 0) break;
                else ball_pocket->add_item(database::get_item_id(raw_ball_pocket[i].index,
                                                                 ball_pocket->get_game_id()),
                                           (raw_ball_pocket[i].count ^ security_key));
            }
            for(size_t i = 0; i < tmhm_pocket->size(); i++)
            {
                if(raw_tmhm_pocket[i].index == 0 or raw_tmhm_pocket[i].count == 0) break;
                else tmhm_pocket->add_item(database::get_item_id(raw_tmhm_pocket[i].index,
                                                                 tmhm_pocket->get_game_id()),
                                           (raw_tmhm_pocket[i].count ^ security_key));
            }
            for(size_t i = 0; i < berry_pocket->size(); i++)
            {
                if(raw_berry_pocket[i].index == 0 or raw_berry_pocket[i].count == 0) break;
                else berry_pocket->add_item(database::get_item_id(raw_berry_pocket[i].index,
                                                                  berry_pocket->get_game_id()),
                                           (raw_berry_pocket[i].count ^ security_key));
            }
        }

        void export_gen3_bag(bag::sptr libpkmn_bag, void* raw_bag, const uint16_t security_key)
        {
            bool is_frlg = (libpkmn_bag->get_game_id() == Versions::FIRERED or
                            libpkmn_bag->get_game_id() == Versions::LEAFGREEN);

            modern_item_t *raw_item_pocket, *raw_keyitem_pocket, *raw_ball_pocket, *raw_tmhm_pocket, *raw_berry_pocket;

            switch(libpkmn_bag->get_game_id())
            {
                case Versions::RUBY:
                case Versions::SAPPHIRE:
                    raw_item_pocket = reinterpret_cast<rs_item_storage_t*>(raw_bag)->items;
                    raw_keyitem_pocket = reinterpret_cast<rs_item_storage_t*>(raw_bag)->key_items;
                    raw_ball_pocket = reinterpret_cast<rs_item_storage_t*>(raw_bag)->balls;
                    raw_tmhm_pocket = reinterpret_cast<rs_item_storage_t*>(raw_bag)->tms_hms;
                    raw_berry_pocket = reinterpret_cast<rs_item_storage_t*>(raw_bag)->berries;
                    break;

                case Versions::EMERALD:
                    raw_item_pocket = reinterpret_cast<e_item_storage_t*>(raw_bag)->items;
                    raw_keyitem_pocket = reinterpret_cast<e_item_storage_t*>(raw_bag)->key_items;
                    raw_ball_pocket = reinterpret_cast<e_item_storage_t*>(raw_bag)->balls;
                    raw_tmhm_pocket = reinterpret_cast<e_item_storage_t*>(raw_bag)->tms_hms;
                    raw_berry_pocket = reinterpret_cast<e_item_storage_t*>(raw_bag)->berries;
                    break;

                case Versions::FIRERED:
                case Versions::LEAFGREEN:
                    raw_item_pocket = reinterpret_cast<frlg_item_storage_t*>(raw_bag)->items;
                    raw_keyitem_pocket = reinterpret_cast<frlg_item_storage_t*>(raw_bag)->key_items;
                    raw_ball_pocket = reinterpret_cast<frlg_item_storage_t*>(raw_bag)->balls;
                    raw_tmhm_pocket = reinterpret_cast<frlg_item_storage_t*>(raw_bag)->tms_hms;
                    raw_berry_pocket = reinterpret_cast<frlg_item_storage_t*>(raw_bag)->berries;
                    break;

                default:
                    return;
            }

            item_list_t item_list;
            libpkmn_bag->get_pocket("Items")->get_item_list(item_list);
            for(size_t i = 0; i < item_list.size(); i++)
            {
                raw_item_pocket[i].index = database::get_item_game_index(item_list[i].first->get_item_id(),
                                                                         libpkmn_bag->get_game_id());
                raw_item_pocket[i].count = item_list[i].second ^ security_key;
            }

            item_list_t keyitem_list;
            libpkmn_bag->get_pocket("Key Items")->get_item_list(keyitem_list);
            for(size_t i = 0; i < keyitem_list.size(); i++)
            {
                raw_keyitem_pocket[i].index = database::get_item_game_index(keyitem_list[i].first->get_item_id(),
                                                                            libpkmn_bag->get_game_id());
                raw_keyitem_pocket[i].count = keyitem_list[i].second ^ security_key;
            }

            item_list_t ball_list;
            libpkmn_bag->get_pocket("Poke Balls")->get_item_list(ball_list);
            for(size_t i = 0; i < ball_list.size(); i++)
            {
                raw_ball_pocket[i].index = database::get_item_game_index(ball_list[i].first->get_item_id(),
                                                                         libpkmn_bag->get_game_id());
                raw_ball_pocket[i].count = ball_list[i].second ^ security_key;
            }

            item_list_t tmhm_list;
            libpkmn_bag->get_pocket(is_frlg ? "TM Case" : "TMs and HMs")->get_item_list(tmhm_list);
            for(size_t i = 0; i < item_list.size(); i++)
            {
                raw_tmhm_pocket[i].index = database::get_item_game_index(tmhm_list[i].first->get_item_id(),
                                                                         libpkmn_bag->get_game_id());
                raw_tmhm_pocket[i].count = tmhm_list[i].second ^ security_key;
            }
        }

        void import_gen4_items(bag::sptr item_bag, PokeLib::Trainer pokelib_trainer)
        {
            unsigned int game_id = item_bag->get_game_id();

            pocket::sptr item_pocket = item_bag->get_pocket("Items");
            pocket::sptr medicine_pocket = item_bag->get_pocket("Medicine");
            pocket::sptr ball_pocket = item_bag->get_pocket("Poke Balls");
            pocket::sptr tm_pocket = item_bag->get_pocket("TMs and HMs");
            pocket::sptr berry_pocket = item_bag->get_pocket("Berries");
            pocket::sptr mail_pocket = item_bag->get_pocket("Mail");
            pocket::sptr battle_item_pocket = item_bag->get_pocket("Battle Items");
            pocket::sptr key_item_pocket = item_bag->get_pocket("Key Items");

            for(size_t i = 0; i < item_pocket->size(); i++)
            {
                PokeLib::BagItem pokelib_bagitem = pokelib_trainer.getItem(PokeLib::ItemGeneral, i+1);

                if(pokelib_bagitem.item == 0 or pokelib_bagitem.item > 536) break;
                else item_pocket->add_item(database::get_item_id(pokelib_bagitem.item, game_id),
                                           pokelib_bagitem.count);
            }
            for(size_t i = 0; i < medicine_pocket->size(); i++)
            {
                PokeLib::BagItem pokelib_bagitem = pokelib_trainer.getItem(PokeLib::ItemMedicine, i+1);

                if(pokelib_bagitem.item == 0 or pokelib_bagitem.item > 536) break;
                else medicine_pocket->add_item(database::get_item_id(pokelib_bagitem.item, game_id),
                                               pokelib_bagitem.count);
            }
            for(size_t i = 0; i < ball_pocket->size(); i++)
            {
                PokeLib::BagItem pokelib_bagitem = pokelib_trainer.getItem(PokeLib::ItemBall, i+1);

                if(pokelib_bagitem.item == 0 or pokelib_bagitem.item > 536) break;
                else ball_pocket->add_item(database::get_item_id(pokelib_bagitem.item, game_id),
                                           pokelib_bagitem.count);
            }
            for(size_t i = 0; i < tm_pocket->size(); i++)
            {
                PokeLib::BagItem pokelib_bagitem = pokelib_trainer.getItem(PokeLib::ItemTM, i+1);

                if(pokelib_bagitem.item == 0 or pokelib_bagitem.item > 536) break;
                else tm_pocket->add_item(database::get_item_id(pokelib_bagitem.item, game_id),
                                         pokelib_bagitem.count);
            }
            for(size_t i = 0; i < berry_pocket->size(); i++)
            {
                PokeLib::BagItem pokelib_bagitem = pokelib_trainer.getItem(PokeLib::ItemBerry, i+1);

                if(pokelib_bagitem.item == 0 or pokelib_bagitem.item > 536) break;
                else berry_pocket->add_item(database::get_item_id(pokelib_bagitem.item, game_id),
                                            pokelib_bagitem.count);
            }
            for(size_t i = 0; i < mail_pocket->size(); i++)
            {
                PokeLib::BagItem pokelib_bagitem = pokelib_trainer.getItem(PokeLib::ItemMail, i+1);

                if(pokelib_bagitem.item == 0 or pokelib_bagitem.item > 536) break;
                else mail_pocket->add_item(database::get_item_id(pokelib_bagitem.item, game_id),
                                           pokelib_bagitem.count);
            }
            for(size_t i = 0; i < battle_item_pocket->size(); i++)
            {
                PokeLib::BagItem pokelib_bagitem = pokelib_trainer.getItem(PokeLib::ItemBattle, i+1);

                if(pokelib_bagitem.item == 0 or pokelib_bagitem.item > 536) break;
                else battle_item_pocket->add_item(database::get_item_id(pokelib_bagitem.item, game_id),
                                                  pokelib_bagitem.count);
            }
            for(size_t i = 0; i < key_item_pocket->size(); i++)
            {
                PokeLib::BagItem pokelib_bagitem = pokelib_trainer.getItem(PokeLib::ItemKey, i+1);

                if(pokelib_bagitem.item == 0 or pokelib_bagitem.item > 536) break;
                else key_item_pocket->add_item(database::get_item_id(pokelib_bagitem.item, game_id),
                                               pokelib_bagitem.count);
            }
        }

        void export_gen4_items(bag::sptr item_bag, PokeLib::Trainer* pokelib_trainer)
        {
            unsigned int game_id = item_bag->get_game_id();

            pocket::sptr item_pocket = item_bag->get_pocket("Items");
            pocket::sptr medicine_pocket = item_bag->get_pocket("Medicine");
            pocket::sptr ball_pocket = item_bag->get_pocket("Poke Balls");
            pocket::sptr tm_pocket = item_bag->get_pocket("TMs and HMs");
            pocket::sptr berry_pocket = item_bag->get_pocket("Berries");
            pocket::sptr mail_pocket = item_bag->get_pocket("Mail");
            pocket::sptr battle_item_pocket = item_bag->get_pocket("Battle Items");
            pocket::sptr key_item_pocket = item_bag->get_pocket("Key Items");

            item_list_t item_list;
            item_pocket->get_item_list(item_list);
            for(size_t i = 0; i < item_list.size(); i++)
            {
                unsigned int item_id = item_list[i].first->get_item_id();
                unsigned int amount = item_list[i].second;

                if(item_id == Items::NONE) break;
                else
                {
                    PokeLib::BagItem pokelib_bagitem;
                    pokelib_bagitem.item = database::get_item_game_index(item_id, game_id);
                    pokelib_bagitem.count = amount;

                    pokelib_trainer->setItem(PokeLib::ItemGeneral, (i+1), pokelib_bagitem);
                }
            }

            item_list_t medicine_list;
            medicine_pocket->get_item_list(medicine_list);
            for(size_t i = 0; i < medicine_list.size(); i++)
            {
                unsigned int item_id = medicine_list[i].first->get_item_id();
                unsigned int amount = medicine_list[i].second;

                if(item_id == Items::NONE) break;
                else
                {
                    PokeLib::BagItem pokelib_bagitem;
                    pokelib_bagitem.item = database::get_item_game_index(item_id, game_id);
                    pokelib_bagitem.count = amount;

                    pokelib_trainer->setItem(PokeLib::ItemMedicine, (i+1), pokelib_bagitem);
                }
            }

            item_list_t ball_list;
            ball_pocket->get_item_list(ball_list);
            for(size_t i = 0; i < ball_list.size(); i++)
            {
                unsigned int item_id = ball_list[i].first->get_item_id();
                unsigned int amount = ball_list[i].second;

                if(item_id == Items::NONE) break;
                else
                {
                    PokeLib::BagItem pokelib_bagitem;
                    pokelib_bagitem.item = database::get_item_game_index(item_id, game_id);
                    pokelib_bagitem.count = amount;

                    pokelib_trainer->setItem(PokeLib::ItemBall, (i+1), pokelib_bagitem);
                }
            }

            item_list_t tm_list;
            tm_pocket->get_item_list(tm_list);
            for(size_t i = 0; i < tm_list.size(); i++)
            {
                unsigned int item_id = tm_list[i].first->get_item_id();
                unsigned int amount = tm_list[i].second;

                if(item_id == Items::NONE) break;
                else
                {
                    PokeLib::BagItem pokelib_bagitem;
                    pokelib_bagitem.item = database::get_item_game_index(item_id, game_id);
                    pokelib_bagitem.count = amount;

                    pokelib_trainer->setItem(PokeLib::ItemTM, (i+1), pokelib_bagitem);
                }
            }

            item_list_t berry_list;
            berry_pocket->get_item_list(berry_list);
            for(size_t i = 0; i < berry_list.size(); i++)
            {
                unsigned int item_id = berry_list[i].first->get_item_id();
                unsigned int amount = berry_list[i].second;

                if(item_id == Items::NONE) break;
                else
                {
                    PokeLib::BagItem pokelib_bagitem;
                    pokelib_bagitem.item = database::get_item_game_index(item_id, game_id);
                    pokelib_bagitem.count = amount;

                    pokelib_trainer->setItem(PokeLib::ItemBerry, (i+1), pokelib_bagitem);
                }
            }

            item_list_t mail_list;
            mail_pocket->get_item_list(mail_list);
            for(size_t i = 0; i < mail_list.size(); i++)
            {
                unsigned int item_id = mail_list[i].first->get_item_id();
                unsigned int amount = mail_list[i].second;

                if(item_id == Items::NONE) break;
                else
                {
                    PokeLib::BagItem pokelib_bagitem;
                    pokelib_bagitem.item = database::get_item_game_index(item_id, game_id);
                    pokelib_bagitem.count = amount;

                    pokelib_trainer->setItem(PokeLib::ItemMail, (i+1), pokelib_bagitem);
                }
            }

            item_list_t battle_item_list;
            battle_item_pocket->get_item_list(battle_item_list);
            for(size_t i = 0; i < battle_item_list.size(); i++)
            {
                unsigned int item_id = battle_item_list[i].first->get_item_id();
                unsigned int amount = battle_item_list[i].second;

                if(item_id == Items::NONE) break;
                else
                {
                    PokeLib::BagItem pokelib_bagitem;
                    pokelib_bagitem.item = database::get_item_game_index(item_id, game_id);
                    pokelib_bagitem.count = amount;

                    pokelib_trainer->setItem(PokeLib::ItemBattle, (i+1), pokelib_bagitem);
                }
            }

            item_list_t key_item_list;
            item_pocket->get_item_list(key_item_list);
            for(size_t i = 0; i < key_item_list.size(); i++)
            {
                unsigned int item_id = key_item_list[i].first->get_item_id();
                unsigned int amount = key_item_list[i].second;

                if(item_id == Items::NONE) break;
                else
                {
                    PokeLib::BagItem pokelib_bagitem;
                    pokelib_bagitem.item = database::get_item_game_index(item_id, game_id);
                    pokelib_bagitem.count = amount;

                    pokelib_trainer->setItem(PokeLib::ItemKey, (i+1), pokelib_bagitem);
                }
            }
        }

        void import_gen5_items(bag::sptr item_bag, ::bag_obj* pkmds_bag)
        {
            unsigned int game_id = item_bag->get_game_id();

            pocket::sptr item_pocket = item_bag->get_pocket("Items");
            pocket::sptr medicine_pocket = item_bag->get_pocket("Medicine");
            pocket::sptr tm_pocket = item_bag->get_pocket("TMs and HMs");
            pocket::sptr berry_pocket = item_bag->get_pocket("Berries");
            pocket::sptr key_item_pocket = item_bag->get_pocket("Key Items");

            for(size_t i = 0; i < ITEMS_POCKET_SIZE; i++)
            {
                ::item_obj pkmds_item = pkmds_bag->items_pocket[i];

                if(pkmds_item.id == ::Items::NOTHING) break;
                else item_pocket->add_item(database::get_item_id(pkmds_item.id, game_id),
                                           pkmds_item.quantity);
            }
            for(size_t i = 0; i < MEDICINE_POCKET_SIZE; i++)
            {
                ::item_obj pkmds_item = pkmds_bag->medicine_pocket[i];

                if(pkmds_item.id == ::Items::NOTHING) break;
                else medicine_pocket->add_item(item::make(pkmds_item.id, game_id),
                                               pkmds_item.quantity);
            }
            for(size_t i = 0; i < TMS_POCKET_SIZE; i++)
            {
                ::item_obj pkmds_item = pkmds_bag->tms_pocket[i];

                if(pkmds_item.id == ::Items::NOTHING) break;
                else tm_pocket->add_item(database::get_item_id(pkmds_item.id, game_id),
                                         pkmds_item.quantity);
            }
            for(size_t i = 0; i < BERRIES_POCKET_SIZE; i++)
            {
                ::item_obj pkmds_item = pkmds_bag->berries_pocket[i];

                if(pkmds_item.id == ::Items::NOTHING) break;
                else berry_pocket->add_item(database::get_item_id(pkmds_item.id, game_id),
                                            pkmds_item.quantity);
            }
            for(size_t i = 0; i < KEYITEMS_POCKET_SIZE; i++)
            {
                ::item_obj pkmds_item = pkmds_bag->keyitems_pocket[i];

                //Get around "Dropped Item" issue
                if(pkmds_item.id == ::Items::droppeditem2) pkmds_item.id = ::Items::droppeditem;

                if(pkmds_item.id == ::Items::NOTHING) break;
                else key_item_pocket->add_item(database::get_item_id(pkmds_item.id, game_id),
                                               1);
            }
        }

        void export_gen5_items(bag::sptr item_bag, ::bag_obj* pkmds_bag)
        {
            unsigned int game_id = item_bag->get_game_id();

            pocket::sptr item_pocket = item_bag->get_pocket("Items");
            pocket::sptr medicine_pocket = item_bag->get_pocket("Medicine");
            pocket::sptr tm_pocket = item_bag->get_pocket("TMs and HMs");
            pocket::sptr berry_pocket = item_bag->get_pocket("Berries");
            pocket::sptr key_item_pocket = item_bag->get_pocket("Key Items");

            item_list_t item_list;
            item_pocket->get_item_list(item_list);
            for(size_t i = 0; i < item_list.size(); i++)
            {
                unsigned int item_id = item_list[i].first->get_game_index();
                unsigned int amount = item_list[i].second;

                if(item_id == Items::NONE) break;
                else
                {
                    pkmds_bag->items_pocket[i].id = ::Items::items(item_id);
                    pkmds_bag->items_pocket[i].quantity = amount;
                }
            }

            item_list_t medicine_list;
            medicine_pocket->get_item_list(medicine_list);
            for(size_t i = 0; i < medicine_list.size(); i++)
            {
                unsigned int item_id = medicine_list[i].first->get_game_index();
                unsigned int amount = medicine_list[i].second;

                if(item_id == Items::NONE) break;
                else
                {
                    pkmds_bag->medicine_pocket[i].id = ::Items::items(item_id);
                    pkmds_bag->medicine_pocket[i].quantity = amount;
                }
            }

            item_list_t tm_list;
            tm_pocket->get_item_list(tm_list);
            for(size_t i = 0; i < tm_pocket->size(); i++)
            {
                unsigned int item_id = tm_list[i].first->get_game_index();
                unsigned int amount = tm_list[i].second;

                if(item_id == Items::NONE) break;
                else
                {
                    pkmds_bag->tms_pocket[i].id = ::Items::items(item_id);
                    pkmds_bag->tms_pocket[i].quantity = amount;
                }
            }

            item_list_t berry_list;
            berry_pocket->get_item_list(berry_list);
            for(size_t i = 0; i < berry_pocket->size(); i++)
            {
                unsigned int item_id = berry_list[i].first->get_game_index();
                unsigned int amount = berry_list[i].second;

                if(item_id == Items::NONE) break;
                else
                {
                    pkmds_bag->berries_pocket[i].id = ::Items::items(item_id);
                    pkmds_bag->berries_pocket[i].quantity = amount;
                }
            }

            item_list_t key_item_list;
            key_item_pocket->get_item_list(key_item_list);
            for(size_t i = 0; i < key_item_pocket->size(); i++)
            {
                //TODO: manually set dropped item based on gender

                unsigned int item_id = key_item_list[i].first->get_game_index();
                unsigned int amount = key_item_list[i].second;

                if(item_id == Items::NONE) break;
                else
                {
                    pkmds_bag->keyitems_pocket[i].id = ::Items::items(item_id);
                    pkmds_bag->keyitems_pocket[i].quantity = 1;
                }
            }
        }
    } /* namespace conversions */
} /* namespace pkmn */
