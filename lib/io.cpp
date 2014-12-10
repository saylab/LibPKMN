/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cstdint>
#include <fstream>
#include <cstring>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/io.hpp>

#include <pkmds/pkmds_g5.h>
#include <pkmds/pkmds_g5_sqlite.h>

#include "library_bridge.hpp"
#include "conversions/pokemon.hpp"

#include "SQLiteCpp/SQLiteC++.h"

namespace fs = boost::filesystem;

namespace pkmn
{
    namespace io
    {
        void export_to_3gpkm(team_pokemon::sptr t_pkmn, const std::string &filename)
        {
            if(t_pkmn->get_generation() != 3)
                throw std::runtime_error("The given Pokemon is not from Generation III.");

            gen3_party_pokemon_t pkmn;
            conversions::export_gen3_pokemon(t_pkmn, pkmn, false);

            std::ofstream ofile(filename.c_str(), std::ofstream::out | std::ofstream::binary);
            ofile.write((char*)&pkmn, sizeof(gen3_party_pokemon_t));
            ofile.close();
        }

        //For now, everything is Ruby until there's a better way
        team_pokemon::sptr import_from_3gpkm(const std::string &filename)
        {
            uint32_t filesize = fs::file_size(fs::path(filename));
            std::ifstream ifile;

            if(filesize == sizeof(gen3_pc_pokemon_t))
            {
                gen3_pc_pokemon_t pkmn;
                ifile.open(filename.c_str(), std::ifstream::in | std::ifstream::binary);
                ifile.read((char*)&pkmn, sizeof(gen3_pc_pokemon_t));
                ifile.close();

                return conversions::import_gen3_pokemon(pkmn, Versions::RUBY, false);
            }
            else if(filesize == sizeof(gen3_party_pokemon_t))
            {
                gen3_party_pokemon_t pkmn;
                ifile.open(filename.c_str(), std::ifstream::in | std::ifstream::binary);
                ifile.read((char*)&pkmn, sizeof(gen3_party_pokemon_t));
                ifile.close();

                return conversions::import_gen3_pokemon(pkmn, Versions::RUBY, false);
            }
            else throw std::runtime_error("This is not a valid .3gpkm file.");
        }

        void export_to_pkm(team_pokemon::sptr t_pkmn, const std::string &filename)
        {
            party_pkm* p_pkm = new party_pkm;
            conversions::export_gen5_pokemon(t_pkmn, p_pkm);

            uint8_t pkm_contents[sizeof(pokemon_obj)];
            memcpy(&pkm_contents, p_pkm, sizeof(pokemon_obj));

            std::ofstream ofile;
            ofile.open(filename.c_str(), std::ofstream::out | std::ofstream::binary);
            ofile.write((char*)p_pkm, sizeof(pokemon_obj));
            ofile.close();
        }

        team_pokemon::sptr import_from_pkm(const std::string &filename)
        {
            party_pkm* p_pkm = new party_pkm;
            pokemon_obj* pkmn_obj = new pokemon_obj;

            uint8_t pkm_contents[sizeof(pokemon_obj)];
            memset(pkm_contents, 0, sizeof(pokemon_obj));

            std::ifstream ifile;
            ifile.open(filename.c_str(), std::ifstream::in | std::ifstream::binary);
            ifile.read((char*)pkm_contents, sizeof(pokemon_obj));
            ifile.close();
            memcpy(pkmn_obj, pkm_contents, sizeof(pokemon_obj));

            libpkmn_pctoparty(p_pkm, pkmn_obj);
            return conversions::import_gen5_pokemon(p_pkm);
        }
    }
}
