/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
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
#include <pkmn/pocket.hpp>
#include <pkmn/database.hpp>
#include <pkmn/conversions/items.hpp>

#define ITEM_GAME_INDEX(list,i,version) (database::get_item_game_index(database::get_item_id(list[i].first.name),version))

namespace pkmn
{
    namespace conversions
    {
        void import_gen1_bag(bag::sptr libpkmn_bag, const native::gen1_item_bag_t* raw_bag)
        {
            pocket::sptr item_pocket = libpkmn_bag->get_pocket("Items");

            for(size_t i = 0; i < raw_bag->count; i++)
            {
                item_pocket->set_item_amount(database::get_item_id(raw_bag->items[i].index, Versions::YELLOW),
                                             raw_bag->items[i].count);
            }
        }

        void export_gen1_bag(bag::sptr libpkmn_bag, native::gen1_item_bag_t* raw_bag)
        {
            pocket::sptr item_pocket = libpkmn_bag->get_pocket("Items");

            item_list_t item_list = item_pocket->get_item_list();
            raw_bag->count = item_list.size();

            for(size_t i = 0; i < item_list.size(); i++)
            {
                raw_bag->items[i].index = uint8_t(ITEM_GAME_INDEX(item_list, i, Versions::YELLOW));
                raw_bag->items[i].count = uint8_t(item_list[i].second);
            }
        }

        void import_gen2_bag(bag::sptr libpkmn_bag, const native::gen2_item_bag_t* raw_bag)
        {
            pocket::sptr item_pocket    = libpkmn_bag->get_pocket("Items");
            pocket::sptr ball_pocket    = libpkmn_bag->get_pocket("Balls");
            pocket::sptr keyitem_pocket = libpkmn_bag->get_pocket("KeyItems");
            pocket::sptr tmhm_pocket    = libpkmn_bag->get_pocket("TM/HM");

            for(size_t i = 0; i < raw_bag->item_pocket.count; i++)
            {
                item_pocket->set_item_amount(database::get_item_id(raw_bag->item_pocket.items[i].index, Versions::CRYSTAL),
                                             raw_bag->item_pocket.items[i].count);
            }
            for(size_t i = 0; i < raw_bag->ball_pocket.count; i++)
            {
                ball_pocket->set_item_amount(database::get_item_id(raw_bag->ball_pocket.items[i].index, Versions::CRYSTAL),
                                             raw_bag->ball_pocket.items[i].count);
            }
            for(size_t i = 0; i < raw_bag->key_item_pocket.count; i++)
            {
                item_pocket->set_item_amount(database::get_item_id(raw_bag->key_item_pocket.items[i].index, Versions::CRYSTAL),
                                             raw_bag->key_item_pocket.items[i].count);
            }
            for(size_t i = 0; i < 50; i++)
            {
                if(raw_bag->tmhm_pocket.tm[i] > 0)
                {
                    tmhm_pocket->set_item_amount((Items::TM01+i), raw_bag->tmhm_pocket.tm[i]);
                }
            }
            for(size_t i = 0; i < 7; i++)
            {
                if(raw_bag->tmhm_pocket.hm[i] > 0)
                {
                    tmhm_pocket->set_item_amount((Items::HM01+i), raw_bag->tmhm_pocket.hm[i]);
                }
            }
        }

        void export_gen2_bag(bag::sptr libpkmn_bag, native::gen2_item_bag_t* raw_bag)
        {
            pocket::sptr tmhm_pocket = libpkmn_bag->get_pocket("TM/HM");
            for(size_t i = 0; i < 50; i++)
            {
                raw_bag->tmhm_pocket.tm[i] = uint8_t(tmhm_pocket->get_item_amount((Items::TM01+i)));
            }
            for(size_t i = 0; i < 7; i++)
            {
                raw_bag->tmhm_pocket.hm[i] = uint8_t(tmhm_pocket->get_item_amount((Items::HM01+i)));
            }

            item_list_t item_list = libpkmn_bag->get_pocket("Items")->get_item_list();
            raw_bag->item_pocket.count = item_list.size();
            for(size_t i = 0; i < item_list.size(); i++)
            {
                raw_bag->item_pocket.items[i].count = uint8_t(item_list[i].second);
                raw_bag->item_pocket.items[i].index = uint8_t(ITEM_GAME_INDEX(item_list, i, Versions::CRYSTAL));
            }

            item_list = libpkmn_bag->get_pocket("KeyItems")->get_item_list();
            raw_bag->key_item_pocket.count = item_list.size();
            for(size_t i = 0; i < item_list.size(); i++)
            {
                raw_bag->key_item_pocket.items[i].count = uint8_t(item_list[i].second);
                raw_bag->key_item_pocket.items[i].index = uint8_t(ITEM_GAME_INDEX(item_list, i, Versions::CRYSTAL));
            }

            item_list = libpkmn_bag->get_pocket("Balls")->get_item_list();
            raw_bag->ball_pocket.count = item_list.size();
            for(size_t i = 0; i < item_list.size(); i++)
            {
                raw_bag->ball_pocket.items[i].count = uint8_t(item_list[i].second);
                raw_bag->ball_pocket.items[i].index = uint8_t(ITEM_GAME_INDEX(item_list, i, Versions::CRYSTAL));
            }
        }

        void import_gen3_bag(bag::sptr libpkmn_bag, const void* raw_bag, const uint16_t security_key)
        {
            bool is_frlg = (libpkmn_bag->get_game_id() == Versions::FIRERED or
                            libpkmn_bag->get_game_id() == Versions::LEAFGREEN);

            pocket::sptr item_pocket    = libpkmn_bag->get_pocket("Items");
            pocket::sptr keyitem_pocket = libpkmn_bag->get_pocket("Key Items");
            pocket::sptr ball_pocket    = libpkmn_bag->get_pocket("Poke Balls");
            pocket::sptr tmhm_pocket    = libpkmn_bag->get_pocket(is_frlg ? "TM Case" : "TMs and HMs");
            pocket::sptr berry_pocket   = libpkmn_bag->get_pocket(is_frlg ? "Berry Pouch" : "Berries");

            const native::modern_item_t *raw_item_pocket, *raw_keyitem_pocket, *raw_ball_pocket,
                                        *raw_tmhm_pocket, *raw_berry_pocket;

            switch(libpkmn_bag->get_game_id())
            {
                case Versions::RUBY:
                case Versions::SAPPHIRE:
                    raw_item_pocket = reinterpret_cast<const native::rs_item_storage_t*>(raw_bag)->items;
                    raw_keyitem_pocket = reinterpret_cast<const native::rs_item_storage_t*>(raw_bag)->key_items;
                    raw_ball_pocket = reinterpret_cast<const native::rs_item_storage_t*>(raw_bag)->balls;
                    raw_tmhm_pocket = reinterpret_cast<const native::rs_item_storage_t*>(raw_bag)->tms_hms;
                    raw_berry_pocket = reinterpret_cast<const native::rs_item_storage_t*>(raw_bag)->berries;
                    break;

                case Versions::EMERALD:
                    raw_item_pocket = reinterpret_cast<const native::e_item_storage_t*>(raw_bag)->items;
                    raw_keyitem_pocket = reinterpret_cast<const native::e_item_storage_t*>(raw_bag)->key_items;
                    raw_ball_pocket = reinterpret_cast<const native::e_item_storage_t*>(raw_bag)->balls;
                    raw_tmhm_pocket = reinterpret_cast<const native::e_item_storage_t*>(raw_bag)->tms_hms;
                    raw_berry_pocket = reinterpret_cast<const native::e_item_storage_t*>(raw_bag)->berries;
                    break;

                case Versions::FIRERED:
                case Versions::LEAFGREEN:
                    raw_item_pocket = reinterpret_cast<const native::frlg_item_storage_t*>(raw_bag)->items;
                    raw_keyitem_pocket = reinterpret_cast<const native::frlg_item_storage_t*>(raw_bag)->key_items;
                    raw_ball_pocket = reinterpret_cast<const native::frlg_item_storage_t*>(raw_bag)->balls;
                    raw_tmhm_pocket = reinterpret_cast<const native::frlg_item_storage_t*>(raw_bag)->tms_hms;
                    raw_berry_pocket = reinterpret_cast<const native::frlg_item_storage_t*>(raw_bag)->berries;
                    break;

                default:
                    return;
            }

            for(size_t i = 0; i < item_pocket->get_size(); i++)
            {
                if(raw_item_pocket[i].index == 0 or raw_item_pocket[i].count == 0) break;
                else item_pocket->set_item_amount(database::get_item_id(raw_item_pocket[i].index,
                                                                        item_pocket->get_game_id()),
                                                  (raw_item_pocket[i].count ^ security_key));
            }
            for(size_t i = 0; i < keyitem_pocket->get_size(); i++)
            {
                if(raw_keyitem_pocket[i].index == 0 or raw_keyitem_pocket[i].count == 0) break;
                else keyitem_pocket->set_item_amount(database::get_item_id(raw_keyitem_pocket[i].index,
                                                                           keyitem_pocket->get_game_id()),
                                                     (raw_keyitem_pocket[i].count ^ security_key));
            }
            for(size_t i = 0; i < ball_pocket->get_size(); i++)
            {
                if(raw_ball_pocket[i].index == 0 or raw_ball_pocket[i].count == 0) break;
                else ball_pocket->set_item_amount(database::get_item_id(raw_ball_pocket[i].index,
                                                                        ball_pocket->get_game_id()),
                                                  (raw_ball_pocket[i].count ^ security_key));
            }
            for(size_t i = 0; i < tmhm_pocket->get_size(); i++)
            {
                if(raw_tmhm_pocket[i].index == 0 or raw_tmhm_pocket[i].count == 0) break;
                else tmhm_pocket->set_item_amount(database::get_item_id(raw_tmhm_pocket[i].index,
                                                                        tmhm_pocket->get_game_id()),
                                                  (raw_tmhm_pocket[i].count ^ security_key));
            }
            for(size_t i = 0; i < berry_pocket->get_size(); i++)
            {
                if(raw_berry_pocket[i].index == 0 or raw_berry_pocket[i].count == 0) break;
                else berry_pocket->set_item_amount(database::get_item_id(raw_berry_pocket[i].index,
                                                                         berry_pocket->get_game_id()),
                                                   (raw_berry_pocket[i].count ^ security_key));
            }
        }

        void export_gen3_bag(bag::sptr libpkmn_bag, void* raw_bag, const uint16_t security_key)
        {
            uint16_t version_id = libpkmn_bag->get_game_id();
            bool is_frlg = (libpkmn_bag->get_game_id() == Versions::FIRERED or
                            libpkmn_bag->get_game_id() == Versions::LEAFGREEN);

            native::modern_item_t *raw_item_pocket, *raw_keyitem_pocket, *raw_ball_pocket,
                                  *raw_tmhm_pocket, *raw_berry_pocket;

            switch(libpkmn_bag->get_game_id())
            {
                case Versions::RUBY:
                case Versions::SAPPHIRE:
                    raw_item_pocket    = reinterpret_cast<native::rs_item_storage_t*>(raw_bag)->items;
                    raw_keyitem_pocket = reinterpret_cast<native::rs_item_storage_t*>(raw_bag)->key_items;
                    raw_ball_pocket    = reinterpret_cast<native::rs_item_storage_t*>(raw_bag)->balls;
                    raw_tmhm_pocket    = reinterpret_cast<native::rs_item_storage_t*>(raw_bag)->tms_hms;
                    raw_berry_pocket   = reinterpret_cast<native::rs_item_storage_t*>(raw_bag)->berries;
                    break;

                case Versions::EMERALD:
                    raw_item_pocket    = reinterpret_cast<native::e_item_storage_t*>(raw_bag)->items;
                    raw_keyitem_pocket = reinterpret_cast<native::e_item_storage_t*>(raw_bag)->key_items;
                    raw_ball_pocket    = reinterpret_cast<native::e_item_storage_t*>(raw_bag)->balls;
                    raw_tmhm_pocket    = reinterpret_cast<native::e_item_storage_t*>(raw_bag)->tms_hms;
                    raw_berry_pocket   = reinterpret_cast<native::e_item_storage_t*>(raw_bag)->berries;
                    break;

                case Versions::FIRERED:
                case Versions::LEAFGREEN:
                    raw_item_pocket    = reinterpret_cast<native::frlg_item_storage_t*>(raw_bag)->items;
                    raw_keyitem_pocket = reinterpret_cast<native::frlg_item_storage_t*>(raw_bag)->key_items;
                    raw_ball_pocket    = reinterpret_cast<native::frlg_item_storage_t*>(raw_bag)->balls;
                    raw_tmhm_pocket    = reinterpret_cast<native::frlg_item_storage_t*>(raw_bag)->tms_hms;
                    raw_berry_pocket   = reinterpret_cast<native::frlg_item_storage_t*>(raw_bag)->berries;
                    break;

                default:
                    return;
            }

            item_list_t item_list = libpkmn_bag->get_pocket("Items")->get_item_list();
            for(size_t i = 0; i < item_list.size(); i++)
            {
                raw_item_pocket[i].index = ITEM_GAME_INDEX(item_list, i, version_id);
                raw_item_pocket[i].count = item_list[i].second ^ security_key;
            }

            item_list = libpkmn_bag->get_pocket("Key Items")->get_item_list();
            for(size_t i = 0; i < item_list.size(); i++)
            {
                raw_keyitem_pocket[i].index = ITEM_GAME_INDEX(item_list, i, version_id);
                raw_keyitem_pocket[i].count = item_list[i].second ^ security_key;
            }

            item_list = libpkmn_bag->get_pocket("Poke Balls")->get_item_list();
            for(size_t i = 0; i < item_list.size(); i++)
            {
                raw_ball_pocket[i].index = ITEM_GAME_INDEX(item_list, i, version_id);
                raw_ball_pocket[i].count = item_list[i].second ^ security_key;
            }

            item_list = libpkmn_bag->get_pocket(is_frlg ? "TM Case" : "TMs and HMs")->get_item_list();
            for(size_t i = 0; i < item_list.size(); i++)
            {
                raw_tmhm_pocket[i].index = ITEM_GAME_INDEX(item_list, i, version_id);
                raw_tmhm_pocket[i].count = item_list[i].second ^ security_key;
            }
        }

        #define CONST_RAW_NDS_POCKET(field) (is_hgss ? reinterpret_cast<const native::hgss_item_storage_t*>(raw_bag)->field \
                                                     : reinterpret_cast<const native::dppt_item_storage_t*>(raw_bag)->field);

        void import_gen4_bag(bag::sptr libpkmn_bag, const void* raw_bag)
        {
            int version_id = libpkmn_bag->get_game_id();
            bool is_hgss = (libpkmn_bag->get_game_id() == Versions::HEARTGOLD or
                            libpkmn_bag->get_game_id() == Versions::SOULSILVER);

            // LibPKMN pockets
            pocket::sptr item_pocket       = libpkmn_bag->get_pocket("Items");
            pocket::sptr medicine_pocket   = libpkmn_bag->get_pocket("Medicine");
            pocket::sptr ball_pocket       = libpkmn_bag->get_pocket("Poke Balls");
            pocket::sptr tmhm_pocket       = libpkmn_bag->get_pocket("TMs and HMs");
            pocket::sptr berry_pocket      = libpkmn_bag->get_pocket("Berries");
            pocket::sptr mail_pocket       = libpkmn_bag->get_pocket("Mail");
            pocket::sptr battleitem_pocket = libpkmn_bag->get_pocket("Battle Items");
            pocket::sptr keyitem_pocket    = libpkmn_bag->get_pocket("Key Items");

            // Native pockets
            const native::modern_item_t* raw_item_pocket       = CONST_RAW_NDS_POCKET(items);
            const native::modern_item_t* raw_medicine_pocket   = CONST_RAW_NDS_POCKET(key_items);
            const native::modern_item_t* raw_ball_pocket       = CONST_RAW_NDS_POCKET(balls);
            const native::modern_item_t* raw_tmhm_pocket       = CONST_RAW_NDS_POCKET(tms_hms);
            const native::modern_item_t* raw_berry_pocket      = CONST_RAW_NDS_POCKET(berries);
            const native::modern_item_t* raw_mail_pocket       = CONST_RAW_NDS_POCKET(mail);
            const native::modern_item_t* raw_battleitem_pocket = CONST_RAW_NDS_POCKET(battle_items);
            const native::modern_item_t* raw_keyitem_pocket    = CONST_RAW_NDS_POCKET(key_items);

            // Items
            for(size_t i = 0; i < item_pocket->get_size(); i++)
            {
                if(raw_item_pocket[i].index == 0 or raw_item_pocket[i].count == 0) break;
                else item_pocket->set_item_amount(database::get_item_id(raw_item_pocket[i].index,
                                                                        version_id),
                                                  raw_item_pocket[i].count);
            }

            // Medicine
            for(size_t i = 0; i < medicine_pocket->get_size(); i++)
            {
                if(raw_medicine_pocket[i].index == 0 or raw_medicine_pocket[i].count == 0) break;
                else medicine_pocket->set_item_amount(database::get_item_id(raw_medicine_pocket[i].index,
                                                                            version_id),
                                                      raw_medicine_pocket[i].count);
            }

            // Balls
            for(size_t i = 0; i < ball_pocket->get_size(); i++)
            {
                if(raw_ball_pocket[i].index == 0 or raw_ball_pocket[i].count == 0) break;
                else ball_pocket->set_item_amount(database::get_item_id(raw_ball_pocket[i].index,
                                                                        version_id),
                                                  raw_ball_pocket[i].count);
            }

            // TMs/HMs
            for(size_t i = 0; i < tmhm_pocket->get_size(); i++)
            {
                if(raw_tmhm_pocket[i].index == 0 or raw_tmhm_pocket[i].count == 0) break;
                else tmhm_pocket->set_item_amount(database::get_item_id(raw_tmhm_pocket[i].index,
                                                                        version_id),
                                                  raw_tmhm_pocket[i].count);
            }

            // Berries
            for(size_t i = 0; i < berry_pocket->get_size(); i++)
            {
                if(raw_berry_pocket[i].index == 0 or raw_berry_pocket[i].count == 0) break;
                else berry_pocket->set_item_amount(database::get_item_id(raw_berry_pocket[i].index,
                                                                         version_id),
                                                   raw_berry_pocket[i].count);
            }

            // Battle Items
            for(size_t i = 0; i < battleitem_pocket->get_size(); i++)
            {
                if(raw_battleitem_pocket[i].index == 0 or raw_battleitem_pocket[i].count == 0) break;
                else battleitem_pocket->set_item_amount(database::get_item_id(raw_battleitem_pocket[i].index,
                                                                              version_id),
                                                        raw_battleitem_pocket[i].count);
            }

            // Key Items
            for(size_t i = 0; i < keyitem_pocket->get_size(); i++)
            {
                if(raw_keyitem_pocket[i].index == 0 or raw_keyitem_pocket[i].count == 0) break;
                else keyitem_pocket->set_item_amount(database::get_item_id(raw_keyitem_pocket[i].index,
                                                                           version_id),
                                                     raw_keyitem_pocket[i].count);
            }
        }

        #define RAW_NDS_POCKET(field) (is_hgss ? reinterpret_cast<native::hgss_item_storage_t*>(raw_bag)->field \
                                               : reinterpret_cast<native::dppt_item_storage_t*>(raw_bag)->field);

        void export_gen4_bag(bag::sptr libpkmn_bag, void* raw_bag)
        {
            uint16_t version_id = libpkmn_bag->get_game_id();
            bool is_hgss = (libpkmn_bag->get_game_id() == Versions::HEARTGOLD or
                            libpkmn_bag->get_game_id() == Versions::SOULSILVER);

            // LibPKMN pockets
            pocket::sptr item_pocket       = libpkmn_bag->get_pocket("Items");
            pocket::sptr medicine_pocket   = libpkmn_bag->get_pocket("Medicine");
            pocket::sptr ball_pocket       = libpkmn_bag->get_pocket("Poke Balls");
            pocket::sptr tmhm_pocket       = libpkmn_bag->get_pocket("TMs and HMs");
            pocket::sptr berry_pocket      = libpkmn_bag->get_pocket("Berries");
            pocket::sptr mail_pocket       = libpkmn_bag->get_pocket("Mail");
            pocket::sptr battleitem_pocket = libpkmn_bag->get_pocket("Battle Items");
            pocket::sptr keyitem_pocket    = libpkmn_bag->get_pocket("Key Items");

            // Native pockets
            native::modern_item_t* raw_item_pocket       = RAW_NDS_POCKET(items);
            native::modern_item_t* raw_medicine_pocket   = RAW_NDS_POCKET(key_items);
            native::modern_item_t* raw_ball_pocket       = RAW_NDS_POCKET(balls);
            native::modern_item_t* raw_tmhm_pocket       = RAW_NDS_POCKET(tms_hms);
            native::modern_item_t* raw_berry_pocket      = RAW_NDS_POCKET(berries);
            native::modern_item_t* raw_mail_pocket       = RAW_NDS_POCKET(mail);
            native::modern_item_t* raw_battleitem_pocket = RAW_NDS_POCKET(battle_items);
            native::modern_item_t* raw_keyitem_pocket    = RAW_NDS_POCKET(key_items);

            // Items
            item_list_t item_list = item_pocket->get_item_list();
            for(size_t i = 0; i < item_list.size(); i++)
            {
                raw_item_pocket[i].index = ITEM_GAME_INDEX(item_list, i, version_id);
                raw_item_pocket[i].count = item_list[i].second;
            }

            // Medicine
            item_list = medicine_pocket->get_item_list();
            for(size_t i = 0; i < item_list.size(); i++)
            {
                raw_medicine_pocket[i].index = ITEM_GAME_INDEX(item_list, i, version_id);
                raw_medicine_pocket[i].count = item_list[i].second;
            }

            // Ball
            item_list = ball_pocket->get_item_list();
            for(size_t i = 0; i < item_list.size(); i++)
            {
                raw_ball_pocket[i].index = ITEM_GAME_INDEX(item_list, i, version_id);
                raw_ball_pocket[i].count = item_list[i].second;
            }

            // TMs/HMS
            item_list = tmhm_pocket->get_item_list();
            for(size_t i = 0; i < item_list.size(); i++)
            {
                raw_tmhm_pocket[i].index = ITEM_GAME_INDEX(item_list, i, version_id);
                raw_tmhm_pocket[i].count = item_list[i].second;
            }

            // Berries
            item_list = berry_pocket->get_item_list();
            for(size_t i = 0; i < item_list.size(); i++)
            {
                raw_berry_pocket[i].index = ITEM_GAME_INDEX(item_list, i, version_id);
                raw_berry_pocket[i].count = item_list[i].second;
            }

            // Mail
            item_list = mail_pocket->get_item_list();
            for(size_t i = 0; i < item_list.size(); i++)
            {
                raw_mail_pocket[i].index = ITEM_GAME_INDEX(item_list, i, version_id);
                raw_mail_pocket[i].count = item_list[i].second;
            }

            // Battle Items
            item_list = battleitem_pocket->get_item_list();
            for(size_t i = 0; i < item_list.size(); i++)
            {
                raw_battleitem_pocket[i].index = ITEM_GAME_INDEX(item_list, i, version_id);
                raw_battleitem_pocket[i].count = item_list[i].second;
            }

            // Key Items
            item_list = keyitem_pocket->get_item_list();
            for(size_t i = 0; i < item_list.size(); i++)
            {
                raw_keyitem_pocket[i].index = ITEM_GAME_INDEX(item_list, i, version_id);
                raw_keyitem_pocket[i].count = item_list[i].second;
            }
        }
    } /* namespace conversions */
} /* namespace pkmn */
