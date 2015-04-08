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

#include "pkm.hpp"
#include "../SQLiteCpp/SQLiteC++.h"

namespace fs = boost::filesystem;

namespace pkmn
{
    namespace io
    {
        bool pkm::valid(const pkmn::pkstring &filename)
        {
            size_t filesize = fs::file_size(fs::path(filename));

            if(filesize != sizeof(native::nds_pc_pokemon_t) and filesize != sizeof(native::nds_party_pokemon_t))
                return false;

            native::nds_party_pokemon_t pkmn;
            std::ifstream ifile(filename.const_char(), (std::ios::in | std::ios::binary));
            ifile.read((char*)&pkmn, sizeof(native::nds_party_pokemon_t));
            ifile.close();

            try
            {
                SQLite::Database db(get_database_path());

                PKMN_UNUSED(int pokemon_id) = database::get_pokemon_id(pkmn.pc.blocks.blockA.species,
                                                                       Versions::PLATINUM);
                for(size_t i = 0; i < 4; i++)
                {
                    if(pkmn.pc.blocks.blockB.moves[i] == 0) continue;

                    if(int(db.execAndGet(str(boost::format("SELECT generation_id FROM moves WHERE id=%d")
                                         % int(pkmn.pc.blocks.blockB.moves[i])).c_str())) > 4)
                        return false;
                }
            }
            catch(...) {return false;}

            return true;
        }

        pokemon::sptr pkm::from(const pkmn::pkstring &filename)
        {
            if(not pkm::valid(filename))
                throw std::runtime_error("Invalid .pkm file.");

            native::nds_pc_pokemon_t native;
            std::ifstream ifile(filename.const_char(), (std::ios::in | std::ios::binary));
            ifile.read((char*)&native, sizeof(native::nds_pc_pokemon_t));
            ifile.close();

            return conversions::import_nds_pokemon(native, "Platinum", false);
        }

        void pkm::to(pokemon::sptr pkmn, const pkmn::pkstring &filename)
        {
            native::nds_pc_pokemon_t native;
            conversions::export_nds_pokemon(pkmn, native, false);

            std::ofstream ofile(filename.const_char(), (std::ios::out | std::ios::binary));
            ofile.write((char*)&native, sizeof(native::nds_pc_pokemon_t));
            ofile.close();

            if(not pkm::valid(filename))
                throw std::runtime_error("Failed to create valid .pkm file.");
        }
    }
}
