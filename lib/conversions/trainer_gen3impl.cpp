/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <fstream>
#include <sstream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include <pkmnsim/base_move.hpp>
#include <pkmnsim/base_pokemon.hpp>
#include <pkmnsim/enums.hpp>
#include <pkmnsim/paths.hpp>
#include <pkmnsim/pkmn_nature.hpp>
#include <pkmnsim/spec_pokemon.hpp>
#include <pkmnsim/database/queries.hpp>

#include <pkmnsim/conversions.hpp>
#include <sqlitecpp/SQLiteCPP.h>
#include "trainer_gen3impl.hpp"

using namespace std;

namespace pkmnsim
{
    trainer_gen3impl::trainer_gen3impl(SQLite::Database *import_db): trainer()
    {
        parser = SaveParser::Instance();

        SQLite::Database pkmnsim_db(get_database_path().c_str());

        SQLite::Statement trainer_info_query(*import_db, "SELECT * FROM trainer_info");
        trainer_info_query.executeStep();

        from_game = trainer_info_query.getColumn(1);
        trainer_name = trainer_info_query.getColumnStr(2);
        trainer_id = trainer_info_query.getColumn(3);
        int party_size = trainer_info_query.getColumn(4);
        money = trainer_info_query.getColumn(5);

        party.clear();

        for(unsigned int i = 0; i < party_size; i++)
        {
            //Grab values from export database necessary to create spec_pokemon
            SQLite::Statement party_query(*import_db, str(boost::format("SELECT * FROM party WHERE id=%d")
                                                                        % i).c_str()
                                         );
            party_query.executeStep();

            int pkmn_id = party_query.getColumn(1);
            int species_id = party_query.getColumn(2);
            string nickname = party_query.getColumn(3);
            int level = party_query.getColumn(4);
            int item_held_id = party_query.getColumn(5);
            int move1_id = party_query.getColumn(6);
            int move2_id = party_query.getColumn(7);
            int move3_id = party_query.getColumn(8);
            int move4_id = party_query.getColumn(9);
            int HP = party_query.getColumn(10);
            int ATK = party_query.getColumn(11);
            int DEF = party_query.getColumn(12);
            int SATK = party_query.getColumn(13);
            int SDEF = party_query.getColumn(14);
            int SPD = party_query.getColumn(15);
            int evHP = party_query.getColumn(16);
            int evATK = party_query.getColumn(17);
            int evDEF = party_query.getColumn(18);
            int evSATK = party_query.getColumn(19);
            int evSDEF = party_query.getColumn(20);
            int evSPD = party_query.getColumn(21);
            int ivHP = party_query.getColumn(22);
            int ivATK = party_query.getColumn(23);
            int ivDEF = party_query.getColumn(24);
            int ivSATK = party_query.getColumn(25);
            int ivSDEF = party_query.getColumn(26);
            int ivSPD = party_query.getColumn(27);
            int nature_id = party_query.getColumn(28);

            string item_held;
            if(item_held_id == -1) item_held = "None";
            else
            {
                item_held = pkmnsim_db.execAndGetStr(str(boost::format(
                                                         "SELECT name FROM item_names WHERE item_id=%d")
                                                         % item_held_id).c_str(), "name"
                                                    );
            }
            spec_pokemon::sptr s_pkmn = spec_pokemon::make(species_id, Games::RUBY, level, move1_id, move2_id,
                                                           move3_id, move4_id);

            //Manually set other values
            s_pkmn->nickname = nickname;
            s_pkmn->HP = HP;
            s_pkmn->ATK = ATK;
            s_pkmn->DEF = DEF;
            s_pkmn->SATK = SATK;
            s_pkmn->SDEF = SDEF;
            s_pkmn->SPD = SPD;
            s_pkmn->evHP = evHP;
            s_pkmn->evATK = evATK;
            s_pkmn->evDEF = evDEF;
            s_pkmn->evSATK = evSATK;
            s_pkmn->evSDEF = evSDEF;
            s_pkmn->evSPD = evSPD;
            s_pkmn->ivHP = ivHP;
            s_pkmn->ivATK = ivATK;
            s_pkmn->ivDEF = ivDEF;
            s_pkmn->ivSATK = ivSATK;
            s_pkmn->ivSDEF = ivSDEF;
            s_pkmn->ivSPD = ivSPD;
            s_pkmn->nature = pkmn_nature::make(nature_id);

            party.push_back(s_pkmn);
        }
    }

    trainer_gen3impl::trainer_gen3impl(string filename, int game): trainer()
    {
        parser = SaveParser::Instance();

        from_game = game;
        int game_type;

        switch(game)
        {
            case Games::RUBY:
            case Games::SAPPHIRE:
            case Games::EMERALD:
                game_type = 0;
                break;

            case Games::FIRE_RED:
            case Games::LEAF_GREEN:
                game_type = 1;
                break;

            default:
                cerr << "Only Ruby, Sapphire, Emerald, Fire Red, and Leaf Green currently supported." << endl;
                exit(EXIT_FAILURE);
        }

        int err = parser->load(filename.c_str(), game_type);
        if(err)
        {
            cerr << "Error loading game save file." << endl;
            exit(EXIT_FAILURE);
        }

        money = 0; //Placeholder until money is implemented in Pokehack
        trainer_name = parser->get_text(parser->pokemon[0]->otname, 7);
        short* trainer_id_short = (short*)&(parser->pokemon[0]->otid);
		trainer_id = int(trainer_id_short[0]);

        converter pkmn_converter;
        party.clear();
        for(int i = 0; i < 6; i++)
            if(string(pokemon_species[parser->pokemon_growth[i]->species]) != "No pokemon")
                party.push_back(pkmn_converter.pokehack_pkmn_to_spec_pokemon(parser->pokemon[i],
                                                                          parser->pokemon_attacks[i],
                                                                          parser->pokemon_effort[i],
                                                                          parser->pokemon_misc[i],
                                                                          parser->pokemon_growth[i]));
    }
}