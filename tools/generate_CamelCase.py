#!/usr/bin/env python
#
# Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

################################################################
# This script is called at build-time to generate the CamelCase
# names for LibPKMN's classes and functions.
################################################################

from header_converter import CppHeaderParser
import datetime
from optparse import OptionParser
import os

header_text = """/*
 * This file was generated: %s
 */""" % datetime.datetime.now()

ignored_files = ["config.hpp",
                 "dict.hpp",
                 "pkstring.hpp",
                 "shared_ptr.hpp"]

overrides = dict(prng = "PRNG",
                 seed_lcrng = "seedLCRNG",
                 lcrng = "LCRNG",
                 seed_arng = "seedARNG",
                 arng = "ARNG",
                 reset_mtrng = "resetMTRNG",
                 mtrng = "MTRNG",
                 get_appdata_path = "getAppDataPath",
                 get_libpkmn_commit = "getLibPKMNCommit",
                 get_sqlite3_version = "getSQLite3Version",
                 get_sqlitecpp_version = "getSQLiteCppVersion",
                 spatk_level1 = "SPAtkLevel1",
                 hp_level1 = "HPLevel1",
                 atk_level1 = "AtkLevel1",
                 spdef_level1 = "SPDefLevel1",
                 spd_level1 = "SpdLevel1",
                 def_level1 = "DefLevel1",
                 spatk_level2 = "SPAtkLevel2",
                 hp_level2 = "HPLevel2",
                 atk_level2 = "AtkLevel2",
                 spdef_level2 = "SPDefLevel2",
                 spd_level2 = "SpdLevel2",
                 def_level2 = "DefLevel2",
                 spatk_level3 = "SPAtkLevel3",
                 hp_level3 = "HPLevel3",
                 atk_level3 = "AtkLevel3",
                 spdef_level3 = "SPDefLevel3",
                 spd_level3 = "SpdLevel3",
                 def_level3 = "DefLevel3",
                 secret_troubles = "secretTroubles",
                 secret_leafstone = "secretLeafStone",
                 secret_firestone = "secretFireStone",
                 secret_waterstone = "secretWaterStone",
                 secret_followgoals = "secretFollowGoals",
                 secret_watchout = "secretWatchOut",
                 secret_lightningquick = "secretLightningQuick",
                 secret_longshots = "secretLongShots",
                 secret_scatterbug = "secretScatterbug",
                 secret_bitbots = "secretBitbots",
                 secret_hydreigon = "secretHydreigon",
                 secret_battlebest = "secretBattleBest",
                 LIBPKMN_PUBLIC_ID = "LIBPKMN_PUBLIC_ID",
                 LIBPKMN_SECRET_ID = "LIBPKMN_SECRET_ID",
                 LIBPKMN_TRAINER_ID = "LIBPKMN_TRAINER_ID",
                )

replacements = dict(Ev = "EV",
                    Id = "ID",
                    Iv = "IV",
                    Pp = "PP")

def generate_new_name(old_name, is_class):
    if old_name in overrides:
        return overrides[old_name]

    if old_name == "super_training_medals_t":
        old_name = "super_training_medals"
    elif old_name.endswith("_t"):
        old_name = old_name.replace("_t","")

    if is_class:
        new_name = old_name[0].upper() + ''.join(x for x in old_name.title()[1:] if not (x.isspace() or x == '_'))
    else:
        new_name = old_name[0].lower() + ''.join(x for x in old_name.title()[1:] if not (x.isspace() or x == '_'))

    for replacement in replacements:
        new_name = new_name.replace(replacement, replacements[replacement])

    return new_name

def generate_rename_line(old_name, is_class):
    return "" if (len(old_name) == 0 or "anon" in old_name.lower()) else "%%rename(%s) %s;" % (generate_new_name(old_name, is_class), old_name)

def convert_header(header):
    output = ""

    for fcn in header.functions:
        if "operator" not in fcn["name"].lower() and "anon" not in fcn["name"].lower():
            output += generate_rename_line(str(fcn["name"]), False) + "\n"

    for cls in header.classes:
        output += generate_rename_line(str(cls), True) + "\n"

        for fcn in header.classes[cls]["methods"]["public"]:
            if "operator" not in fcn["name"].lower() and not fcn["constructor"] and not fcn["destructor"]:
                output += generate_rename_line(fcn["name"], False) + "\n"

        for var in header.classes[cls]["properties"]["public"]:
            output += generate_rename_line(str(var["name"]), False) + "\n"

        for enum in header.classes[cls]._public_enums:
            output += generate_rename_line(str(header.classes[cls]._public_enums[enum]["name"]), True) + "\n"

    return output

if __name__ == "__main__":

    parser = OptionParser()
    parser.add_option("--include-dir", type="string", help="LibPKMN include directory")
    parser.add_option("--output-dir", type="string", help="CamelCase.i output directory")
    (options,args) = parser.parse_args()

    output = header_text + "\n\n"

    os.chdir(options.include_dir)
    for root, dirs, files in os.walk(os.getcwd()):
        for file in files:
            if file.endswith(".hpp") and file not in ignored_files and not root.endswith("conversions") and not root.endswith("native") and not root.endswith("qt4"):
                output += convert_header(CppHeaderParser.CppHeader(os.path.join(root, file)))

    output += "%rename(pocket) pkmn::item_entry_t::pocket;\n"
    output += "%rename(Pocket) pkmn::pocket;\n"

    os.chdir(options.output_dir)
    f = open("CamelCase.i", 'w')
    f.write(output)
    f.close()
