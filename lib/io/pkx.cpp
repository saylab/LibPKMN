/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <pkmn/config.hpp>
#include <pkmn/database.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/paths.hpp>
#include <pkmn/conversions/pokemon.hpp>

#include "pkx.hpp"
#include "../SQLiteCpp/SQLiteC++.h"

namespace fs = boost::filesystem;

namespace pkmn
{
    namespace io
    {
        bool pkx::valid(const pkmn::pkstring &filename)
        {
            size_t filesize = fs::file_size(fs::path(filename));

            if(filesize != sizeof(native::gen6_pc_pokemon_t) and filesize != sizeof(native::gen6_party_pokemon_t))
                return false;

            native::gen6_party_pokemon_t pkmn;
            std::ifstream ifile(filename.const_char(), (std::ios::in | std::ios::binary));
            ifile.read((char*)&pkmn, sizeof(native::gen6_party_pokemon_t));
            ifile.close();

            try
            {
                SQLite::Database db(get_database_path());

                PKMN_UNUSED(int pokemon_id) = database::get_pokemon_id(pkmn.pc.blocks.blockA.species,
                                                                       Versions::X);
            }
            catch(...) {return false;}

            return true;
        }

        pokemon::sptr pkx::from(const pkmn::pkstring &filename)
        {
            if(not pkx::valid(filename))
                throw std::runtime_error("Invalid .pkm file.");

            native::gen6_pc_pokemon_t native;
            std::ifstream ifile(filename.const_char(), (std::ios::in | std::ios::binary));
            ifile.read((char*)&native, sizeof(native::gen6_pc_pokemon_t));
            ifile.close();

            return conversions::import_gen6_pokemon(native, "X", false);
        }

        void pkx::to(pokemon::sptr pkmn, const pkmn::pkstring &filename)
        {
            native::gen6_pc_pokemon_t native;
            conversions::export_gen6_pokemon(pkmn, native, false);

            std::ofstream ofile(filename.const_char(), (std::ios::out | std::ios::binary));
            ofile.write((char*)&native, sizeof(native::gen6_pc_pokemon_t));
            ofile.close();

            if(not pkx::valid(filename))
                throw std::runtime_error("Failed to create valid .pkm file.");
        }
    }
}
