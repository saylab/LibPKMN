#include "base_pkmn_gen1impl.hpp"
#include <boost/format.hpp>
#include <sqlitecpp/SQLiteCPP.h>
#include <stdio.h>

using namespace std;

namespace pkmnsim
{
    base_pkmn_gen1impl::base_pkmn_gen1impl(string identifier)
    {
        SQLite::Database db("@PKMNSIM_DB@"); //Filepath filled by CMake
        string query_string;

        query_string = str(boost::format("SELECT display_name FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        display_name = db.execAndGetStr(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT pokedex_num FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        nat_pokedex_num = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT species FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        species = db.execAndGetStr(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT height FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        height = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT weight FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        weight = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT type1 FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        type1 = db.execAndGetStr(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT type2 FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        type2 = db.execAndGetStr(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT base_hp FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        baseHP = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT base_atk FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        baseATK = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT base_def FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        baseDEF = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT base_spcl FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        baseSPCL = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT base_spd FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        baseSPD = db.execAndGet(query_string.c_str(), identifier);

        query_string = str(boost::format("SELECT exp_yield FROM pokedex WHERE identifier='%s'") % identifier.c_str());
        exp_yield = db.execAndGet(query_string.c_str(), identifier);
    }

    string base_pkmn_gen1impl::get_info()
    {
        string types_str;
        if(type2 == "None") types_str = type1;
        else types_str = str(boost::format("%s/%s") % type1 % type2);

        return str(boost::format(
            "%s (#%d)\n"
            "Type: %s\n"
            "Stats: %d, %d, %d, %d, %d"
            ) % display_name % nat_pokedex_num
              % types_str
              % baseHP % baseATK % baseDEF % baseSPD % baseSPCL
        );
    }

    string base_pkmn_gen1impl::get_info_verbose()
    {
        string types_str;
        if(type2 == "None") types_str = type1;
        else types_str = str(boost::format("%s/%s") % type1 % type2);

        return str(boost::format(
            "%s (#%d)\n"
            "%s Pokemon\n"
            "Type: %s\n"
            "%d m, %d kg\n"
            "Base Stats:\n"
            " - HP: %d\n"
            " - Attack: %d\n"
            " - Defense: %d\n"
            " - Speed: %d\n"
            " - Special: %d\n"
            " - Experience Yield: %d\n"
            ) % display_name % nat_pokedex_num
              % species
              % types_str
              % height % weight
              % baseHP
              % baseATK
              % baseDEF
              % baseSPD
              % baseSPCL
              % exp_yield
        );
    }

    map<string,int> base_pkmn_gen1impl::get_base_stats()
    {
        map<string,int> stats;
        stats["HP"] = baseHP;
        stats["ATK"] = baseATK;
        stats["DEF"] = baseDEF;
        stats["SPD"] = baseSPD;
        stats["SPCL"] = baseSPCL;
        return stats;
    }

    map<string,int> base_pkmn_gen1impl::get_ev_yields()
    {
        //In Generation 1, EV yields were the same as the corresponding base stat
        return get_base_stats();
    }

}