#!/usr/bin/env python
#
# Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import python as pkmn
import sys
import traceback

def database_usage_test():
    try:
        test_string = "Torrent"
        test_string2 = pkmn.database.get_ability_name(pkmn.database.get_ability_id(test_string))
        if test_string != test_string2:
            raise Exception("test_string != test_string2")

        test_num1 = pkmn.database.get_item_id(pkmn.database.get_item_name(pkmn.Items.POKE_BALL))
        if test_num1 != pkmn.Items.POKE_BALL:
            raise Exception("test_num1 != pkmn.Items.POKE_BALL")

        test_num2 = pkmn.database.get_species_id(pkmn.database.get_species_name(pkmn.Species.NIDORAN_M))
        if test_num2 != pkmn.Species.NIDORAN_M:
            raise Exception("test_num2 != pkmn.Species.NIDORAN_M")
        return True
    except:
        print
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

if __name__ == "__main__":

    successful = database_usage_test()

    sys.exit(0 if successful else 1)
