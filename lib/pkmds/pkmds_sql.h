#pragma once
#include "pkmds_g5_sqlite.h"

namespace pkmds
{
std::string getpkmstatsql(const pokemon_obj &pkm, const Stat_IDs::stat_ids stat_id);
std::string getpkmstatsql(const pokemon_obj *pkm, const Stat_IDs::stat_ids stat_id);
void getspritesql(std::ostringstream& o, const pokemon_obj & pkm, int generation = GENERATION);
void getspritesql(std::ostringstream& o, const pokemon_obj * pkm, int generation = GENERATION);
void geticonsql(std::ostringstream& o, const pokemon_obj & pkm, int generation = GENERATION);
void geticonsql(std::ostringstream& o, const pokemon_obj * pkm, int generation = GENERATION);
void gettypesql(std::ostringstream& o, const Types::types type);
void gettypesql(std::ostringstream& o, const int type);
void getwallpapersql(std::ostringstream& o, const int wallpaper);
void getwallpapersql(std::ostringstream& o, const Wallpapers::wallpapers wallpaper);
void getitemsql(std::ostringstream& itemsql, const uint16 itemid, const int generation = GENERATION);
void getmarkingsql(std::ostringstream& o, const Markings::markings mark, const bool marked);
void getballsql(std::ostringstream& o, const Balls::balls ball, const int generation = GENERATION);
std::string getspeciesnamesql(const int speciesid, const int langid = LANG_ID);
std::string getmovenamesql(const int moveid, const int langid = LANG_ID);
std::string getmoveflavortextsql(const uint16 moveid, const int langid = LANG_ID, const int versiongroup = VERSION_GROUP);
std::string getmovetypenamesql(const uint16 moveid, const int langid = LANG_ID);
std::string getmovedamagetypenamesql(const uint16 moveid, const int langid = LANG_ID);
std::string getpkmlevelsql(const int id, const int exp);
std::string getpkmexptonextsql(const int id, const int exp);
std::string getpkmexpatcursql(const int id, const int exp);
std::string getpkmexpatlevelsql(const int id, const int level);
std::string getpkmexpatlevelsql(const Species::species id, const int level);
std::string getnaturenamesql(const int natureid, const int langid = LANG_ID);
std::string getmoveppsql(const Moves::moves moveid);
std::string lookuppkmevolvedspeciessql(int speciesid);
std::string getsetlevelsql(pokemon_obj *pkm, int level);
std::string getsetlevelsql(pokemon_obj &pkm, int level);
std::string getpkmformnamesql(const pokemon_obj *pkm, const int generation = GENERATION, const int langid = LANG_ID);
std::string getpkmformnamesql(const int speciesid, const int formid, const int generation = GENERATION, const int langid = LANG_ID);
std::string getpkmformnamesql(const pokemon_obj &pkm, const int generation = GENERATION, const int langid = LANG_ID);
std::string getnatureincreasesql(const int natureid);
std::string getnaturedecreasesql(const int natureid);
std::string lookupitemnamesql(const int itemid, const int generation = GENERATION, const int langid = LANG_ID);
std::string pkmhasgenddiffsql(const int species);
std::string lookupabilitynamesql(const int abilityid, const int langid = LANG_ID);
std::string lookuplocnamesql(const int locid, const int gen, const int langid = LANG_ID);
std::string lookupitemflavortextsql(const int itemid, const int generation = GENERATION, const int langid = LANG_ID, const int versiongroup = VERSION_GROUP);
std::string lookuppkmcolornamesql(const int species, const int langid = LANG_ID);
std::string lookuppkmcoloridsql(const int species);
std::string lookupabilityflavortextsql(const int abilityid, const int version_group = VERSION_GROUP, const int langid = LANG_ID);
std::string lookupcharacteristicsql(const int statid, const int iv, const int langid = LANG_ID);
std::string lookuptypenamesql(const int type, const int langid = LANG_ID);
void getmovecatsql(std::ostringstream& o, const Moves::moves moveid);
std::string getmachinetypesql(const Items::items itemid, const int generation = GENERATION, const int version_group = VERSION_GROUP);
std::string getmachinemovenamesql(const Items::items itemid, const int generation = GENERATION, const int version_group = VERSION_GROUP, const int langid = LANG_ID);
std::string getpokemoncolorstringsql(const Species::species speciesid);
std::string getpokemoncolorsql(const Species::species speciesid);
std::string getribbonsql(std::string ribbon, bool hoenn);
std::string getitempocketsql(const Items::items itemid, const int generation = GENERATION);
std::string getitempocketnamesql(const Items::items itemid, const int generation = GENERATION, const int langid = LANG_ID);
}
