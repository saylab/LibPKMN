#!/usr/bin/env python
#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import python as pkmn

if __name__ == "__main__":

    testString = "Torrent"
    testString2 = pkmn.database.get_ability_name(pkmn.database.get_ability_id(testString))
    assert(testString == testString2)

    testnum1 = pkmn.database.get_item_id(pkmn.database.get_item_name(pkmn.Items.POKE_BALL))
    assert(testnum1 == pkmn.Items.POKE_BALL)

    testnum2 = pkmn.database.get_species_id(pkmn.database.get_species_name(pkmn.Species.NIDORAN_M))
    assert(testnum2 == pkmn.Species.NIDORAN_M)