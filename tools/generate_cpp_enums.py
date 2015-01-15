#!/usr/bin/env python
#
# Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

###############################################################
# This script is to be manually run when LibPKMN's database
# is updated. This isn't called at build-time for two reasons:
# a public header shouldn't be generated at build-time, and it
# removes a build-time dependency on PySQLite3 and Unidecode.
###############################################################

import datetime
from optparse import OptionParser
import os
import sqlite3
from unidecode import unidecode

abilities = []
balls = []
egg_groups = []
forms = []
genders = []
items = []
moves = []
move_damage_classes = []
natures = []
ribbons = []
species = []
stats = []
types = []
versions = []
version_groups = []
db_table_output = ""

def get_abilities(c):
    global abilities

    c.execute("SELECT ability_id,name FROM ability_names WHERE local_language_id=9 AND ability_id<10000")
    from_db = c.fetchall()
    abilities = [(0,"NONE")]

    for i in range(len(from_db)):
        ability_name = str(from_db[i][1]).replace("-","_").replace(" ","_").upper()
        abilities += [(from_db[i][0], ability_name)]

def get_balls(c):
    global balls

    c.execute("SELECT * FROM balls")
    from_db = c.fetchall()
    balls = [(0,"UNKNOWN")]

    for i in range(len(from_db)):
        ball_name = str(unidecode(from_db[i][1])).replace("-","_").replace(" ","_").replace(".","").replace("'","").upper()
        balls += [(from_db[i][0], ball_name)]

def get_egg_groups(c):
    global egg_groups

    c.execute("SELECT * FROM egg_groups")
    from_db = c.fetchall()
    egg_groups = [(0,"NONE")]

    for i in range(len(from_db)):
        egg_group_name = str(from_db[i][1]).replace("-","_").replace(" ","_").upper()
        egg_groups += [(from_db[i][0], egg_group_name)]

def get_forms(c):
    global forms

    c.execute("SELECT DISTINCT species_id FROM pokemon WHERE id IN (SELECT pokemon_id FROM pokemon_forms WHERE form_identifier != 'NULL' ORDER BY pokemon_id) ORDER BY species_id")
    species_ids = c.fetchall()

    for i in range(len(species_ids)):
        c.execute("SELECT name FROM pokemon_species_names WHERE pokemon_species_id=%d AND local_language_id=9" % species_ids[i][0])
        species_name = str(unidecode(c.fetchone()[0])).replace("-","_").replace(" ","_").replace(".","").replace("'","")

        c.execute("SELECT pokemon_form_id,form_name FROM pokemon_form_names WHERE pokemon_form_id IN (SELECT id FROM pokemon_forms WHERE pokemon_id IN (SELECT id FROM pokemon WHERE species_id=%s)) AND local_language_id=9" % species_ids[i][0])
        species_forms = c.fetchall()

        form_index = []
        form_index += [species_name]
        for j in range(len(species_forms)):
            form_name = "STANDARD" if species_forms[j][1] == None else str(unidecode(species_forms[j][1])).replace("-","_").replace(" ","_").replace(".","").replace("'","").upper()
            form_name = form_name.replace("_FORME","").replace("_FORM","").replace("_TYPE","").replace("_ROTOM","").replace("???","QUESTION_MARK").replace("!","EXCLAMATION_MARK")
            form_name = form_name.replace("?","QUESTION_MARK").replace("_PATTERN","").replace("_KYUREM","").replace("_MODE","")

            if "MEGA" in form_name and "_X" in form_name:
                form_name = "MEGA_X"
            elif "MEGA" in form_name and "_Y" in form_name:
                form_name = "MEGA_Y"
            elif "MEGA" in form_name:
                form_name = "MEGA"

            form_index += [(species_forms[j][0], form_name)]

        forms += [form_index]

def get_genders():
    global genders

    genders = [(0,"FEMALE"),(1,"MALE"),(2,"GENDERLESS")]

def get_items(c):
    global items

    c.execute("SELECT item_id,name FROM item_names WHERE local_language_id=9")
    from_db = c.fetchall()
    items = [(0,"NONE")]

    for i in range(len(from_db)):
        item_name = str(unidecode(from_db[i][1])).replace("-","_").replace(" ","_").replace(".","").replace("'","").upper()
        items += [(from_db[i][0], item_name)]

    items += [(100000, "INVALID")]

def get_moves(c):
    global moves

    c.execute("SELECT move_id,name FROM move_names WHERE local_language_id=9")
    from_db = c.fetchall()
    moves = [(0,"NONE")]

    for i in range(len(from_db)):
        move_name = str(from_db[i][1]).replace("-","_").replace(" ","_").replace("'","").upper().replace("SING","__SING")
        moves += [(from_db[i][0], move_name)]

    moves += [(100000, "INVALID")]

def get_move_damage_classes(c):
    global move_damage_classes

    c.execute("SELECT * FROM move_damage_classes")
    from_db = c.fetchall()
    move_damage_classes = [(0,"NONE")]

    for i in range(len(from_db)):
        move_damage_class_name = str(from_db[i][1]).upper()
        move_damage_classes += [(from_db[i][0], move_damage_class_name)]

def get_natures(c):
    global natures

    c.execute("SELECT nature_id,name FROM nature_names WHERE local_language_id=9")
    from_db = c.fetchall()
    natures = [(0,"NONE")]

    for i in range(len(from_db)):
        nature_name = str(from_db[i][1]).upper()
        natures += [(from_db[i][0], nature_name)]

def get_ribbons():
    global ribbons

    hoenn_ribbons = ["WORLD","EARTH","NATIONAL","COUNTRY","SKY","LAND","MARINE","EFFORT","ARTIST","VICTORY","WINNING","CHAMPION",
                     "TOUGH_MASTER","TOUGH_HYPER","TOUGH_SUPER","TOUGH",
                     "SMART_MASTER","SMART_HYPER","SMART_SUPER","SMART",
                     "CUTE_MASTER","CUTE_HYPER","CUTE_SUPER","CUTE",
                     "BEAUTY_MASTER","BEAUTY_HYPER","BEAUTY_SUPER","BEAUTY",
                     "COOL_MASTER","COOL_HYPER","COOL_SUPER","COOL"]

    sinnoh_ribbons = ["","","","",
                      "TOUGH_MASTER","TOUGH_HYPER","TOUGH_SUPER","TOUGH",
                      "SMART_MASTER","SMART_HYPER","SMART_SUPER","SMART",
                      "CUTE_MASTER","CUTE_HYPER","CUTE_SUPER","CUTE",
                      "BEAUTY_MASTER","BEAUTY_HYPER","BEAUTY_SUPER","BEAUTY",
                      "COOL_MASTER","COOL_HYPER","COOL_SUPER","COOL",
                      "","","","",
                      "PREMIER","CLASSIC","CARNIVAL","FESTIVAL","BLUE","GREEN","RED","LEGEND","HISTORY","RECORD",
                      "FOOTPRINT","GORGEOUS_ROYAL","ROYAL","GORGEOUS","SMILE","SNOOZE","RELAX","CARELESS",
                      "DOWNCAST","SHOCK","ALERT","WORLD_ABILITY","PAIR_ABILITY","MULTI_ABILITY","DOUBLE_ABILITY",
                      "GREAT_ABILITY","ABILITY","CHAMPION"]


    unova_ribbons = ["","","","",
                     "TOUGH_MASTER","TOUGH_HYPER","TOUGH_SUPER","TOUGH",
                     "SMART_MASTER","SMART_HYPER","SMART_SUPER","SMART",
                     "CUTE_MASTER","CUTE_HYPER","CUTE_SUPER","CUTE",
                     "BEAUTY_MASTER","BEAUTY_HYPER","BEAUTY_SUPER","BEAUTY",
                     "COOL_MASTER","COOL_HYPER","COOL_SUPER","COOL",
                     "","","","",
                     "PREMIER","CLASSIC","WISHING","SOUVENIR","SPECIAL","BIRTHDAY","WORLD_CHAMPION","LEGEND",
                     "EVENT","RECORD","FOOTPRINT","GORGEOUS_ROYAL","ROYAL","GORGEOUS","SMILE","SNOOZE","RELAX","CARELESS",
                     "DOWNCAST","SHOCK","ALERT","WORLD_ABILITY","PAIR_ABILITY","MULTI_ABILITY","DOUBLE_ABILITY",
                     "GREAT_ABILITY","ABILITY","CHAMPION"]

    kalos_ribbons = ["","","","WORLD_CHAMPION","NATIONAL_CHAMPION","REGIONAL_CHAMPION","BATTLE_CHAMPION",
                     "WISHING","FESTIVAL","SPECIAL","BIRTHDAY","EVENT","PREMIER","CLASSIC","WORLD",
                     "EARTH","NATIONAL","COUNTRY","LEGEND","RECORD","FOOTPRINT","ARTIST","GORGEOUS_ROYAL",
                     "ROYAL","GORGEOUS","SMILE","SNOOZE","RELAX","CARELESS","DOWNCAST","SHOCK","ALERT",
                     "EFFORT","","SKILLFUL_BATTLER","TRAINING","BEST_FRIENDS","SINNOH_CHAMPION","CHAMPION",
                     "KALOS_CHAMPION"]

    ribbons = [hoenn_ribbons, sinnoh_ribbons, unova_ribbons, kalos_ribbons]

def get_species(c):
    global species

    c.execute("SELECT pokemon_species_id,name FROM pokemon_species_names WHERE local_language_id=9")
    from_db = c.fetchall()
    species = [(0,"NONE")]

    for i in range(len(from_db)):
        if from_db[i][0] == 29:
            species_name = "NIDORAN_F"
        elif from_db[i][0] == 32:
            species_name = "NIDORAN_M"
        else:
            species_name = str(unidecode(from_db[i][1])).replace("-","_").replace(" ","_").replace(".","").replace("'","").upper()

        species += [(from_db[i][0], species_name)]

    species += [(10000, "INVALID")]

def get_stats(c):
    global stats

    c.execute("SELECT stat_id,name FROM stat_names WHERE local_language_id=9")
    from_db = c.fetchall()
    stats = [(0,"NONE")]

    for i in range(len(from_db)):
        stat_name = str(from_db[i][1]).replace(" ","_").upper()
        stats += [(from_db[i][0], stat_name)]

def get_types(c):
    global types

    c.execute("SELECT type_id,name FROM type_names WHERE local_language_id=9")
    from_db = c.fetchall()
    types = [(0,"NONE")]

    for i in range(len(from_db)):
        type_name = str(from_db[i][1]).replace("-","_").replace(" ","_").replace("???","QUESTION_MARK").upper()
        types += [(from_db[i][0], type_name)]

def get_versions(c):
    global versions

    c.execute("SELECT version_id,name FROM version_names WHERE local_language_id=9")
    from_db = c.fetchall()
    versions = [(0,"NONE")]

    for i in range(len(from_db)):
        version_name = str(from_db[i][1]).replace("-","_").replace(" ","_").replace("???","QUESTION_MARK").upper()
        versions += [(from_db[i][0], version_name)]

def get_version_groups(c):
    global version_groups

    c.execute("SELECT id,identifier FROM version_groups")
    from_db = c.fetchall()
    version_groups = [(0,"NONE")]

    for i in range(len(from_db)):
        version_group_name = str(from_db[i][1]).replace("-","_").replace(" ","_").upper()
        version_groups += [(from_db[i][0], version_group_name)]

def generate_enums_hpp(output_dir, license):
    global db_table_output

    output = license + """

#ifndef INCLUDED_PKMN_ENUMS_HPP
#define INCLUDED_PKMN_ENUMS_HPP

"""

    output += """namespace pkmn
{
    namespace Abilities
    {
        enum abilities
        {"""

    for i in range(len(abilities)):
        if i > 0 and abilities[i][0] > (abilities[i-1][0]+1):
            output += """
            %s = %d,""" % (abilities[i][1], abilities[i][0])
        else:
            output += """
            %s,""" % abilities[i][1]

    output += """
        };
    }

    namespace Balls
    {
        enum balls
        {"""

    for i in range(len(balls)):
        output += """
            %s,""" % balls[i][1]

    output += """
        };
    }

    namespace Egg_Groups
    {
        enum egg_groups
        {"""

    for i in range(len(egg_groups)):
        if i > 0 and egg_groups[i][0] > (egg_groups[i-1][0]+1):
            output += """
            %s = %d,""" % (egg_groups[i][1], egg_groups[i][0])
        else:
            output += """
            %s,""" % egg_groups[i][1]

    output += """
        };
    }

    namespace Forms
    {
"""
    for i in range(len(forms)):
        output += """        namespace %s
        {
            enum forms
            {""" % forms[i][0]

        for j in range(1,len(forms[i])):
            output += "\n                %s = %d," % (forms[i][j][1], forms[i][j][0])

            form_name = forms[i][j][1].replace("_","-").replace("Mega-","Mega ").title()
            form_image_name = form_name.lower().replace("-mark","").replace("mega x","mega-x").replace("mega y","mega-y").replace("-cloak","")
            form_image_name = "'%s'" % form_image_name if form_image_name != 'standard' else "NULL"
            db_table_output += "INSERT INTO \"libpkmn_pokemon_form_names\" VALUES(%d,\'%s\',%s);\n" % (forms[i][j][0],
                                   form_name, form_image_name)


        output += """
            };
        }
"""

    output += """    }

    namespace Genders
    {
        enum genders
        {"""

    for i in range(len(genders)):
        output += """
            %s,""" % genders[i][1]

    output += """
        };
    }

    namespace Items
    {
        enum items
        {"""

    for i in range(len(items)):
        if i > 0 and items[i][0] > (items[i-1][0]+1):
            output += """
            %s = %d,""" % (items[i][1], items[i][0])
        else:
            output += """
            %s,""" % items[i][1]

    output += """
        };
    }

    namespace Moves
    {
        enum moves
        {"""

    for i in range(len(moves)):
        if i > 0 and moves[i][0] > (moves[i-1][0]+1):
            output += """
            %s = %d,""" % (moves[i][1], moves[i][0])
        else:
            output += """
            %s,""" % moves[i][1]

    output += """
        };
    }

    namespace Move_Classes
    {
        enum move_classes
        {"""

    for i in range(len(move_damage_classes)):
        output += """
            %s,""" % move_damage_classes[i][1]

    output += """
        };
    }

    namespace Natures
    {
        enum natures
        {"""

    for i in range(len(natures)):
        output += """
            %s,""" % natures[i][1]

    output += """
        };
    }

    namespace Ribbons
    {"""

    output += """
        namespace Hoenn
        {
            enum hoenn_ribbons
            {"""

    for i in range(len(ribbons[0])):
        if ribbons[0][i] != "":
            output += """
                %s = %d,""" % (ribbons[0][i], i)

    output += """
            };
        }
        namespace Sinnoh
        {
            enum sinnoh_ribbons
            {"""

    for i in range(len(ribbons[1])):
        if ribbons[1][i] != "":
            output += """
                %s = %d,""" % (ribbons[1][i], i)

    output += """
            };
        }
        namespace Unova
        {
            enum unova_ribbons
            {"""

    for i in range(len(ribbons[2])):
        if ribbons[2][i] != "":
            output += """
                %s = %d,""" % (ribbons[2][i], i)

    output += """
            };
        }
        namespace Kalos
        {
            enum kalos_ribbons
            {"""

    for i in range(len(ribbons[3])):
        if ribbons[3][i] != "":
            output += """
                %s = %d,""" % (ribbons[3][i], i)

    output += """
            };
        }
    }

    namespace Species
    {
        enum species
        {"""

    for i in range(len(species)):
        output += """
            %s,""" % species[i][1]

    output += """
        };
    }

    namespace Stats
    {
        enum stats
        {"""

    for i in range(len(stats)):
        output += """
            %s,""" % stats[i][1]

    output += """
        };
    }

    namespace Types
    {
        enum types
        {"""

    for i in range(len(types)):
        if i > 0 and types[i][0] > (types[i-1][0]+1):
            output += """
            %s = %d,""" % (types[i][1], types[i][0])
        else:
            output += """
            %s,""" % types[i][1]

    output += """
        };
    }

    namespace Versions
    {
        enum versions
        {"""

    for i in range(len(versions)):
        output += """
            %s,""" % versions[i][1]

    output += """
        };
    }

    namespace Version_Groups
    {
        enum version_groups
        {"""

    for i in range(len(version_groups)):
        output += """
            %s,""" % version_groups[i][1]

    output += """
        };
    }
}

#endif /* INCLUDED_PKMN_ENUMS_HPP */"""

    os.chdir(output_dir)
    f = open("enums.hpp",'w')
    f.write(output)
    f.close()

def generate_db_table(my_path):
    global db_table_output
    db_table_output += "COMMIT;"

    os.chdir(my_path)
    f = open("libpkmn_pokemon_form_names",'w')
    f.write(db_table_output)
    f.close()

if __name__ == "__main__":

    my_path = os.path.dirname(os.path.realpath(__file__))

    parser = OptionParser()
    parser.add_option("--database-path", type="string", help="LibPKMN database location")
    parser.add_option("--output-dir", type="string", help="Output directory")
    parser.add_option("--create-form-table", action="store_true", default=False, help="Generate libpkmn_pokemon_form_names table")
    (options,args) = parser.parse_args()

    conn = sqlite3.connect(options.database_path)
    c = conn.cursor()

    time = datetime.datetime.now()

    license = """/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 *
 * This file was generated: %s
 */""" % time

    db_table_output = """CREATE TABLE libpkmn_pokemon_form_names (
    form_id INTEGER NOT NULL,
    name VARCHAR(30) NOT NULL,
    image_name VARCHAR(30),
    PRIMARY KEY (form_id)
);\n"""

    get_abilities(c)
    get_balls(c)
    get_egg_groups(c)
    get_forms(c)
    get_genders()
    get_items(c)
    get_moves(c)
    get_move_damage_classes(c)
    get_natures(c)
    get_ribbons()
    get_species(c)
    get_stats(c)
    get_types(c)
    get_versions(c)
    get_version_groups(c)

    generate_enums_hpp(options.output_dir, license)
    if options.create_form_table:
        generate_db_table(my_path)
