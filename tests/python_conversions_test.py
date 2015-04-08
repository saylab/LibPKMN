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

    except:
        print
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

def gen1_text_conversion_test():
    try:
        gen1_input = "\x8B\xA8\x81\xAF\x8A\xAC\x8D\xF7\xF8\xF9\xFA\xFB\x50\x50\x50"
        gen1_pkstring = pkmn.conversions.import_gen1_text(gen1_input)
        if gen1_pkstring != "LiBpKmN12345":
            raise Exception("gen1_pkstring != \"LiBpKmN12345\"")

        return True
    except:
        print
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

def gen2_text_conversion_test():
    try:
        gen2_input = "\x8B\xA8\x81\xAF\x8A\xAC\x8D\xF7\xF8\xF9\xFA\xFB\x50\x50\x50"
        gen2_pkstring = pkmn.conversions.import_gen2_text(gen2_input)
        if gen2_pkstring != "LiBpKmN12345":
            raise Exception("gen2_pkstring != \"LiBpKmN12345\"")

        return True
    except:
        print
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

def gen3_text_conversion_test():
    try:
        gen3_input = "\xC6\xDD\xBC\xE4\xC5\xE1\xC8\xA2\xA3\xA4\xA5\xA6\xFF\xFF\xFF"
        gen3_pkstring = pkmn.conversions.import_gen3_text(gen3_input)
        if gen3_pkstring != "LiBpKmN12345":
            raise Exception("gen3_pkstring != \"LiBpKmN12345\"")

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
    successful = gen1_text_conversion_test() and successful
    successful = gen2_text_conversion_test() and successful
    successful = gen3_text_conversion_test() and successful

    sys.exit(0 if successful else 1)
