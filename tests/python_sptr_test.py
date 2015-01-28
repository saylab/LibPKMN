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

#
# Compare the reported pointer values from the SWIG proxies.
#
def compare_ptrs(obj1, obj2):
    obj1_val = obj1.__repr__().split(">")[-3].split(" ")[-1]
    obj2_val = obj2.__repr__().split(">")[-3].split(" ")[-1]
    return obj1_val == obj2_val

#  
# Make sure two instances of bag_sptr properly point to the same underlying
# bag::sptr. By adding items through each bag directly and through each of
# their pockets, we can query all of them to make sure the change can be
# seen with each method.
#
def bag_pocket_test():
    try:
        bag1 = pkmn.bag("Gold")
        bag2 = bag1
        if not compare_ptrs(bag1, bag2):
            raise Exception("not compare_ptrs(bag1, bag2)")

        bag1_pockets = bag1.get_pockets()
        bag2_pockets = bag2.get_pockets()

        # Add items with each possible method
        bag1.add_item("Potion", 5)
        bag2.add_item("Master Ball", 10)
        bag1_pockets["KeyItems"].add_item("Bicycle", 1)
        bag2_pockets["TM/HM"].add_item("TM01", 15)

        # Make sure bag1 reports proper amounts
        if bag1.get_item_amount("Potion") != 5:
            raise Exception("bag1.get_item_amount(\"Potion\") != 5")
        if bag1.get_item_amount("Master Ball") != 10:
            raise Exception("bag1.get_item_amount(\"Master Ball\") != 10")
        if bag1.get_item_amount("Bicycle") != 1:
            raise Exception("bag1.get_item_amount(\"Bicycle\") != 1")
        if bag1.get_item_amount("TM01") != 15:
            raise Exception("bag1.get_item_amount(\"TM01\") != 15")

        # Make sure bag2 reports proper amounts
        if bag2.get_item_amount("Potion") != 5:
            raise Exception("bag2.get_item_amount(\"Potion\") != 5")
        if bag2.get_item_amount("Master Ball") != 10:
            raise Exception("bag2.get_item_amount(\"Master Ball\") != 10")
        if bag2.get_item_amount("Bicycle") != 1:
            raise Exception("bag2.get_item_amount(\"Bicycle\") != 1")
        if bag2.get_item_amount("TM01") != 15:
            raise Exception("bag2.get_item_amount(\"TM01\") != 15")

        # Make sure bag1_pockets reports proper amounts
        if bag1_pockets["Items"].get_item_amount("Potion") != 5:
            raise Exception("bag1_pockets[\"Items\"].get_item_amount(\"Potion\") != 5")
        if bag1_pockets["Balls"].get_item_amount("Master Ball") != 10:
            raise Exception("bag1_pockets[\"Balls\"].get_item_amount(\"Master Ball\") != 10")
        if bag1_pockets["KeyItems"].get_item_amount("Bicycle") != 1:
            raise Exception("bag1_pockets[\"KeyItems\"].get_item_amount(\"Bicycle\") != 1")
        if bag1_pockets["TM/HM"].get_item_amount("TM01") != 15:
            raise Exception("bag1_pockets[\"TM/HM\"].get_item_amount(\"TM01\") != 15")

        # Make sure bag2_pockets reports proper amounts
        if bag2_pockets["Items"].get_item_amount("Potion") != 5:
            raise Exception("bag2_pockets[\"Items\"].get_item_amount(\"Potion\") != 5")
        if bag2_pockets["Balls"].get_item_amount("Master Ball") != 10:
            raise Exception("bag2_pockets[\"Balls\"].get_item_amount(\"Master Ball\") != 10")
        if bag2_pockets["KeyItems"].get_item_amount("Bicycle") != 1:
            raise Exception("bag2_pockets[\"KeyItems\"].get_item_amount(\"Bicycle\") != 1")
        if bag2_pockets["TM/HM"].get_item_amount("TM01") != 15:
            raise Exception("bag2_pockets[\"TM/HM\"].get_item_amount(\"TM01\") != 15")

        return True

    except:
        print
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

#
# Make sure the = operator correctly points the new pokedex_sptr
# to the same underlying pkmn::pokedex::sptr instance.
#
def pokedex_test():
    try:
        pokedex1 = pkmn.pokedex("Red")
        pokedex2 = pokedex1
        if not compare_ptrs(pokedex1, pokedex2):
            raise Exception("not compare_ptrs(pokedex1, pokedex2)")

        return True

    except:
        print
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

#
# Make sure the = operator correctly points the new pokemon_sptr
# to the same underlying pkmn::pokemon::sptr instance.
#
def pokemon_test():
    try:
        pokemon1 = pkmn.pokemon("Houndoom", "X", 50,
                                "None", "None",
                                "None", "None")
        pokemon2 = pokemon1
        if not compare_ptrs(pokemon1, pokemon2):
            raise Exception("not compare_ptrs(pokemon1, pokemon2)")

        # Set form with pokemon1
        pokemon1.set_form("Mega")
        if pokemon2.get_form() != "Mega":
            raise Exception("pokemon2.get_form() != \"Mega\"")

        # Set form with pokemon2
        pokemon2.set_form("Standard")
        if pokemon1.get_form() != "Standard":
            raise Exception("pokemon1.get_form() != \"Standard\"")

        return True

    except:
        print
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

#  
# Make sure the = operator correctly points the new PRNGSPtr
# to the same underlying pkmn::prng::sptr instance.
#
# We will seed each instance with the same value and check to
# see if the other produces the same output.
#
def prng_test():
    try:
        prng1 = pkmn.prng(3)
        prng2 = prng1
        if not compare_ptrs(prng1, prng2):
            raise Exception("not compare_ptrs(prng1, prng2)")

        # Set value with prng1
        prng1.seed_arng(123456)
        arng1_output1 = prng1.arng()
        prng1.seed_arng(123456)
        arng2_output1 = prng2.arng()
        if arng1_output1 != arng2_output1:
            raise Exception("arng1_output1 != arng2_output1")

        # Set value with prng2
        prng2.seed_arng(123456)
        arng1_output2 = prng2.arng()
        prng2.seed_arng(123456)
        arng2_output2 = prng1.arng()
        if arng1_output2 != arng2_output2:
            raise Exception("arng1_output2 != arng2_output2")

        return True

    except:
        print
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

#
# Make sure the = operator correctly points the new TrainerSPtr
# to the same underlying pkmn::trainer::sptr instance.
#
def trainer_test():
    try:
        trainer1 = pkmn.trainer("Red", "Red", "Male")
        trainer2 = trainer1
        if not compare_ptrs(trainer1, trainer2):
            raise Exception("not compare_ptrs(trainer1, trainer2)")

        # Set name with trainer1
        trainer1.set_name("Blue")
        if trainer2.get_name() != "Blue":
            raise Exception("trainer2.get_name() != \"Blue\"")

        # Set name with trainer2
        trainer2.set_name("Red")
        if trainer1.get_name() != "Red":
            raise Exception("trainer1.get_name() != \"Red\"")

    except:
        print
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

if __name__ == "__main__":
    successful = bag_pocket_test()
    successful = pokedex_test() and successful
    successful = pokemon_test() and successful
    successful = prng_test() and successful
    successful = trainer_test() and successful

    sys.exit(0 if successful else 1)
