#!/usr/bin/env python
#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import python as LibPKMN

if __name__ == "__main__":

    print LibPKMN.database.get_ability_id("Scrappy")
    print LibPKMN.database.get_ability_name(LibPKMN.Abilities.SCRAPPY)
    t = LibPKMN.trainer("Ruby","LibPKMN","Male") #Will throw if pkstring doesn't work
