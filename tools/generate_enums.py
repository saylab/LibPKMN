#!/usr/bin/env python
#
# Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

###############################################################
# This script is called at build-time to generate the database
# enums for the SWIG-wrapped libraries. Values are taken
# directly from include/pkmn/enums.hpp so as to guarantee all
# values match.
###############################################################

from header_converter import CppHeaderParser
import datetime
from optparse import OptionParser
import os

abilities = []
egg_groups = []
forms = []
genders = []
items = []
moves = []
move_damage_classes = []
natures = []
pokeballs = []
species = []
stats = []
types = []
versions = []
version_groups = []

def get_abilities(e):
    global abilities

    for value in e["values"]:
        abilities += [(value["name"], value["value"])]

def get_egg_groups(e):
    global egg_groups

    for value in e["values"]:
        egg_groups += [(value["name"], value["value"])]

def get_forms(e):
    global forms

    form_index = []
    form_index += [e["namespace"].split("::")[-2]]

    for value in e["values"]:
        form_index += [(value["name"], value["value"])]

    forms += [form_index]

def get_genders():
    global genders

    genders = [("FEMALE",0),("MALE",1),("GENDERLESS",2)]

def get_items(c):
    global items

    for value in e["values"]:
        items += [(value["name"], value["value"])]

def get_moves(c):
    global moves

    for value in e["values"]:
        moves += [(value["name"], value["value"])]

def get_move_damage_classes(c):
    global move_damage_classes

    for value in e["values"]:
        move_damage_classes += [(value["name"], value["value"])]

def get_natures(c):
    global natures

    for value in e["values"]:
        natures += [(value["name"], value["value"])]

def get_pokeballs():
    global pokeballs

    pokeballs = [("UNKNOWN",0),("POKE_BALL",1),("GREAT_BALL",2),("ULTRA_BALL",3),("MASTER_BALL",4),("SAFARI_BALL",5),
                 ("LEVEL_BALL",6),("LURE_BALL",7),("MOON_BALL",8),("FRIEND_BALL",8),("LOVE_BALL",9),("HEAVY_BALL",10),
                 ("FAST_BALL",11),("SPORT_BALL",12),("PREMIER_BALL",13),("REPEAT_BALL",14),("TIMER_BALL",15),
                 ("NEST_BALL",16),("NET_BALL",17),("DIVE_BALL",18),("LUXURY_BALL",19),("HEAL_BALL",20),
                 ("QUICK_BALL",21),("DUSK_BALL",22),("CHERISH_BALL",23),("PARK_BALL",24),("DREAM_BALL",25)]

def get_species(c):
    global species

    for value in e["values"]:
        species += [(value["name"], value["value"])]

def get_stats(c):
    global stats

    for value in e["values"]:
        stats += [(value["name"], value["value"])]

def get_types(c):
    global types

    for value in e["values"]:
        types += [(value["name"], value["value"])]

def get_versions(c):
    global versions

    for value in e["values"]:
        versions += [(value["name"], value["value"])]

def get_version_groups(c):
    global version_groups

    for value in e["values"]:
        version_groups += [(value["name"], value["value"])]

def generate_python_files(output_dir, python_license):
    os.chdir(output_dir)

    f = open("Abilities.py",'w')
    f.write(python_license + "\n\n")

    for ability in abilities:
        f.write("%s = %d\n" % (ability[0],ability[1]))

    f.close()

    f = open("Egg_Groups.py",'w')
    f.write(python_license + "\n\n")

    for egg_group in egg_groups:
        f.write("%s = %d\n" % (egg_group[0],egg_group[1]))

    f.close()

    f = open("Forms.py",'w')
    f.write(python_license + "\n")

    for i in range(len(forms)):
        f.write("\nclass %s:" % forms[i][0])

        for j in range(1,len(forms[i])):
            f.write("\n    %s = %d" % (forms[i][j][0], forms[i][j][1]))

        f.write("\n")

    f.close()

    f = open("Genders.py",'w')
    f.write(python_license + "\n\n")

    for gender in genders:
        f.write("%s = %d\n" % (gender[0],gender[1]))

    f.close()

    f = open("Items.py",'w')
    f.write(python_license + "\n\n")

    for item in items:
        f.write("%s = %d\n" % (item[0],item[1]))

    f.close()

    f = open("Moves.py",'w')
    f.write(python_license + "\n\n")

    for move in moves:
        f.write("%s = %d\n" % (move[0],move[1]))

    f.close()

    f = open("Move_Classes.py",'w')
    f.write(python_license + "\n\n")

    for move_damage_class in move_damage_classes:
        f.write("%s = %d\n" % (move_damage_class[0],move_damage_class[1]))

    f.close()

    f = open("PokeBalls.py",'w')
    f.write(python_license + "\n\n")

    for pokeball in pokeballs:
        f.write("%s = %d\n" % (pokeball[0],pokeball[1]))

    f.close()

    f = open("Natures.py",'w')
    f.write(python_license + "\n\n")

    for nature in natures:
        f.write("%s = %d\n" % (nature[0],nature[1]))

    f.close()

    f = open("Species.py",'w')
    f.write(python_license + "\n\n")

    for pokemon in species:
        f.write("%s = %d\n" % (pokemon[0],pokemon[1]))

    f.close()

    f = open("Stats.py",'w')
    f.write(python_license + "\n\n")

    for stat in stats:
        f.write("%s = %d\n" % (stat[0],stat[1]))

    f.close()

    f = open("Types.py",'w')
    f.write(python_license + "\n\n")

    for _type in types:
        f.write("%s = %d\n" % (_type[0],_type[1]))

    f.close()

    f = open("Versions.py",'w')
    f.write(python_license + "\n\n")

    for version in versions:
        f.write("%s = %d\n" % (version[0],version[1]))

    f.close()

    f = open("Version_Groups.py",'w')
    f.write(python_license + "\n\n")

    for version_group in version_groups:
        f.write("%s = %d\n" % (version_group[0],version_group[1]))

    f.close()

def generate_cs_files(output_dir, license):
    os.chdir(output_dir)

    #Non-nested enums can be done in a loop
    enums = dict()
    enums["Abilities"] = abilities
    enums["EggGroups"] = egg_groups
    enums["Genders"] = genders
    enums["Items"] = items
    enums["Moves"] = moves
    enums["MoveClasses"] = move_damage_classes
    enums["Natures"] = natures
    enums["Species"] = species
    enums["Stats"] = stats
    enums["PokeBalls"] = pokeballs
    enums["Types"] = types
    enums["Versions"] = versions
    enums["VersionGroups"] = version_groups

    for key in enums:
        f = open("%s.cs" % key,'w')
        f.write(license + "\n\n")

        f.write("""namespace PKMN
{
    public static class %s
    {""" % key)

        for i in range(len(enums[key])):
            f.write("\n        public const uint %s = %d;" % (enums[key][i][0],enums[key][i][1]))

        f.write("""
    }
}""")

        f.close()

    f = open("PokemonForms.cs",'w')
    f.write(license + "\n\n")

    f.write("""namespace PKMN
{
    namespace PokemonForms
    {""")

    for i in range(len(forms)):
        f.write("""
        public static class %s
        {""" % forms[i][0])

        for j in range(1,len(forms[i])):
            f.write("""
            public const uint %s = %d;""" % (forms[i][j][0],forms[i][j][1]))

        f.write("""
        }
""")

    f.write("""
    }
}""")

    f.close()

def generate_java_files(output_dir, license):
    os.chdir(output_dir)

    #Non-nested enums can be done in a loop
    enums = dict()
    enums["Abilities"] = abilities
    enums["EggGroups"] = egg_groups
    enums["Genders"] = genders
    enums["Items"] = items
    enums["Moves"] = moves
    enums["MoveClasses"] = move_damage_classes
    enums["Natures"] = natures
    enums["Species"] = species
    enums["Stats"] = stats
    enums["PokeBalls"] = pokeballs
    enums["Types"] = types
    enums["Versions"] = versions
    enums["VersionGroups"] = version_groups

    for key in enums:
        f = open("%s.java" % key,'w')
        f.write(license + "\n\n")

        f.write("package nc.PKMN;\n\n");

        f.write("""public class %s
{""" % key)

        for i in range(len(enums[key])):
            f.write("""
    public static final int %s = %d;""" % (enums[key][i][0],enums[key][i][1]))

        f.write("""
}""")

        f.close()

    f = open("PokemonForms.java",'w')
    f.write(license + "\n\n")

    f.write("package nc.PKMN;\n\n")

    f.write("""public class PokemonForms
{""")

    for i in range(len(forms)):
        f.write("""
    public class %s
    {""" % forms[i][0])

        for j in range(1,len(forms[i])):
            f.write("""
        public static final int %s = %d;""" % (forms[i][j][0], forms[i][j][1]))

        f.write("""
    }""")

    f.write("""
}""")

    f.close()

if __name__ == "__main__":

    parser = OptionParser()
    parser.add_option("--include-path", type="string", help="enums.hpp location")
    parser.add_option("--output-dir", type="string", help="Output directory")
    parser.add_option("--language", type="string", help="cpp, python, cs, or java")
    (options,args) = parser.parse_args()

    time = datetime.datetime.now()

    license = """/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 *
 * This file was generated: %s
 */""" % time

    python_license = """#!/usr/bin/env python
#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#
# This file was generated: %s
#""" % time

    header = CppHeaderParser.CppHeader(options.include_path)
    enums = header.__dict__["enums"]
    for e in enums:
        if e["namespace"] == "pkmn::Abilities::":
            get_abilities(e)
        elif e["namespace"] == "pkmn::Egg_Groups::":
            get_egg_groups(e)
        elif "Forms" in e["namespace"]:
            get_forms(e)
        elif e["namespace"] == "pkmn::Items::":
            get_items(e)
        elif e["namespace"] == "pkmn::Moves::":
            get_moves(e)
        elif e["namespace"] == "pkmn::Move_Classes::":
            get_move_damage_classes(e)
        elif e["namespace"] == "pkmn::Natures::":
            get_natures(e)
        elif e["namespace"] == "pkmn::Species::":
            get_species(e)
        elif e["namespace"] == "pkmn::Stats::":
            get_stats(e)
        elif e["namespace"] == "pkmn::Types::":
            get_types(e)
        elif e["namespace"] == "pkmn::Versions::":
            get_versions(e)
        elif e["namespace"] == "pkmn::Version_Groups::":
            get_version_groups(e)

    #These don't need the header file
    get_genders()
    get_pokeballs()

    if options.language == "python":
        generate_python_files(options.output_dir, python_license)
    elif options.language == "cs":
        generate_cs_files(options.output_dir, license)
    elif options.language == "java":
        generate_java_files(options.output_dir, license)
