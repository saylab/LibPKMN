/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

#include <pkmnsim/dict.hpp>
#include <pkmnsim/pkmn_nature.hpp>

#include "internal.hpp"
#include "sqlitecpp/SQLiteCPP.h"

using namespace std;

namespace pkmnsim
{
    pkmn_nature::pkmn_nature(string identifier)
    {
        double ATKmod = 1.0;
        double DEFmod = 1.0;
        double SATKmod = 1.0;
        double SDEFmod = 1.0;
        double SPDmod = 1.0;

        SQLite::Database db(get_database_path().c_str());

        //Match database's identifier format
        to_database_format(&identifier);

        string query_string = "SELECT id FROM natures WHERE identifier='" + identifier + "'";
        int nature_id = db.execAndGet(query_string.c_str(), identifier);

        query_string = "SELECT name FROM nature_names WHERE nature_id=" + to_string(nature_id);
        name = db.execAndGetStr(query_string.c_str(), identifier);

        //Getting positive mod
        query_string = "SELECT increased_stat_id FROM natures WHERE identifier='" + identifier + "'";
        int pos_id = db.execAndGet(query_string.c_str(), identifier);
        switch(pos_id)
        {
            case 2:
                ATKmod += 0.1;
                break;
            case 3:
                DEFmod += 0.1;
                break;
            case 4:
                SATKmod += 0.1;
                break;
            case 5:
                SDEFmod += 0.1;
                break;
            case 6:
                SPDmod += 0.1;
                break;
            default:
                break;
        }

        //Getting negative mod
        query_string = "SELECT decreased_stat_id FROM natures WHERE identifier='" + identifier + "'";
        int neg_id = db.execAndGet(query_string.c_str(), identifier);
        switch(neg_id)
        {
            case 2:
                ATKmod -= 0.1;
                break;
            case 3:
                DEFmod -= 0.1;
                break;
            case 4:
                SATKmod -= 0.1;
                break;
            case 5:
                SDEFmod -= 0.1;
                break;
            case 6:
                SPDmod -= 0.1;
                break;
            default:
                break;
        }
    }

    string pkmn_nature::get_name() {return name;}

    dict<string, double> pkmn_nature::get_mods()
    {
        dict<string, double> mod_dict;
        mod_dict["ATK"] = ATKmod;
        mod_dict["DEF"] = DEFmod;
        mod_dict["SPD"] = SPDmod;
        mod_dict["SATK"] = SATKmod;
        mod_dict["SDEF"] = SDEFmod;
        return mod_dict;
    }

    std::string pkmn_nature::get_info()
    {
        std::string info_string = "Nature: " + name + "\n";
        info_string += "Stat Mods:\n";
        info_string += " - Attack: " + to_string(ATKmod);
        info_string += " - Defense: " + to_string(DEFmod);
        info_string += " - Special Attack: " + to_string(SATKmod);
        info_string += " - Special Defense: " + to_string(SDEFmod);
        info_string += " - Speed: " + to_string(SPDmod);
        
        return info_string;
    }

    pkmn_nature::sptr pkmn_nature::make(string identifier)
    {
        try {return pkmn_nature::sptr(new pkmn_nature(identifier));}
        catch(exception &e)
        {
            cout << "Caught exception: " << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }
}
