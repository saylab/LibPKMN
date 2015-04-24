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

def gen1_pokemon_conversion_test():
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

        # Make sure get_native() method uses right export function
        if pk1.get_native() != exported:
            raise Exception("get_native() failed")

        return True
    except:
        print
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

def gen2_pokemon_conversion_test():
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

        # Make sure get_native() method uses right export function
        if pk1.get_native() != exported:
            raise Exception("get_native() failed")

        return True
    except:
        print
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

def gen3_pokemon_conversion_test():
    try:
        pk1 = pkmn.pokemon("Pikachu", "Emerald", 50, "Tackle", "Tail Whip", "Thunder", "Surf")
        exported = pkmn.conversions.export_gen3_pokemon(pk1)
        pk2 = pkmn.conversions.import_gen3_pokemon(exported, "Emerald")
        entry1 = pk1.get_pokedex_entry()
        entry2 = pk2.get_pokedex_entry()

        if pk1.get_nickname() != pk2.get_nickname():
            raise Exception("pk.get_nickname() mismatch")
        if pk1.get_trainer_name() != pk2.get_trainer_name():
            raise Exception("pk.get_trainer_name() mismatch")
        if entry1.species_name != entry2.species_name:
            raise Exception("entry.species_name mismatch")

        # Make sure get_native() method uses right export function
        if pk1.get_native() != exported:
            raise Exception("get_native() failed")

        return True
    except:
        print
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

def nds_pokemon_conversion_test():
    try:
        pk1 = pkmn.pokemon("Pikachu", "Platinum", 50, "Tackle", "Tail Whip", "Thunder", "Surf")
        exported = pkmn.conversions.export_nds_pokemon(pk1)
        pk2 = pkmn.conversions.import_nds_pokemon(exported, "Platinum")
        entry1 = pk1.get_pokedex_entry()
        entry2 = pk2.get_pokedex_entry()

        if pk1.get_nickname() != pk2.get_nickname():
            raise Exception("pk.get_nickname() mismatch")
        if pk1.get_trainer_name() != pk2.get_trainer_name():
            raise Exception("pk.get_trainer_name() mismatch")
        if entry1.species_name != entry2.species_name:
            raise Exception("entry.species_name mismatch")

        # Make sure get_native() method uses right export function
        if pk1.get_native() != exported:
            raise Exception("get_native() failed")

        return True
    except:
        print
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

if __name__ == "__main__":

    successful = gen1_pokemon_conversion_test()
    successful = gen2_pokemon_conversion_test() and successful
    successful = gen3_pokemon_conversion_test() and successful
    successful = nds_pokemon_conversion_test() and successful

    sys.exit(0 if successful else 1)
