/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

#include <pkmnsim/base_pkmn.hpp>

#include "game_save_gen3impl.hpp"

using namespace std;

//TODO: Figure out format of gender
//TODO: Implement ability

//We have to use Pokehack's method of getting strings
extern string attacks[];
extern string natures[];
extern string pokemon_species[];

namespace pkmnsim
{
    game_save_gen3impl::game_save_gen3impl(std::string filename): game_save(filename)
    {
        try
        {
            if(verify()) throw runtime_error("Could not parse game save file!");

            party.clear();
            for(int i = 0; i < 6; i++)
            {
                if(pokemon_species[parser->pokemon_growth[i]->species] != "No pokemon") //Empty slot in party
                {
                    party.push_back(belt_pokemon_t_to_spec_pkmn(parser->pokemon[i], parser->pokemon_effort[i],
                                    parser->pokemon_attacks[i], parser->pokemon_misc[i], i));
                }
            }
        }
        catch(const exception &e)
        {
            cout << "Caught exception: " << e.what()<< endl;
            exit(EXIT_FAILURE);
        }
        
    }

    int game_save_gen3impl::verify() {return parser->load(game_save_filepath.c_str(), save_type);}

    spec_pkmn::sptr game_save_gen3impl::belt_pokemon_t_to_spec_pkmn(belt_pokemon_t* bpt, pokemon_effort_t* pet,
                                                                        pokemon_attacks_t* pat, pokemon_misc_t* pmt, int index)
    {
        //Get values necessary to create spec_pkmn::sptr instance
        string identifier = pokemon_species[parser->pokemon_growth[index]->species];
        int level = bpt->level;
        string move1 = attacks[pat->atk1];
        string move2 = (attacks[pat->atk2] == "No attack") ? "None" : attacks[pat->atk2];
        string move3 = (attacks[pat->atk3] == "No attack") ? "None" : attacks[pat->atk3];
        string move4 = (attacks[pat->atk4] == "No attack") ? "None" : attacks[pat->atk4];

        spec_pkmn::sptr s_pkmn = spec_pkmn::make(identifier, level, 3, move1, move2, move3, move4, true);

        //Get values to manually set s_pkmn's private values
        s_pkmn->nickname = (char*)bpt->name;
        s_pkmn->HP = bpt->maxHP;
        s_pkmn->ATK = bpt->attack;
        s_pkmn->DEF = bpt->defense;
        s_pkmn->SPD = bpt->speed;
        s_pkmn->SATK = bpt->spatk;
        s_pkmn->SDEF = bpt->spdef;
        s_pkmn->ivHP = pmt->IVs.hp;
        s_pkmn->ivATK = pmt->IVs.atk;
        s_pkmn->ivDEF = pmt->IVs.def;
        s_pkmn->ivSPD = pmt->IVs.spd;
        s_pkmn->ivSATK = pmt->IVs.spatk;
        s_pkmn->ivSDEF = pmt->IVs.spdef;
        s_pkmn->evHP = pet->hp;
        s_pkmn->evATK = pet->attack;
        s_pkmn->evDEF = pet->defense;
        s_pkmn->evSPD = pet->speed;
        s_pkmn->evSATK = pet->spatk;
        s_pkmn->evSDEF = pet->spdef;
        s_pkmn->gender = pmt->tgender;

        //Get nature (string splitting necessary)
        string nature_string = natures[bpt->personality];
        vector<string> strs;
        boost::split(strs, nature_string, boost::is_any_of(" "));
        s_pkmn->nature = pkmn_nature::make(strs[0]);

        return s_pkmn;
    }
}
