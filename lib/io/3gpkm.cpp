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

#include "3gpkm.hpp"
#include "../SQLiteCpp/SQLiteC++.h"

namespace fs = boost::filesystem;

namespace pkmn
{
    namespace io
    {
        bool _3gpkm::valid(const pkmn::pkstring &filename)
        {
            size_t filesize = fs::file_size(fs::path(filename));

            if(filesize != sizeof(native::gen3_pc_pokemon_t) and filesize != sizeof(native::gen3_party_pokemon_t))
                return false;

            native::gen3_party_pokemon_t pkmn;
            std::ifstream ifile(filename.const_char(), (std::ifstream::in | std::ifstream::binary));
            ifile.read((char*)&pkmn, sizeof(native::gen3_party_pokemon_t));
            ifile.close();

            try
            {
                SQLite::Database db(get_database_path());

                PKMN_UNUSED(int pokemon_id) = database::get_pokemon_id(pkmn.pc.blocks.growth.species,
                                                                       Versions::EMERALD);
                for(size_t i = 0; i < 4; i++)
                {
                    if(int(db.execAndGet(str(boost::format("SELECT generation_id FROM moves WHERE id=%d")
                                         % int(pkmn.pc.blocks.attacks.moves[i])).c_str())) > 3)
                        return false;
                }
            }
            catch(...) {return false;}

            return true;
        }

        pokemon::sptr _3gpkm::from(const pkmn::pkstring &filename)
        {
            if(not _3gpkm::valid(filename))
                throw std::runtime_error("Invalid .3gpkm file.");

            native::gen3_pc_pokemon_t pkmn;
            std::ifstream ifile(filename.const_char(), (std::ifstream::in | std::ifstream::binary));
            ifile.read((char*)&pkmn, sizeof(native::gen3_pc_pokemon_t));
            ifile.close();

            return conversions::import_gen3_pokemon(pkmn, "Emerald");
        }

        void _3gpkm::to(pokemon::sptr pkmn, const pkmn::pkstring &filename)
        {
            native::gen3_pc_pokemon_t native;
            memcpy(&native, pkmn->get_native(), sizeof(native::gen3_pc_pokemon_t));

            std::ofstream ofile(filename.const_char(), (std::ofstream::out | std::ofstream::binary));
            ofile.write((char*)&native, sizeof(native::gen3_pc_pokemon_t));
            ofile.close();
        }
    }
}
