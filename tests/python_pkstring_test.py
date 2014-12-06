#!/usr/bin/env python
#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import python as LibPKMN

if __name__ == "__main__":

    testString = "Torrent"
    testString2 = LibPKMN.database.get_ability_name(LibPKMN.database.get_ability_id(testString))
    assert(testString == testString2)

    testnum1 = LibPKMN.database.get_item_id(LibPKMN.database.get_item_name(LibPKMN.Items.POKE_BALL))
    assert(testnum1 == LibPKMN.Items.POKE_BALL)

    testnum2 = LibPKMN.database.get_species_id(LibPKMN.database.get_species_name(LibPKMN.Species.NIDORAN_M))
    assert(testnum2 == LibPKMN.Species.NIDORAN_M)