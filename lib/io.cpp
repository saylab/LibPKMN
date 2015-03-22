/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cstdint>
#include <fstream>
#include <cstring>

#include <boost/filesystem.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/io.hpp>
#include <pkmn/conversions/pokemon.hpp>

namespace fs = boost::filesystem;

namespace pkmn
{
    namespace io
    {
        //For now, everything is Ruby until there's a better way
        pokemon::sptr import_from_3gpkm(const pkmn::pkstring &filename)
        {
            uint32_t filesize = uint32_t(fs::file_size(fs::path(filename)));
            std::ifstream ifile;

            if(filesize == sizeof(native::gen3_pc_pokemon_t))
            {
                native::gen3_pc_pokemon_t native_pkmn;
                ifile.open(filename.const_char(), std::ifstream::in | std::ifstream::binary);
                ifile.read((char*)&native_pkmn, sizeof(native::gen3_pc_pokemon_t));
                ifile.close();

                return conversions::import_gen3_pokemon(native_pkmn, "Ruby", false);
            }
            else if(filesize == sizeof(native::gen3_party_pokemon_t))
            {
                native::gen3_party_pokemon_t native_pkmn;
                ifile.open(filename.const_char(), std::ifstream::in | std::ifstream::binary);
                ifile.read((char*)&native_pkmn, sizeof(native::gen3_party_pokemon_t));
                ifile.close();

                return conversions::import_gen3_pokemon(native_pkmn, "Ruby", false);
            }
            else throw std::runtime_error("This is not a valid .3gpkm file.");
        }

        void export_to_3gpkm(pokemon::sptr pkmn, const pkmn::pkstring &filename)
        {
            if(pkmn->get_generation() != 3)
                throw std::runtime_error("The given Pokémon is not from Generation III.");

            native::gen3_party_pokemon_t native_pkmn;
            conversions::export_gen3_pokemon(pkmn, native_pkmn, false);

            std::ofstream ofile(filename.const_char(), std::ofstream::out | std::ofstream::binary);
            ofile.write((char*)&native_pkmn, sizeof(native::gen3_party_pokemon_t));
            ofile.close();
        }

        pokemon::sptr import_from_pkm(const pkmn::pkstring &filename)
        {
            uint32_t filesize = uint8_t(fs::file_size(fs::path(filename)));
            std::ifstream ifile;

            if(filesize == sizeof(native::nds_pc_pokemon_t))
            {
                native::nds_pc_pokemon_t native_pkmn;
                memset(&native_pkmn, 0xFF, sizeof(native::nds_pc_pokemon_t));
                ifile.open(filename.const_char(), std::ifstream::in | std::ifstream::binary);
                ifile.read((char*)&native_pkmn, sizeof(native::nds_pc_pokemon_t));
                ifile.close();

                // TODO: use fields to distinguish
                return conversions::import_nds_pokemon(native_pkmn, "Diamond", false);
            }
            else if(filesize == sizeof(native::nds_party_pokemon_t)) // Generation IV
            {
                native::nds_party_pokemon_t native_pkmn;
                memset(&native_pkmn, 0xFF, sizeof(native::nds_party_pokemon_t));
                ifile.open(filename.const_char(), std::ifstream::in | std::ifstream::binary);
                ifile.read((char*)&native_pkmn, sizeof(native::nds_party_pokemon_t));
                ifile.close();

                // TODO: distinguish between versions
                return conversions::import_nds_pokemon(native_pkmn, "Diamond", false);
            }
            else if(filesize == 220) // Generation V
            {
                native::nds_party_pokemon_t native_pkmn;
                memset(&native_pkmn, 0xFF, sizeof(native::nds_party_pokemon_t));
                ifile.open(filename.const_char(), std::ifstream::in | std::ifstream::binary);
                ifile.read((char*)&native_pkmn, sizeof(native::nds_party_pokemon_t));
                ifile.close();

                // TODO: distinguish between versions
                return conversions::import_nds_pokemon(native_pkmn, "Black", false);
            }
            else throw std::runtime_error("This is not a valid .pkm file.");
        }

        void export_to_pkm(pokemon::sptr pkmn, const pkmn::pkstring &filename)
        {
            if(pkmn->get_generation() != 4 and pkmn->get_generation() != 5)
                throw std::runtime_error("The given Pokemon is not from Generation IV-V.");

            // TODO: Gen IV vs Gen V
            native::nds_party_pokemon_t native_pkmn;
            conversions::export_nds_pokemon(pkmn, native_pkmn, false);

            std::ofstream ofile(filename.const_char(), std::ofstream::out | std::ofstream::binary);
            ofile.write((char*)&native_pkmn, sizeof(native::nds_party_pokemon_t));
            ofile.close();
        }
    }
}
