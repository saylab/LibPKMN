#pragma once

#include <string>

#include "sqlite3.h"
#include "pkmds_g5.h"

namespace pkmds
{

#ifndef __cplusplus_cli
extern sqlite3 *imgdatabase;
extern sqlite3_stmt *imgstatement;
void opendb(const char db_file[]);
void closedb();
void openimgdb(const char db_file[]);
void closeimgdb();
#endif
void getapic(const std::ostringstream &o, byte ** picdata, int * size);
void getapic(const std::string &str, byte ** picdata, int * size);
std::string getastring(const std::ostringstream &o);
std::wstring getawstring(const std::ostringstream &o);
int getanint(const std::ostringstream &o);
std::string getastring(const std::string &str);
//std::wstring getastd::wstring(const std::wstring &str);
int getanint(const std::string &str);
void dostatement(const std::string &cmd);
struct ivtest{
	byte id;
	byte val;
	byte order;
};
int getpkmlevel(const int id, const int exp);
int getpkmlevel(const pokemon_obj &pkm);
int getpkmlevel(const pokemon_obj *pkm);
int getpkmexptonext(const int id, const int exp);
int getpkmexptonext(const pokemon_obj &pkm);
int getpkmexptonext(const pokemon_obj *pkm);
int getpkmexpatcur(const int id, const int exp);
int getpkmexpatcur(const pokemon_obj &pkm);
int getpkmexpatcur(const pokemon_obj *pkm);
int getnatureincrease(const int natureid);
int getnatureincrease(const pokemon_obj &pkm);
int getnatureincrease(const pokemon_obj *pkm);
int getnaturedecrease(const int natureid);
int getnaturedecrease(const pokemon_obj &pkm);
int getnaturedecrease(const pokemon_obj *pkm);
int getpkmstat(const pokemon_obj &pkm, const Stat_IDs::stat_ids stat_id);
int getpkmstat(const pokemon_obj *pkm, const Stat_IDs::stat_ids stat_id);
bool pkmhasgenddiff(const int species);
bool pkmhasgenddiff(const pokemon_obj &pkm);
bool pkmhasgenddiff(const pokemon_obj *pkm);
int lookuppkmcolorid(const int species);
int lookuppkmcolorid(const pokemon_obj &pkm);
int lookuppkmcolorid(const pokemon_obj *pkm);
bool compareivbyval(const ivtest &a, const ivtest &b);
bool compareivbyid(const ivtest &a, const ivtest &b);
bool compareivbyorder(const ivtest &a, const ivtest &b);
void setlevel(pokemon_obj &pkm, int level);
void setlevel(pokemon_obj *pkm, int level);
int lookuppkmtype(const int species, const int form, const int slot, const int generation = GENERATION);
int lookuppkmtype(const pokemon_obj &pkm, const int slot, const int generation = GENERATION);
int lookuppkmtype(const pokemon_obj *pkm, const int slot, const int generation = GENERATION);
int lookuppkmevolvedspecies(int speciesid);
void pctoparty(party_pkm *ppkm, const pokemon_obj *pkm);
void pctoparty(party_pkm &ppkm, const pokemon_obj *pkm);
void pctoparty(party_pkm *ppkm, const pokemon_obj &pkm);
void pctoparty(party_pkm &ppkm, const pokemon_obj &pkm);
//void displaypkminconsole(pokemon_obj * pkm);
uint32 getpkmexpatlevel(const int id, const int level);
uint32 getpkmexpatlevel(const Species::species id, const int level);
int getmovepp(const Moves::moves moveid);
int getmovepp(const pokemon_obj * pkm, const int movenum);
int getmovetotalpp(const pokemon_obj * pkm, const int movenum);
Genders::genders calcpkmgender(const pokemon_obj * pkm);
int getpkmgenderrate(Species::species speciesid);
Genders::genders calcpkmgender(const pokemon_obj & pkm);
int getmovecategory(const Moves::moves moveid);
Types::types getmovetype(Moves::moves moveid);
int getmovepower(const Moves::moves moveid);
int getmoveaccuracy(const Moves::moves moveid);
std::string lookuppkmname(const int speciesid, const int langid = LANG_ID);
std::string lookuppkmname(const pokemon_obj &pkm, const int langid = LANG_ID);
std::string lookuppkmname(const pokemon_obj *pkm, const int langid = LANG_ID);
std::wstring lookuppkmnamewstring(const pokemon_obj *pkm, const int langid = LANG_ID);
std::string lookupmovename(const int moveid, const int langid = LANG_ID);
std::string lookupmovename(const pokemon_obj &pkm, const int movenum, const int langid = LANG_ID);
std::string lookupmovename(const pokemon_obj *pkm, const int movenum, const int langid = LANG_ID);
std::string lookupmoveflavortext(const uint16 moveid, const int langid = LANG_ID, const int versiongroup = VERSION_GROUP);
std::string lookupmoveflavortext(const pokemon_obj &pkm, const int movenum, const int langid = LANG_ID, const int versiongroup = VERSION_GROUP);
std::string lookupmoveflavortext(const pokemon_obj *pkm, const int movenum, const int langid = LANG_ID, const int versiongroup = VERSION_GROUP);
std::string lookupmovetypename(const int moveid, const int langid = LANG_ID);
std::string lookupmovetypename(const pokemon_obj &pkm, const int movenum, const int langid=LANG_ID);
std::string lookupmovetypename(const pokemon_obj *pkm, const int movenum, const int langid=LANG_ID);
std::string lookupmovedamagetypename(const uint16 moveid, const int langid = LANG_ID);
std::string lookupmovedamagetypename(const pokemon_obj &pkm, const int movenum, const int langid = LANG_ID);
std::string lookupmovedamagetypename(const pokemon_obj *pkm, const int movenum, const int langid = LANG_ID);
std::string getnaturename(const int natureid, const int langid = LANG_ID);
std::string getnaturename(const pokemon_obj &pkm, const int langid = LANG_ID);
std::string getnaturename(const pokemon_obj *pkm, const int langid = LANG_ID);
std::string lookupitemname(const int itemid, const int generation = GENERATION, const int langid = LANG_ID);
std::string lookupitemname(const pokemon_obj &pkm, const int generation = GENERATION, const int langid = LANG_ID);
std::string lookupitemname(const pokemon_obj *pkm, const int generation = GENERATION, const int langid = LANG_ID);
std::string getpkmgendername(const pokemon_obj &pkm);
std::string getpkmgendername(const pokemon_obj *pkm);
std::string lookupabilityname(const int abilityid, const int langid = LANG_ID);
std::string lookupabilityname(const pokemon_obj &pkm, const int langid = LANG_ID);
std::string lookupabilityname(const pokemon_obj *pkm, const int langid = LANG_ID);
std::string getpkmformname(const pokemon_obj &pkm, const int generation = GENERATION, const int langid = LANG_ID);
std::string getpkmformname(const pokemon_obj *pkm, const int generation = GENERATION, const int langid = LANG_ID);
std::string getpkmformname(const int speciesid, const int formid, const int generation = GENERATION, const int langid = LANG_ID);
std::string lookuplocname(const int locid, const int gen = GENERATION, const int langid = LANG_ID);
std::string getpkmmetlocname(const pokemon_obj &pkm, const int gen = GENERATION, const int langid = LANG_ID);
std::string getpkmmetlocname(const pokemon_obj *pkm, const int gen = GENERATION, const int langid = LANG_ID);
std::string getpkmegglocname(const pokemon_obj &pkm, const int gen = GENERATION, const int langid = LANG_ID);
std::string getpkmegglocname(const pokemon_obj *pkm, const int gen = GENERATION, const int langid = LANG_ID);
std::string lookupitemflavortext(const int itemid, const int generation = GENERATION, const int langid = LANG_ID, const int versiongroup = VERSION_GROUP);
std::string lookupitemflavortext(const pokemon_obj &pkm, const int generation = GENERATION, const int langid = LANG_ID, const int versiongroup = VERSION_GROUP);
std::string lookupitemflavortext(const pokemon_obj *pkm, const int generation = GENERATION, const int langid = LANG_ID, const int versiongroup = VERSION_GROUP);
std::string lookuppkmcolorname(const int species, const int langid = LANG_ID);
std::string lookuppkmcolorname(const pokemon_obj &pkm, const int langid = LANG_ID);
std::string lookuppkmcolorname(const pokemon_obj *pkm, const int langid = LANG_ID);
std::string lookupabilityflavortext(const int abilityid, const int version_group = VERSION_GROUP, const int langid = LANG_ID);
std::string lookupabilityflavortext(const pokemon_obj &pkm, const int version_group = VERSION_GROUP, const int langid = LANG_ID);
std::string lookupabilityflavortext(const pokemon_obj *pkm, const int version_group = VERSION_GROUP, const int langid = LANG_ID);
std::string lookupcharacteristic(const int statid, const int iv, const int langid = LANG_ID);
std::string lookupcharacteristic(const pokemon_obj &pkm, const int langid = LANG_ID);
std::string lookupcharacteristic(const pokemon_obj *pkm, const int langid = LANG_ID);
std::string lookuptypename(const Types::types type, const int langid = LANG_ID);
std::string lookuptypename(const int type, const int langid = LANG_ID);
std::string getmachinetypename(const Items::items itemid, const int generation = GENERATION, const int version_group = VERSION_GROUP);
std::string getpokemoncolorstring(const Species::species speciesid);
int getpokemoncolor(const Species::species speciesid);
uint32 getpkmcolor(const Species::species species);
std::string getmachinemovename(const Items::items itemid, const int generation = GENERATION, const int version_group = VERSION_GROUP, const int langid = LANG_ID);
int getitempocket(const Items::items itemid, const int generation = GENERATION);
std::string getitempocketname(const Items::items itemid, const int generation = GENERATION, const int langid = LANG_ID);
item_obj * finditeminbag(bw2sav_obj * sav, Items::items itemid, int & slot);
item_obj * getavailablebagslot(bw2sav_obj * sav, ItemPockets::itempockets pocket, int & slot);
void removeitem(bw2sav_obj * sav, pokemon_obj * pkm);
item_obj * getpocket(bw2sav_obj * sav, ItemPockets::itempockets pocket);
void tossitem(bw2sav_obj * sav, item_obj * item);
void giveitem(bw2sav_obj * sav, item_obj * item, pokemon_obj * pkm);
void insertitem(bw2sav_obj * sav, item_obj * item, int slot);

}
