/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <sstream>

#include <boost/format.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/database.hpp>
#include <pkmn/paths.hpp>

#include "pokedex_impl.hpp"

namespace pkmn
{
    pokedex::sptr pokedex::make(const pkmn::pkstring& game)
    {
        return sptr(new pokedex_impl(game));
    }

    pokedex_impl::pokedex_impl(const pkmn::pkstring& game):
        pokedex(),
        _version_id(database::get_version_id(game))
    {
        if(!_db) _db = pkmn::shared_ptr<SQLite::Database>(new SQLite::Database(get_database_path()));

        _generation       = database::get_generation(_version_id);
        _version_group_id = database::get_version_group_id(_version_id);
    }

    pkmn::dict<uint16_t, pkmn::dict<uint16_t, pokemon_entry_t> > pokedex_impl::_pokemon_entry_cache;
    pkmn::dict<uint16_t, pkmn::dict<uint16_t, move_entry_t> >    pokedex_impl::_move_entry_cache;
    pkmn::dict<uint16_t, pkmn::dict<uint16_t, item_entry_t> >    pokedex_impl::_item_entry_cache;
    pkmn::shared_ptr<SQLite::Database>                           pokedex_impl::_db;

    pkmn::pkstring pokedex_impl::get_game() const
    {
        return database::get_version_name(_version_id);
    }

    uint16_t pokedex_impl::get_generation() const
    {
        return database::get_generation(_version_id);
    }

    pokemon_entry_t& pokedex_impl::get_pokemon_entry(const uint16_t species_id,
                                                     const uint16_t form_id)
    {
        uint16_t pokemon_id;

        // Use default form if nothing given
        if(form_id == 0 or form_id == species_id) pokemon_id = species_id;
        else
        {
            std::ostringstream query_stream;
            query_stream << "SELECT pokemon_id,introduced_in_version_group_id FROM pokemon_forms WHERE id="
                         << form_id;
            SQLite::Statement query(*_db, query_stream.str().c_str());
            if(query.executeStep())
            {
                if(database::get_version_group_id(_version_id) < int(query.getColumn(1)))
                    throw std::runtime_error("This form did not exist in this version.");

                pokemon_id = query.getColumn(0);
            }
            else throw std::runtime_error("Invalid form.");
        }

        _create_pokemon_entry(pokemon_id);
        return _pokemon_entry_cache[_version_id][pokemon_id];
    }

    pokemon_entry_t& pokedex_impl::get_pokemon_entry(const pkmn::pkstring& species_name,
                                                     const pkmn::pkstring& form_name)
    {
        unsigned int form_id;
        if(form_name == "Standard" or form_name == species_name or form_name == "")
            form_id = database::get_species_id(species_name);
        else
            form_id = database::get_form_id(species_name, form_name);

        return get_pokemon_entry(database::get_species_id(species_name), form_id);
    }

    move_entry_t& pokedex_impl::get_move_entry(const uint16_t move_id)
    {
        std::ostringstream query_stream;
        query_stream << "SELECT generation_id FROM moves WHERE id=" << move_id;
        if(uint16_t(_db->execAndGet(query_stream.str().c_str())) > _generation)
            throw std::runtime_error("This move did not exist in this generation.");

        _create_move_entry(move_id);
        return _move_entry_cache[_version_id][move_id];
    }

    move_entry_t& pokedex_impl::get_move_entry(const pkmn::pkstring& move_name)
    {
        return get_move_entry(database::get_move_id(move_name));
    }

    item_entry_t& pokedex_impl::get_item_entry(const uint16_t item_id)
    {
        std::ostringstream query_stream;
        query_stream << "SELECT generation_id FROM item_game_indices WHERE item_id="
                     << item_id;
        SQLite::Statement query(*_db, query_stream.str().c_str());
        if(not query.executeStep())
            throw std::runtime_error("This item did not exist in this game.");

        _create_item_entry(item_id);
        return _item_entry_cache[_version_group_id][item_id];
    }

    item_entry_t& pokedex_impl::get_item_entry(const pkmn::pkstring& item_name)
    {
        return get_item_entry(database::get_item_id(item_name));
    }

    void pokedex_impl::_create_pokemon_entry(const uint16_t pokemon_id)
    {
        if(_pokemon_entry_cache[_version_id].has_key(pokemon_id)) return;

        pokemon_entry_t entry;

        std::ostringstream query_stream;

        /*
         * Everything from "pokemon" table
         */
        query_stream << "SELECT * FROM pokemon WHERE id=" << pokemon_id;
        SQLite::Statement pokemon_query(*_db, query_stream.str().c_str());
        pokemon_query.executeStep();
        entry.pokedex_num = pokemon_query.getColumn(2);           // species_id
        entry.species_name = database::get_species_name(entry.pokedex_num);
        entry.height = float(pokemon_query.getColumn(3)) / float(10.0); // height
        entry.weight = float(pokemon_query.getColumn(4)) / float(10.0); // weight
        entry.exp_yield = pokemon_query.getColumn(5);             // base_experience

        /*
         * Everything from "pokemon_species" table
         */
        query_stream.str("");
        query_stream << "SELECT * FROM pokemon_species WHERE id="
                     << database::get_species_id(pokemon_id);
        SQLite::Statement pokemon_species_query(*_db, query_stream.str().c_str());
        pokemon_species_query.executeStep();
        entry.chance_male = _get_gender_chance(entry.pokedex_num,
                                               pokemon_species_query.getColumn(8),
                                               true);                       // gender_rate
        entry.chance_female = _get_gender_chance(entry.pokedex_num,
                                                 pokemon_species_query.getColumn(8),
                                                 false);                    // gender_rate
        entry.catch_rate = pokemon_species_query.getColumn(9);              // capture_rate
        entry.base_friendship = pokemon_species_query.getColumn(10);        // base_happiness
        entry.has_gender_differences = pokemon_species_query.getColumn(13); // has_gender_differences

        bool old_games = (_generation < 3);

        /*
         * Abilities
         */
        query_stream.str("");
        query_stream << "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id="
                     << pokemon_id << " AND is_hidden=0";
        SQLite::Statement pokemon_abilities_query(*_db, query_stream.str().c_str());
        pokemon_abilities_query.executeStep();
        entry.abilities.first = database::get_ability_name(pokemon_abilities_query.getColumn(0)); // ability_id
        if(pokemon_abilities_query.executeStep())
            entry.abilities.second = database::get_ability_name(pokemon_abilities_query.getColumn(0)); // ability_id
        else
            entry.abilities.second = "None";
        if(_generation < 5)
            entry.hidden_ability = "None";
        else
        {
            query_stream.str("");
            query_stream << "SELECT ability_id FROM pokemon_abilities WHERE pokemon_id="
                         << pokemon_id << " AND is_hidden=1";
            SQLite::Statement pokemon_abilities_query2(*_db, query_stream.str().c_str());
            if(pokemon_abilities_query2.executeStep())
                entry.hidden_ability = database::get_ability_name(pokemon_abilities_query2.getColumn(0));
            else
                entry.hidden_ability = "None";
        }

        /*
         * Egg groups
         */
        query_stream.str("");
        query_stream << "SELECT egg_group_id FROM pokemon_egg_groups WHERE species_id="
                     << entry.pokedex_num;
        SQLite::Statement pokemon_egg_groups_query(*_db, query_stream.str().c_str());
        pokemon_egg_groups_query.executeStep();
        entry.egg_groups.first = database::get_egg_group_name(pokemon_egg_groups_query.getColumn(0));
        if(pokemon_egg_groups_query.executeStep())
            entry.egg_groups.second = database::get_egg_group_name(pokemon_egg_groups_query.getColumn(0));
        else
            entry.egg_groups.second = "None";

        /*
         * Pokédex entry
         */
        query_stream.str("");
        query_stream << "SELECT flavor_text FROM pokemon_species_flavor_text WHERE species_id="
                     << database::get_species_id(pokemon_id) << " AND version_id="
                     << _version_id << " AND language_id=9";
        entry.pokedex_entry = _db->execAndGet(query_stream.str().c_str());

        /*
         * Base stats, effort yields
         */
        query_stream.str("");
        query_stream << "SELECT base_stat,effort FROM pokemon_stats WHERE pokemon_id="
                     << pokemon_id << " AND stat_id IN (1,2,3,6)";
        SQLite::Statement pokemon_stats_query(*_db, query_stream.str().c_str());
        pokemon_stats_query.executeStep();
        entry.base_stats["HP"] = pokemon_stats_query.getColumn(0);                      // base_stat
        entry.ev_yields["HP"]  = pokemon_stats_query.getColumn(old_games ? 0 : 1);      // base_stat, effort
        pokemon_stats_query.executeStep();
        entry.base_stats["Attack"] = pokemon_stats_query.getColumn(0);                  // base_stat
        entry.ev_yields["Attack"]  = pokemon_stats_query.getColumn(old_games ? 0 : 1);  // base_stat, effort
        pokemon_stats_query.executeStep();
        entry.base_stats["Defense"] = pokemon_stats_query.getColumn(0);                 // base_stat
        entry.ev_yields["Defense"]  = pokemon_stats_query.getColumn(old_games ? 0 : 1); // base_stat, effort
        pokemon_stats_query.executeStep();
        entry.base_stats["Speed"] = pokemon_stats_query.getColumn(0);                   // base_stat
        entry.ev_yields["Speed"]  = pokemon_stats_query.getColumn(old_games ? 0 : 1);   // base_stat, effort
        query_stream.str("");
        if(old_games)
        {
            query_stream << "SELECT base_stat FROM pokemon_stats WHERE pokemon_id="
                         << pokemon_id << " AND stat_id=9";
            entry.base_stats["Special"] = _db->execAndGet(query_stream.str().c_str());  // base_stat
            entry.ev_yields["Special"]  = _db->execAndGet(query_stream.str().c_str());  // base_stat
        }
        else
        {
            query_stream << "SELECT base_stat,effort FROM pokemon_stats WHERE pokemon_id="
                         << pokemon_id << " AND stat_id IN (4,5)";
            SQLite::Statement pokemon_stats_query2(*_db, query_stream.str().c_str());
            pokemon_stats_query2.executeStep();
            entry.base_stats["Special Attack"] = pokemon_stats_query2.getColumn(0);     // base_stat
            entry.ev_yields["Special Attack"]  = pokemon_stats_query2.getColumn(0);     // effort
        }

        /*
         * Types
         */
        query_stream.str("");
        query_stream << "SELECT type_id FROM pokemon_types WHERE pokemon_id="
                     << pokemon_id;
        SQLite::Statement pokemon_types_query(*_db, query_stream.str().c_str());
        pokemon_types_query.executeStep();
        entry.types.first = database::get_type_name(pokemon_types_query.getColumn(0)); // type_id
        if(pokemon_types_query.executeStep())
            entry.types.second = database::get_type_name(pokemon_types_query.getColumn(0));
        else
            entry.types.second = "None";

        _adjust_pokemon_entry(entry);
        _pokemon_entry_cache[_version_id][pokemon_id] = entry;
    }

    float pokedex_impl::_get_gender_chance(const uint16_t species_id,
                                           const uint8_t gender_rate,
                                           bool male) const
    {
        /*
         * gender_val_dict's keys correspond to how the different
         * gender rates are represented in the database. The values
         * are the actual decimal representations of the percentages.
         */
        pkmn::dict<uint8_t, float> gender_val_dict; // Double is percentage male
        gender_val_dict[0] = 1.0;
        gender_val_dict[1] = 0.875;
        gender_val_dict[2] = 0.75;
        gender_val_dict[4] = 0.5;
        gender_val_dict[6] = 0.25;
        gender_val_dict[8] = 0.0;

        std::ostringstream query_stream;
        query_stream << "SELECT gender_rate FROM pokemon_species WHERE id=" << species_id;
        int gender_val = _db->execAndGet(query_stream.str().c_str());

        if(gender_val == -1) return 0.0;
        else return male ? gender_val_dict[gender_val]
                         : (1 - gender_val_dict[gender_val]);
    }

    /*
     * Database values are valid for Generation VI, this fixes entries
     * from earlier generations.
     */
    void pokedex_impl::_adjust_pokemon_entry(pokemon_entry_t& entry)
    {
        if(_generation < 5)
        {
            switch(entry.pokedex_num)
            {
                case Species::BUTTERFREE:
                    if(_generation > 1)
                        entry.base_stats["Special Attack"] = 80;
                    break;

                case Species::BEEDRILL:
                    entry.base_stats["Attack"] = 80;
                    break;

                case Species::PIDGEOT:
                    entry.base_stats["Speed"] = 91;
                    break;

                case Species::PIKACHU:
                    entry.base_stats["Defense"] = 30;
                    if(_generation > 1)
                        entry.base_stats["Special Defense"] = 40;
                    break;

                case Species::RAICHU:
                    entry.base_stats["Speed"] = 100;
                    break;

                case Species::NIDOQUEEN:
                    entry.base_stats["Attack"] = 82;
                    break;

                case Species::NIDOKING:
                    entry.base_stats["Attack"] = 92;
                    break;

                case Species::CLEFAIRY:
                    entry.types.first = "Normal";
                    break;

                case Species::CLEFABLE:
                    entry.types.first = "Normal";
                    if(_generation > 1)
                        entry.base_stats["Special Attack"] = 85;
                    break;

                case Species::JIGGLYPUFF:
                    entry.types.second = "None";
                    break;

                case Species::WIGGLYTUFF:
                    entry.types.second = "None";
                    if(_generation > 1)
                        entry.base_stats["Special Attack"] = 75;
                    break;

                case Species::VILEPLUME:
                    if(_generation > 1)
                        entry.base_stats["Special Attack"] = 75;
                    break;

                case Species::POLIWRATH:
                    entry.base_stats["Attack"] = 85;
                    break;

                case Species::ALAKAZAM:
                    if(_generation > 1)
                        entry.base_stats["Special Defense"] = 85;
                    break;

                case Species::VICTREEBEL:
                    if(_generation > 1)
                        entry.base_stats["Special Defense"] = 60;
                    break;

                case Species::GOLEM:
                    entry.base_stats["Attack"] = 210;
                    break;

                case Species::MR_MIME:
                    entry.types.second = "None";
                    break;

                case Species::IGGLYBUFF:
                    entry.types.second = "None";
                    break;

                case Species::AMPHAROS:
                    entry.base_stats["Defense"] = 75;
                    break;

                case Species::BELLOSSOM:
                    entry.base_stats["Defense"] = 85;
                    break;

                case Species::MARILL:
                    entry.types.second = "None";
                    break;

                case Species::AZUMARILL:
                    entry.types.second = "None";
                    entry.base_stats["Defense"] = 85;
                    break;

                case Species::JUMPLUFF:
                    entry.base_stats["Special Defense"] = 85;
                    break;

                case Species::BEAUTIFLY:
                    entry.base_stats["Special Attack"] = 90;
                    break;

                case Species::RALTS:
                case Species::KIRLIA:
                case Species::GARDEVOIR:
                    entry.types.second = "None";
                    break;

                case Species::EXPLOUD:
                    entry.base_stats["Special Defense"] = 63;
                    break;

                case Species::AZURILL:
                case Species::MAWILE:
                    entry.types.second = "None";
                    break;

                case Species::STARAPTOR:
                    entry.base_stats["Special Defense"] = 50;
                    break;

                case Species::ROSERADE:
                    entry.base_stats["Defense"] = 55;
                    break;

                case Species::MIME_JR:
                    entry.types.second = "None";
                    break;

                case Species::STOUTLAND:
                    entry.base_stats["Attack"] = 100;
                    break;

                case Species::UNFEZANT:
                    entry.base_stats["Attack"] = 105;
                    break;

                case Species::GIGALITH:
                    entry.base_stats["Special Defense"] = 70;
                    break;

                case Species::SEISMITOAD:
                    entry.base_stats["Attack"] = 85;
                    break;

                case Species::LEAVANNY:
                    entry.base_stats["Special Defense"] = 70;
                    break;

                case Species::SCOLIPEDE:
                    entry.base_stats["Attack"] = 90;
                    break;

                case Species::COTTONEE:
                case Species::WHIMSICOTT:
                    entry.types.second = "None";
                    break;

                case Species::KROOKODILE:
                    entry.base_stats["Defense"] = 70;
                    break;
            }

            entry.hidden_ability = "None";
        }
        if(_generation < 4)
        {
            entry.has_gender_differences = false;
        }
        if(_generation < 3)
        {
            entry.abilities       = pkmn::pkstring_pair_t("None", "None");
            entry.base_friendship = 0;
        }
        if(_generation == 1)
        {
            entry.egg_groups    = pkmn::pkstring_pair_t("None", "None");
            entry.chance_male   = 0.0;
            entry.chance_female = 0.0;
        }
    }

    void pokedex_impl::_create_move_entry(const uint16_t move_id)
    {
        if(_move_entry_cache[_generation].has_key(move_id)) return;

        move_entry_t entry;

        std::ostringstream query_stream;

        /*
         * The "moves" table should give us most things.
         */
        query_stream << "SELECT * FROM moves WHERE id=" << move_id;
        SQLite::Statement moves_query(*_db, query_stream.str().c_str());
        moves_query.executeStep();

        entry.name = database::get_move_name(move_id);
        entry.type = database::get_type_name(moves_query.getColumn(3)); // type_id
        entry.damage_class = database::get_move_damage_class_name(uint16_t(moves_query.getColumn(9))); // damage_class_id

        entry.power = moves_query.getColumn(4); // power
        entry.pp = moves_query.getColumn(5); // pp
        entry.accuracy = float(moves_query.getColumn(6)) / float(100.0); // accuracy
        entry.priority = moves_query.getColumn(7); // priority

        // entry.effect = 
        entry.effect_chance = float(moves_query.getColumn(11)) / float(100.0); // effect_chance

        /*
         * Description
         */
        query_stream.str("");
        query_stream << "SELECT flavor_text FROM move_flavor_text WHERE move_id="
                     << move_id << " AND language_id=9 AND version_group_id="
                     << database::get_version_group_id(_version_id);
        entry.description = _db->execAndGet(query_stream.str().c_str());

        /*
         * Target
         */
        query_stream.str("");
        query_stream << "SELECT name FROM move_target_prose WHERE move_target_id="
                     << moves_query.getColumn(8) << " AND local_language_id=9"; // target_id
        entry.target = _db->execAndGet(query_stream.str().c_str());

        /*
         * Effect
         * TODO: parse tokens in short_effect
         */
        query_stream.str("");
        query_stream << "SELECT short_effect FROM move_effect_prose WHERE move_effect_id="
                     << moves_query.getColumn(10);  // effect_id
        entry.effect = _db->execAndGet(query_stream.str().c_str());

        /*
         * Contest type
         */
        query_stream.str("");
        query_stream << "SELECT name FROM contest_type_names WHERE contest_type_id="
                     << moves_query.getColumn(12) << " AND local_language_id=9"; // contest_type_id
        entry.contest_type = _db->execAndGet(query_stream.str().c_str());

        /*
         * Contest effect
         */
        query_stream.str("");
        query_stream << "SELECT flavor_text FROM contest_effect_prose WHERE contest_effect_id="
                     << moves_query.getColumn(13) << " AND local_language_id=9"; // contest_effect_id
        entry.contest_effect = _db->execAndGet(query_stream.str().c_str());

        /*
         * Super Contest effect
         */
        query_stream.str("");
        query_stream << "SELECT flavor_text FROM super_contest_effect_prose"
                     << " WHERE super_contest_effect_id=" << moves_query.getColumn(14)
                     << " AND local_language_id=9"; // super_contest_effect_id
        entry.super_contest_effect = _db->execAndGet(query_stream.str().c_str());

        if(_generation < 6) _adjust_move_entry(entry);
        _move_entry_cache[_version_id][move_id] = entry;
    }

    /*
     * Database values are valid for Generation VI, this fixes entries
     * from earlier generations.
     */
    void pokedex_impl::_adjust_move_entry(move_entry_t& entry)
    {
        uint16_t move_id = database::get_move_id(entry.name);

        std::ostringstream query_stream;
        query_stream << "SELECT gen" << _generation << "_accuracy FROM old_move_accuracies WHERE move_id=" << move_id;
        SQLite::Statement accuracy_query(*_db, query_stream.str().c_str());
        if(accuracy_query.executeStep()) entry.accuracy = float(accuracy_query.getColumn(0)) / 100.0;
    
        // Hypnosis varies in accuracy between games
        if(move_id == Moves::HYPNOSIS and (_version_id == Versions::DIAMOND or _version_id == Versions::PEARL))
            entry.accuracy = 0.7;

        query_stream.str("");
        query_stream << "SELECT gen" << _generation << "_power FROM old_move_powers WHERE move_id=" << move_id;
        SQLite::Statement power_query(*_db, query_stream.str().c_str());
        if(power_query.executeStep()) entry.power = power_query.getColumn(0);

        // Shadow Rush varies in power between Gamecube games
        if(move_id == Moves::SHADOW_RUSH and _version_id == Versions::COLOSSEUM)
            entry.power = 90; 

        query_stream.str("");
        query_stream << "SELECT gen" << _generation << "_pp FROM old_move_pps WHERE move_id=" << move_id;
        SQLite::Statement pp_query(*_db, query_stream.str().c_str());
        if(pp_query.executeStep())
            entry.pp = pp_query.getColumn(0);

        // Not enough type changes to warrant a database table
        if(_generation == 1)
        {
            if(move_id == Moves::BITE or move_id == Moves::GUST or
               move_id == Moves::KARATE_CHOP or move_id == Moves::SAND_ATTACK)
                entry.type = "Normal";
        }
        else if(move_id == Moves::CURSE and _generation < 4)
            entry.type = "???";
        else if(move_id == Moves::CHARM or move_id == Moves::MOONLIGHT or
                move_id == Moves::SWEET_KISS)
            entry.type = "Normal";

        // Only one move changes categories before Generation IV
        if(_generation == 1 and move_id == Moves::BITE)
            entry.damage_class = "Physical";
    
        // TODO: targeting changes, making contact

        query_stream.str("");
        query_stream << "SELECT gen" << _generation << "_priority FROM old_move_priorities WHERE move_id=" << move_id;
        SQLite::Statement priority_query(*_db, query_stream.str().c_str());
        if(priority_query.executeStep())
            entry.priority = priority_query.getColumn(0);

        // Only one move changed name between Generation II-III
        if(move_id == Moves::CONVERSION_2 and _generation < 3)
            entry.name = "Conversion2";

        query_stream.str("");
        query_stream << "SELECT name FROM old_move_names WHERE move_id=" << move_id;
        SQLite::Statement name_query(*_db, query_stream.str().c_str());
        if(name_query.executeStep())
            entry.name = name_query.getColumn(0);

        if(_generation != 4)
            entry.super_contest_effect = "None";

        if(_generation < 4)
        {
            query_stream.str("");
            query_stream << "SELECT name FROM move_damage_class_prose WHERE local_language_id=9"
                         << " AND move_damage_class_id=(SELECT damage_class_id FROM types WHERE"
                         << " id=" << database::get_type_id(entry.type) << ")";
            std::string damage_class_name = _db->execAndGet(query_stream.str().c_str());
            damage_class_name[0] = ::toupper(damage_class_name[0]);
            entry.damage_class = damage_class_name;
        }

        if(_generation < 3)
        {
            entry.contest_type   = "None";
            entry.contest_effect = "None";
        }
    }

    void pokedex_impl::_create_item_entry(const uint16_t item_id)
    {
        if(_item_entry_cache[_version_group_id].has_key(item_id)) return;

        item_entry_t entry;

        std::ostringstream query_stream;

        query_stream << "SELECT * FROM items WHERE id=" << item_id;
        SQLite::Statement items_query(*_db, query_stream.str().c_str());
        items_query.executeStep();

        entry.name = database::get_item_name(item_id);
        entry.category = database::get_item_category_name(uint16_t(items_query.getColumn(2))); // category_id

        /*
         * Pocket name
         */
        query_stream.str("");
        query_stream << "SELECT name FROM pocket_names WHERE version_group_id="
                     << _version_group_id << " AND pocket_id=(SELECT pocket_id" // category_id
                     << " FROM item_categories WHERE id=" << uint16_t(items_query.getColumn(2))
                     << ")";
        entry.pocket = _db->execAndGet(query_stream.str().c_str());

        /*
         * Description
         */
        query_stream.str("");
        uint16_t machine_id;
        bool is_machine = ((item_id >= Items::TM01 and item_id <= Items::HM08)
                           or (item_id >= Items::TM93 and item_id <= Items::TM95)
                           or (item_id >= Items::TM96 and item_id <= Items::TM100));

        if(is_machine)
        {
            if(item_id >= Items::TM01 and item_id <= Items::TM92)
                machine_id = item_id - 304;
            else
                machine_id = 101 + (item_id - Items::HM01);

            query_stream << "SELECT move_id FROM machines WHERE machine_number=" << machine_id
                         << " AND version_group_id=" << database::get_version_group_id(_version_id);
            uint16_t move_id = _db->execAndGet(query_stream.str().c_str());

            query_stream.str("");
            query_stream << "SELECT flavor_text FROM move_flavor_text WHERE move_id="
                         << move_id << " AND version_group_id=" << _version_group_id
                         << " AND language_id=9";
            pkmn::pkstring move_description = _db->execAndGet(query_stream.str().c_str());

            entry.description = str(boost::format("%s - %s")
                                    % database::get_move_name(move_id)
                                    % move_description);
        }
        else
        {
            query_stream << "SELECT flavor_text FROM item_flavor_text WHERE item_id="
                         << item_id << " AND version_group_id=" << _version_group_id
                         << " AND language_id=9";
            entry.description = _db->execAndGet(query_stream.str().c_str());
        }

        entry.cost = items_query.getColumn(3); // cost

        /*
         * Fling effect
         */
        if(_generation < 4)
        {
            entry.fling_power  = 0;
            entry.fling_effect = "None";
        }
        else
        {
            entry.fling_power = items_query.getColumn(4); // fling_power
            query_stream.str("");
            query_stream << "SELECT effect FROM item_fling_effect_prose WHERE" // fling_effect_id
                         << " item_fling_effect_id=" << uint16_t(items_query.getColumn(5))
                         << " AND local_language_id=9";
            SQLite::Statement fling_effect_query(*_db, query_stream.str().c_str());
            if(fling_effect_query.executeStep())
                entry.fling_effect = fling_effect_query.getColumn(0); // effect
            else
                entry.fling_effect = "None";
        }

        _item_entry_cache[_version_group_id][item_id] = entry;
    }
} /* namespace pkmn */
