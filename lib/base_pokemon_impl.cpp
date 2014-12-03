/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <boost/format.hpp>

#include "base_pokemon_impl.hpp"
#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/database/queries.hpp>

#include "base_pokemon_impl.hpp"
#include "base_pokemon_gen1impl.hpp"
#include "base_pokemon_gen2impl.hpp"
#include "base_pokemon_modernimpl.hpp"

namespace fs = boost::filesystem;

namespace pkmn
{
    base_pokemon::sptr base_pokemon::make(unsigned int species_id, unsigned int game_id)
    {
        //Get generation from game enum
        unsigned int gen = database::get_generation(game_id);

        if(gen < 1 or gen > 6) throw std::runtime_error("Gen must be 1-6.");

        switch(gen)
        {
            case 1:
                return sptr(new base_pokemon_gen1impl(species_id, game_id));

            case 2:
                return sptr(new base_pokemon_gen2impl(species_id, game_id));

            default:
                return sptr(new base_pokemon_modernimpl(species_id, game_id));
        }
    }

    base_pokemon::sptr base_pokemon::make(const pkmn::pkstring &species, const pkmn::pkstring &game)
    {
        return make(database::get_species_id(species), database::get_version_id(game));
    }

    pkmn::shared_ptr<SQLite::Database> base_pokemon_impl::_db;
    boost::format base_pokemon_impl::_png_format = boost::format("%d.png");

    base_pokemon_impl::base_pokemon_impl(unsigned int id, unsigned int game_id):
        base_pokemon(),
        _game_id(game_id),
        _generation(database::get_generation(game_id)),
        _pokemon_id(Species::NONE),
        _form_id(0),
        _species_id(Species::NONE),
        _type1_id(Types::NONE), _type2_id(Types::NONE),
        _hp(0), _attack(0), _defense(0), _speed(0),
        _special(0), _special_attack(0), _special_defense(0)
    {
        //The first initialization will stick for all others
        if(!_db) _db = pkmn::shared_ptr<SQLite::Database>(new SQLite::Database(get_database_path().c_str()));

        if(id == Species::INVALID)
        {
            _pokemon_id = Species::INVALID;
            _species_id = Species::INVALID;
            _form_id = Species::INVALID;
        }
        else if(id != Species::NONE)
        {
            _species_id = id;
            _pokemon_id = id; //For all default forms, pokemon_id = species_id
            _form_id = id; //For all default forms, form_id = species_id
            std::ostringstream query_stream;

            //Fail if this Pokemon didn't exist in this generation
            query_stream << "SELECT generation_id FROM pokemon_species WHERE id=" << _species_id;
            unsigned int gen_id = _db->execAndGet(query_stream.str().c_str());
            if(gen_id > _generation)
            {
                std::string error_message = str(boost::format("base_pokemon: invalid generation (%d > %d)")
                                                          % gen_id % _generation);
                throw std::runtime_error(error_message.c_str());
            }

            /*
             * Even though most attributes are queried from the database when called, stats take a long time when
             * doing a lot at once, so grab these upon instantiation
             */
            query_stream.str("");
            query_stream << "SELECT base_stat FROM pokemon_stats WHERE pokemon_id=" << _pokemon_id
                         << " AND stat_id IN (1,2,3,6)";
            SQLite::Statement stats_query(*_db, query_stream.str().c_str());

            stats_query.executeStep();
            _hp = stats_query.getColumn(0);
            stats_query.executeStep();
            _attack = stats_query.getColumn(0);
            stats_query.executeStep();
            _defense = stats_query.getColumn(0);
            stats_query.executeStep();
            _speed = stats_query.getColumn(0);

            //Get first type
            query_stream.str("");
            query_stream << "SELECT type_id FROM pokemon_types WHERE pokemon_id=" << _pokemon_id << " AND slot=1";
            _type1_id = _db->execAndGet(query_stream.str().c_str());

            //Get second type, if applicable
            query_stream.str("");
            query_stream << "SELECT type_id FROM pokemon_types WHERE pokemon_id=" << _pokemon_id << " AND slot=2";
            SQLite::Statement type2_query(*_db, query_stream.str().c_str());
            _type2_id = type2_query.executeStep() ? type2_query.getColumn(0) : int(Types::NONE);
        }

        _images_dir = fs::path(get_images_dir());
        _icon_dir = fs::path(_images_dir / "pokemon-icons");
        _images_default_basename = str(_png_format % _species_id);
        _images_gen_string = str(boost::format("generation-%d") % _generation);
    }

    pkmn::pkstring base_pokemon_impl::get_game() const
    {
        std::ostringstream query_stream;
        query_stream << "SELECT name FROM version_names WHERE local_language_id=9 AND version_id=" << _game_id;
        return (const char*)(_db->execAndGet(query_stream.str().c_str()));
    }

    unsigned int base_pokemon_impl::get_generation() const {return _generation;}

    pkmn::pkstring base_pokemon_impl::get_name() const
    {
        switch(_species_id)
        {
            case Species::NONE:
                return "None";

            case Species::INVALID:
                return "Invalid";

            default:
                return database::get_species_name(_species_id);
        }
    }

    pkmn::pkstring base_pokemon_impl::get_species() const
    {
        switch(_species_id)
        {
            case Species::NONE:
                return "None";

            case Species::INVALID:
                return "Invalid";

            default:
            {
                std::ostringstream query_stream;
                query_stream << "SELECT genus FROM pokemon_species_names WHERE local_language_id=9 AND pokemon_species_id="
                             << _species_id;
                return (const char*)(_db->execAndGet(query_stream.str().c_str()));
            }
        }
    }

    unsigned int base_pokemon_impl::get_pokedex_num() const {return _species_id;}
    pkmn::pkstring base_pokemon_impl::get_pokedex_entry() const {return database::get_pokedex_entry(_species_id, _game_id);}

    pkmn::pkstring_pair_t base_pokemon_impl::get_types() const
    {
        pkmn::pkstring_pair_t types;
        types.first = database::get_type_name(_type1_id);
        types.second = database::get_type_name(_type2_id);

        return types;
    }

    double base_pokemon_impl::get_height() const
    {
        switch(_species_id)
        {
            case Species::NONE:
            case Species::INVALID:
                return 0.0;

            default:
                std::ostringstream query_stream;
                query_stream << "SELECT height FROM pokemon WHERE id=" << _pokemon_id;
                return double(_db->execAndGet(query_stream.str().c_str())) / 10.0;
        }
    }

    double base_pokemon_impl::get_weight() const
    {
        switch(_species_id)
        {
            case Species::NONE:
            case Species::INVALID:
                return 0.0;

            default:
                std::ostringstream query_stream;
                query_stream << "SELECT weight FROM pokemon WHERE id=" << _pokemon_id;
                return double(_db->execAndGet(query_stream.str().c_str())) / 10.0;
        }
    }

    void base_pokemon_impl::get_egg_groups(pkmn::pkstring_vector_t& egg_group_vec) const
    {    
        std::vector<unsigned int> egg_group_ids;
        get_egg_group_ids(egg_group_ids);

        for(size_t i = 0; i < egg_group_ids.size(); i++) egg_group_vec.push_back(database::get_egg_group_name(egg_group_ids[i]));
    }

    void base_pokemon_impl::get_evolutions(base_pokemon_vector &evolution_vec) const
    {
        evolution_vec.clear();

        std::vector<unsigned int> id_vec;
        _get_evolution_ids(id_vec);

        if(id_vec.begin() == id_vec.end()) evolution_vec.push_back(make(pkmn::Species::NONE, _game_id));
        else
        {
            for(size_t i = 0; i < id_vec.size(); i++) evolution_vec.push_back(make(id_vec[i], _game_id));
        }
    }

    bool base_pokemon_impl::is_fully_evolved() const
    {
        std::vector<unsigned int> id_vec;
        _get_evolution_ids(id_vec);

        return (id_vec.begin() == id_vec.end());
    }

    double base_pokemon_impl::get_chance_male() const
    {
        switch(_species_id)
        {
            case Species::NONE:
            case Species::INVALID:
                return 0.0;

            default:
                /*
                 * gender_val_dict's keys correspond to how the different
                 * gender rates are represented in the database. The values
                 * are the actual decimal representations of the percentages.
                 */
                pkmn::dict<unsigned int, double> gender_val_dict; //Double is percentage male
                gender_val_dict[0] = 1.0;
                gender_val_dict[1] = 0.875;
                gender_val_dict[2] = 0.75;
                gender_val_dict[4] = 0.5;
                gender_val_dict[6] = 0.25;
                gender_val_dict[8] = 0.0;

                std::ostringstream query_stream;
                query_stream << "SELECT gender_rate FROM pokemon_species WHERE id=" << _species_id;
                int gender_val = _db->execAndGet(query_stream.str().c_str());

                if(gender_val == -1) return 0.0;
                else return gender_val_dict[gender_val];
        }
    }

    double base_pokemon_impl::get_chance_female() const
    {
        switch(_species_id)
        {
            case Species::NONE:
            case Species::INVALID:
                return 0.0;

            default:
                /*
                 * gender_val_dict's keys correspond to how the different
                 * gender rates are represented in the database. The values
                 * are the actual decimal representations of the percentages.
                 */
                pkmn::dict<unsigned int, double> gender_val_dict; //Double is percentage male
                gender_val_dict[0] = 1.0;
                gender_val_dict[1] = 0.875;
                gender_val_dict[2] = 0.75;
                gender_val_dict[4] = 0.5;
                gender_val_dict[6] = 0.25;
                gender_val_dict[8] = 0.0; 

                std::ostringstream query_stream;
                query_stream << "SELECT gender_rate FROM pokemon_species WHERE id=" << _species_id;
                int gender_val = _db->execAndGet(query_stream.str().c_str());

                if(gender_val == -1) return 0.0; 
                else return (1.0 - gender_val_dict[gender_val]);
        }
    }

    unsigned int base_pokemon_impl::get_exp_yield() const
    {
        /*
         * The "old_exp_yields" table in libpkmn_db_additions covers the case
         * where there the experience yield changed in Generation V. However,
         * there are eleven cases where Generation IV has its own experience
         * yield.
         */
        if(_generation == 4 and _species_id == Species::ABRA) return 75;
        else if(_generation == 4 and _species_id == Species::MACHOP) return 75;
        else if(_generation == 4 and _species_id == Species::GEODUDE) return 73;
        else if(_generation == 4 and _species_id == Species::OMANYTE) return 99;
        else if(_generation == 4 and _species_id == Species::KABUTO) return 99;
        else if(_generation == 4 and _species_id == Species::KABUTOPS) return 199;
        else if(_generation == 4 and _species_id == Species::DUNSPARCE) return 125;
        else if(_generation == 4 and _species_id == Species::LILEEP) return 99;
        else if(_generation == 4 and _species_id == Species::CRADILY) return 199;
        else if(_generation == 4 and _species_id == Species::ANORITH) return 99;
        else if(_generation == 4 and _species_id == Species::ARMALDO) return 199;
        else
        {
            std::ostringstream query_stream;
            if(_generation < 4 and _species_id <= 439) query_stream << "SELECT exp_yield FROM old_exp_yields WHERE species_id=" << _species_id;
            else query_stream << "SELECT base_experience FROM pokemon WHERE species_id=" << _species_id;

            return _db->execAndGet(query_stream.str().c_str());
        }
    }

    pkmn::pkstring base_pokemon_impl::get_form() const
    {
        if(_form_id == _species_id) return "Standard";
        else
        {
            std::ostringstream query_stream;
            query_stream << "SELECT form_name FROM pokemon_form_names WHERE local_language_id=9 AND pokemon_form_id="
                         << _form_id;
            return (const char*)(_db->execAndGet(query_stream.str().c_str()));
        }
    }

    unsigned int base_pokemon_impl::get_pokemon_id() const {return _pokemon_id;}

    unsigned int base_pokemon_impl::get_species_id() const {return _species_id;}

    unsigned int base_pokemon_impl::get_game_id() const {return _game_id;}

    unsigned int base_pokemon_impl::get_form_id() const {return _form_id;}

    void base_pokemon_impl::get_egg_group_ids(std::vector<unsigned int>& egg_group_id_vec) const
    {
        egg_group_id_vec.clear();

        switch(_species_id)
        {
            case Species::NONE:
            case Species::INVALID:
                egg_group_id_vec.push_back(pkmn::Species::NONE);
                break;

            default:
                std::ostringstream query_stream;
                query_stream << "SELECT egg_group_id FROM pokemon_egg_groups WHERE species_id="
                             << _species_id;
                SQLite::Statement query(*_db, query_stream.str().c_str());

                while(query.executeStep()) egg_group_id_vec.push_back(query.getColumn(0));
                break;
        }
    }

    void base_pokemon_impl::_get_evolution_ids(std::vector<unsigned int>& id_vec) const
    {
        id_vec.clear();

        if(_species_id != Species::NONE and _species_id != Species::INVALID)
        {
            std::ostringstream query_stream;
            query_stream << "SELECT id FROM pokemon_species WHERE evolves_from_species_id=" << _species_id;
            SQLite::Statement query(*_db, query_stream.str().c_str());
            while(query.executeStep()) id_vec.push_back(query.getColumn(0));

            //Evolutions may not be present in specified generation
            std::vector<int> to_erase;
            for(unsigned int i = 0; i < id_vec.size(); i++)
            {
                query_stream.str("");
                query_stream << "SELECT generation_id FROM pokemon_species WHERE id=" << id_vec[i];
                unsigned int generation_id = _db->execAndGet(query_stream.str().c_str());
                if(generation_id > _generation) to_erase.push_back(i);
            }
            for(int j = to_erase.size()-1; j >= 0; j--) id_vec.erase(id_vec.begin() + to_erase[j]);
        }
    }

    /*
     * There are several Pokémon whose stats changed in Generation VI. This function sets the old stats
     * if the Pokémon is from a prior generation.
     */
    void base_pokemon_impl::_set_old_values()
    {
        switch(_species_id)
        {
            case Species::BUTTERFREE:
                _special_attack = 80;
                break;

            case Species::BEEDRILL:
                _attack = 80;
                break;

            case Species::PIDGEOT:
                _speed = 91;
                break;

            case Species::PIKACHU:
                _defense = 30;
                _special_defense = 40;
                break;

            case Species::RAICHU:
                _speed = 100;
                break;

            case Species::NIDOQUEEN:
                _attack = 82;
                break;

            case Species::NIDOKING:
                _attack = 92;
                break;

            case Species::CLEFAIRY:
                _type1_id = Types::NORMAL;
                break;

            case Species::CLEFABLE:
                _type1_id = Types::NORMAL;
                _special_attack = 85;
                break;

            case Species::JIGGLYPUFF:
                _type2_id = Types::NONE;
                break;

            case Species::WIGGLYTUFF:
                _type2_id = Types::NONE;
                _special_attack = 75;
                break;

            case Species::VILEPLUME:
                _special_attack = 75;
                break;

            case Species::POLIWRATH:
                _attack = 85;
                break;

            case Species::ALAKAZAM:
                _special_defense = 85;
                break;

            case Species::VICTREEBEL:
                _special_defense = 60;
                break;

            case Species::GOLEM:
                _attack = 210;
                break;

            case Species::MR_MIME:
                _type2_id = Types::NONE;
                break;

            case Species::IGGLYBUFF:
                _type2_id = Types::NONE;
                break;

            case Species::AMPHAROS:
                _defense = 75;
                break;

            case Species::BELLOSSOM:
                _defense = 85;
                break;

            case Species::MARILL:
                _type2_id = Types::NONE;
                break;

            case Species::AZUMARILL:
                _type2_id = Types::NONE;
                _defense = 85;
                break;

            case Species::JUMPLUFF:
                _special_defense = 85;
                break;

            case Species::BEAUTIFLY:
                _special_attack = 90;
                break;

            case Species::RALTS:
            case Species::KIRLIA:
            case Species::GARDEVOIR:
                _type2_id = Types::NONE;
                break;

            case Species::EXPLOUD:
                _special_defense = 63;
                break;

            case Species::AZURILL:
            case Species::MAWILE:
                _type2_id = Types::NONE;
                break;

            case Species::STARAPTOR:
                _special_defense = 50;
                break;

            case Species::ROSERADE:
                _defense = 55;
                break;

            case Species::MIME_JR:
                _type2_id = Types::NONE;
                break;

            case Species::STOUTLAND:
                _attack = 100;
                break;

            case Species::UNFEZANT:
                _attack = 105;
                break;

            case Species::GIGALITH:
                _special_defense = 70;
                break;

            case Species::SEISMITOAD:
                _attack = 85;
                break;

            case Species::LEAVANNY:
                _special_defense = 70;
                break;

            case Species::SCOLIPEDE:
                _attack = 90;
                break;

            case Species::COTTONEE:
            case Species::WHIMSICOTT:
                _type2_id = Types::NONE;
                break;

            case Species::KROOKODILE:
                _defense = 70;
                break;
        }
    }
} /* namespace pkmn */
