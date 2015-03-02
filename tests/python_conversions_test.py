#!/usr/bin/env python
#
# Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import python as pkmn
import sys
import traceback

def gen1_conversions_test():
    try:
        pk1 = pkmn.pokemon("Pikachu", "Red", 50, "Tackle", "Tail Whip", "Thunder", "Surf")
        exported = pkmn.conversions.export_gen1_pokemon(pk1)
        pk2 = pkmn.conversions.import_gen1_pokemon(exported)
        entry1 = pk1.get_pokedex_entry()
        entry2 = pk2.get_pokedex_entry()

        if pk1.get_nickname() != pk2.get_nickname():
            raise Exception("pk.get_nickname() mismatch")
        if pk1.get_trainer_name() != pk2.get_trainer_name():
            raise Exception("pk.get_trainer_name() mismatch")
        if entry1.species_name != entry2.species_name:
            raise Exception("entry.species_name mismatch")

        return True
    except:
        print
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

def gen2_conversions_test():
    try:
        pk1 = pkmn.pokemon("Pikachu", "Gold", 50, "Tackle", "Tail Whip", "Thunder", "Surf")
        exported = pkmn.conversions.export_gen2_pokemon(pk1)
        pk2 = pkmn.conversions.import_gen2_pokemon(exported)
        entry1 = pk1.get_pokedex_entry()
        entry2 = pk2.get_pokedex_entry()

        if pk1.get_nickname() != pk2.get_nickname():
            raise Exception("pk.get_nickname() mismatch")
        if pk1.get_trainer_name() != pk2.get_trainer_name():
            raise Exception("pk.get_trainer_name() mismatch")
        if entry1.species_name != entry2.species_name:
            raise Exception("entry.species_name mismatch")

        return True
    except:
        print
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

if __name__ == "__main__":

    successful = gen1_conversions_test()
    successful = gen2_conversions_test() and successful

    sys.exit(0 if successful else 1)
