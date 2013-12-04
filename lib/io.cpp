/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cstdint>
#include <fstream>
#include <string.h>

#include <pkmnsim/io.hpp>

#include <pkmds/pkmds_g5.h>
#include <pkmds/pkmds_g5_sqlite.h>

#include "library_bridge.hpp"
#include "conversions/pokemon.hpp"

namespace pkmnsim
{
    namespace io
    {
        void export_to_pkm(team_pokemon::sptr t_pkmn, std::string filename)
        {
            party_pkm* p_pkm = new party_pkm;
            conversions::team_pokemon_to_pkmds_g5_pokemon(t_pkmn, p_pkm);

            uint8_t pkm_contents[sizeof(pokemon_obj)];
            memcpy(&pkm_contents, &(p_pkm->pkm_data), sizeof(pokemon_obj));

            std::ofstream ofile;
            ofile.open(filename.c_str(), std::ofstream::out | std::ofstream::binary);
            ofile.write((char*)pkm_contents, sizeof(pokemon_obj));
            ofile.close();
        }

        team_pokemon::sptr import_from_pkm(std::string filename)
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
            
            pkmnsim_pctoparty(p_pkm, pkmn_obj);
            return conversions::pkmds_g5_pokemon_to_team_pokemon(p_pkm);
        }
        
        void export_to_pkx(team_pokemon::sptr t_pkmn, std::string filename)
        {
            party_pkx* p_pkx = new party_pkx;
            conversions::team_pokemon_to_pkmds_g6_pokemon(t_pkmn, p_pkx);

            uint8_t pkx_contents[sizeof(pokemonx_obj)];
            memcpy(&pkx_contents, &(p_pkx->pkx_data), sizeof(pokemonx_obj));

            std::ofstream ofile;
            ofile.open(filename.c_str(), std::ofstream::out | std::ofstream::binary);
            ofile.write((char*)pkx_contents, sizeof(pokemonx_obj));
            ofile.close();
        }

        team_pokemon::sptr import_from_pkx(std::string filename)
        {
            party_pkx* p_pkm = new party_pkx;
            pokemonx_obj* pkmn_obj = new pokemonx_obj;

            uint8_t pkx_contents[sizeof(pokemonx_obj)];
            memset(pkx_contents, 0, sizeof(pokemonx_obj));

            std::ifstream ifile;
            ifile.open(filename.c_str(), std::ifstream::in | std::ifstream::binary);
            ifile.read((char*)pkx_contents, sizeof(pokemonx_obj));
            ifile.close();
            memcpy(pkmn_obj, pkx_contents, sizeof(pokemonx_obj));
            
            pkmnsim_pctopartyx(p_pkm, pkmn_obj);
            return conversions::pkmds_g6_pokemon_to_team_pokemon(p_pkm);
        }
    }
}
