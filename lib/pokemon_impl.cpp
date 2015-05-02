/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <sstream>
#include <stdexcept>

#include <boost/assign.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/pokemon.hpp>
#include <pkmn/database.hpp>

#include "copy_sptrs.hpp"

#include "pokemon_impl.hpp"
#include "pokemon_gen1impl.hpp"
#include "pokemon_gen2impl.hpp"
#include "pokemon_gen3impl.hpp"
#include "pokemon_ndsimpl.hpp"
#include "pokemon_gen6impl.hpp"

#include "internal.hpp"
#include "io/3gpkm.hpp"
#include "io/pkm.hpp"
#include "io/pkx.hpp"
#include "io/pksql.hpp"
#include "SQLiteCpp/SQLiteC++.h"

namespace fs = boost::filesystem;

namespace pkmn
{
    pokemon::sptr pokemon::make(int species, int version, int level,
                                int move1,   int move2,   int move3,
                                int move4)
    {
        int generation = database::get_generation(version);

        // Check if Pokémon is valid and was present in given version
        std::ostringstream query_stream;
        query_stream << "SELECT generation_id FROM pokemon_species WHERE id="
                     << species;
        SQLite::Database db(get_database_path());
        SQLite::Statement query(db, query_stream.str().c_str());
        if(species != Species::NONE and species != Species::INVALID)
        {
            if(not query.executeStep())
                throw std::runtime_error("Invalid Pokémon species.");
            if(int(query.getColumn(0)) > generation)
                throw std::runtime_error("This Pokémon was not present in the given version.");
        }

        switch(generation)
        {
            case 1:
                return sptr(new pokemon_gen1impl(species, version, level,
                                                 move1, move2, move3, move4));

            case 2:
                return sptr(new pokemon_gen2impl(species, version, level,
                                                 move1, move2, move3, move4));

            case 3:
                return sptr(new pokemon_gen3impl(species, version, level,
                                                 move1, move2, move3, move4));

            case 6:
                return sptr(new pokemon_gen6impl(species, version, level,
                                                 move1, move2, move3, move4));

            default:
                return sptr(new pokemon_ndsimpl(species, version, level,
                                                move1, move2, move3, move4));
        }
    }

    pokemon::sptr pokemon::make(const pkmn::pkstring &name,  const pkmn::pkstring &game, int level,
                                const pkmn::pkstring &move1, const pkmn::pkstring &move2,
                                const pkmn::pkstring &move3, const pkmn::pkstring &move4)
    {
        return make(database::get_species_id(name),
                    database::get_version_id(game),
                    level,
                    database::get_move_id(move1),
                    database::get_move_id(move2),
                    database::get_move_id(move3),
                    database::get_move_id(move4));
    }

    pokemon::sptr pokemon::make(const pkmn::pkstring &filename)
    {
        if(fs::extension(fs::path(filename)) == ".pksql" and io::pksql::valid(filename))
            return io::pksql::from(filename);
        else if(fs::extension(fs::path(filename)) == ".3gpkm" and io::_3gpkm::valid(filename))
            return io::_3gpkm::from(filename);
        else if(fs::extension(fs::path(filename)) == ".pkm" and io::pkm::valid(filename))
            return io::pkm::from(filename);
        else if(fs::extension(fs::path(filename)) == ".pkx" and io::pkx::valid(filename))
            return io::pkx::from(filename);
        else
            throw std::runtime_error("Invalid file.");
    }

    void pokemon::export_to(pokemon::sptr pkmn, const pkmn::pkstring &filename)
    {
        std::string ext = fs::extension(fs::path(filename));

        if(ext == ".pksql")
        {
            pkmn::database_sptr output = io::pksql::to(pkmn, filename);
            if(not io::pksql::valid(filename))
            {
                fs::remove(fs::path(filename));
                throw std::runtime_error("Failed to export to PKSQL.");
            }
        }
        else if(ext == ".3gpkm")
        {
            io::_3gpkm::to(pkmn, filename);
            if(not io::_3gpkm::valid(filename))
            {
                fs::remove(fs::path(filename));
                throw std::runtime_error("Failed to export to .3gpkm.");
            }
        }
        else if(ext == ".pkm")
        {
            io::pkm::to(pkmn, filename);
            if(not io::pkm::valid(filename))
            {
                fs::remove(fs::path(filename));
                throw std::runtime_error("Failed to export to .pkm.");
            }
        }
        else throw std::runtime_error("Extension must be .pksql, .3gpkm, or .pkm.");
    }

    pkmn::shared_ptr<SQLite::Database> pokemon_impl::_db;

    pkmn::dict<int, std::string> pokemon_impl::_version_dirs = boost::assign::map_list_of
        (Versions::GOLD,       "gold")
        (Versions::SILVER,     "silver")
        (Versions::CRYSTAL,    "crystal")
        (Versions::RUBY,       "ruby-sapphire")
        (Versions::SAPPHIRE,   "ruby-sapphire")
        (Versions::FIRERED,    "firered-leafgreen")
        (Versions::LEAFGREEN,  "firered-leafgreen")
        (Versions::EMERALD,    "emerald")
        (Versions::DIAMOND,    "diamond-pearl")
        (Versions::PEARL,      "diamond-pearl")
        (Versions::PLATINUM,   "diamond-pearl")
        (Versions::HEARTGOLD,  "heartgold-soulsilver")
        (Versions::SOULSILVER, "heartgold-soulsilver")
        (Versions::BLACK,      "black-white")
        (Versions::WHITE,      "black-white")
        (Versions::BLACK_2,    "black2-white2")
        (Versions::WHITE_2,    "black2-white2")
    ;

    boost::format pokemon_impl::_generation_format   = boost::format("generation-%d");
    boost::format pokemon_impl::_pokemon_format      = boost::format("%d.png");
    boost::format pokemon_impl::_pokemon_form_format = boost::format("%d-%s.png");

    pokemon_impl::pokemon_impl(int game_index, int version_id, bool none):
        pokemon(),
        _version_id(version_id),
        _none(none),
        _pokedex(pokedex::make(database::get_version_name(version_id))),
        _prng(prng::make(database::get_generation(version_id)))
    {
        CONNECT_TO_DB(_db);

        if(none)
        {
            _species_id = Species::NONE;
            _form_id = Species::NONE;
            _invalid = false;
        }
        else
        {
            try
            {
                _species_id = database::get_pokemon_id(game_index, version_id);
                _invalid = false;
            }
            catch(...)
            {
                _species_id = Species::INVALID;
                _invalid = true;
            }

            _form_id = _species_id;
        }

        _pokedex_entry = _pokedex->get_pokemon_entry(_species_id);
    }

    pokemon_impl::pokemon_impl(const pokemon_impl &other):
        _species_id(other._species_id),
        _form_id(other._form_id),
        _version_id(other._version_id),
        _none(other._none),
        _invalid(other._invalid),
        _attributes(other._attributes),
        _pokedex(copy_pokedex(other._pokedex)),
        _pokedex_entry(other._pokedex_entry),
        _prng(copy_prng(other._prng)) {}

    pokemon_impl& pokemon_impl::operator=(const pokemon_impl &other)
    {
        _pokedex       = copy_pokedex(other._pokedex);
        _pokedex_entry = other._pokedex_entry;
        _prng          = copy_prng(other._prng);
        _species_id    = other._species_id;
        _form_id       = other._form_id;
        _version_id    = other._version_id;
        _attributes    = other._attributes;
        _none          = other._none;
        _invalid       = other._invalid;

        return *this;
    }

    /*
     * Getting Non-battle info
     */

    pkmn::pokemon_entry_t pokemon_impl::get_pokedex_entry() const
    {
        return _pokedex->get_pokemon_entry(_species_id, get_form_id());
    }

    /*
     * Getting Individual Stat Info
     */

    pkmn::pkstring pokemon_impl::get_form() const
    {
        if(_species_id == _form_id)
            return "Standard";
        else
            return database::get_form_name(_form_id);
    }

    /*
     * Getting LibPKMN info
     */

    pkmn::pkstring pokemon_impl::get_game() const
    {
        return database::get_version_name(_version_id);
    }

    int pokemon_impl::get_generation() const
    {
        return database::get_generation(_version_id);
    }

    int pokemon_impl::get_attribute(const pkmn::pkstring &attribute) const
    {
        return _attributes.at(attribute);
    }

    pkmn::dict<pkmn::pkstring, int> pokemon_impl::get_attributes() const
    {
        return _attributes;
    }

    bool pokemon_impl::has_attribute(const pkmn::pkstring &attribute) const
    {
        return _attributes.has_key(attribute);
    }

    void pokemon_impl::set_attribute(const pkmn::pkstring &attribute, int value)
    {
        _attributes[attribute] = value;
    }

    pkmn::pkstring pokemon_impl::get_icon_path() const
    {   
        fs::path icon_path(get_images_dir());

        icon_path /= "pokemon-icons";
        if(_form_id == _species_id)
            icon_path /= str(_pokemon_format % _species_id);
        else
        {
            std::ostringstream query_stream;
            query_stream << "SELECT image_name FROM libpkmn_pokemon_form_names WHERE form_id="
                         << _form_id;
            std::string image_suffix = _db->execAndGet(query_stream.str().c_str());

            icon_path /= str(_pokemon_form_format % _species_id % image_suffix);
        }

        return icon_path.string();
    }   

    pkmn::pkstring pokemon_impl::get_sprite_path() const
    {   
        fs::path sprite_path(get_images_dir());

        sprite_path /= str(_generation_format % database::get_generation(_version_id));
        sprite_path /= _version_dirs[_version_id];
        if(_pokedex_entry.has_gender_differences and get_gender() == "Female")
            sprite_path /= "female";
        if(is_shiny())
            sprite_path /= "shiny";

        if(_form_id == _species_id)
            sprite_path /= str(_pokemon_format % _species_id);
        else
        {
            std::ostringstream query_stream;
            query_stream << "SELECT image_name FROM libpkmn_pokemon_form_names WHERE form_id="
                         << _form_id;
            std::string image_suffix = _db->execAndGet(query_stream.str().c_str());

            sprite_path /= str(_pokemon_form_format % _species_id % image_suffix);
        }

        return sprite_path.string();
    }

    /*
     * Database Info
     */

    int pokemon_impl::get_species_id() const
    {
        return _species_id;
    }

    int pokemon_impl::get_pokemon_id() const
    {
        return database::get_pokemon_id(_form_id);
    }

    int pokemon_impl::get_game_id() const
    {
        return _version_id;
    }

    int pokemon_impl::get_form_id() const
    {
        return _form_id;
    }
} /* namespace pkmn */
