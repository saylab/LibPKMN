#!/usr/bin/env python
#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import python as pkmn

#
# Compare the reported pointer values from the SWIG proxies.
#
def compare_ptrs(obj1, obj2):
    obj1_val = obj1.__repr__().split(">")[-3].split(" ")[-1]
    obj2_val = obj2.__repr__().split(">")[-3].split(" ")[-1]
    return obj1_val == obj2_val

#
# Make sure the bag shared_ptr copies correctly.
#
def bag_pocket_test():
    bag1 = pkmn.bag("Gold")
    bag2 = bag1
    assert compare_ptrs(bag1, bag2)

    #
    # SWIG+Python reports pockets from bags from the same pointer having
    # different values, so check equality through item additions.
    #
    bag1_pockets = bag1.get_pockets()
    bag2_pockets = bag2.get_pockets()

    bag1_pockets["Items"].add_item("Potion", 10)
    bag1_pockets["Balls"].add_item("Master Ball", 10)
    bag1_pockets["KeyItems"].add_item("Bicycle", 1)
    bag1_pockets["TM/HM"].add_item("TM01", 10)
    assert bag2_pockets["Items"].get_item_amount("Potion") == 10
    assert bag2_pockets["Balls"].get_item_amount("Master Ball") == 10
    assert bag2_pockets["KeyItems"].get_item_amount("Bicycle") == 1
    assert bag2_pockets["TM/HM"].get_item_amount("TM01") == 10

#
# Make sure the base_pokemon shared_ptr copies correctly.
#
def base_pokemon_test():
    base_pokemon1 = pkmn.base_pokemon("Bulbasaur", "Ruby")
    base_pokemon2 = base_pokemon1
    assert compare_ptrs(base_pokemon1, base_pokemon2)

#
# Make sure the item shared_ptr copies correctly.
#
def item_test():
    item1 = pkmn.item("Potion", "Diamond")
    item2 = item1
    assert compare_ptrs(item1, item2)

#
# Make sure the move shared_ptr copies correctly.
#
def move_test():
    move1 = pkmn.move("Tackle", "Diamond")
    move2 = move1
    assert compare_ptrs(move1, move2)

#
# Make sure the prng shared_ptr copies correctly.
#
def prng_test():
    prng1 = pkmn.prng(3)
    prng2 = prng1
    assert compare_ptrs(prng1, prng2)

#
# Make sure the team_pokemon shared_ptr copies correctly.
# Make sure team_pokemon_modernimpl's signals/slots work correctly.
#
def team_pokemon_test():
    deoxys = pkmn.team_pokemon("Deoxys", "Diamond", 50, "None", "None", "None", "None")
    deoxys2 = deoxys
    assert compare_ptrs(deoxys, deoxys2)

    deoxys_stats1 = deoxys.get_stats()

    deoxys_base1 = deoxys.get_base_pokemon(False); # Same as deoxys
    deoxys_base2 = deoxys.get_base_pokemon(True)   # Copies from deoxys

    deoxys_base1.set_form("Attack")
    deoxys_base2.set_form("Defense")

    deoxys_stats2 = deoxys.get_stats()

    assert (not compare_ptrs(deoxys_base1, deoxys_base2))
    assert deoxys.get_form_id() == pkmn.Forms.Deoxys.ATTACK
    assert deoxys_base1.get_form_id() == pkmn.Forms.Deoxys.ATTACK
    assert deoxys.get_form_id() == deoxys_base1.get_form_id()
    assert deoxys_base2.get_form_id() == pkmn.Forms.Deoxys.DEFENSE
    assert deoxys_stats1["Attack"] != deoxys_stats2["Attack"]
    assert deoxys_stats1["Defense"] != deoxys_stats2["Defense"]
    assert deoxys_stats1["Special Attack"] != deoxys_stats2["Special Attack"]
    assert deoxys_stats1["Special Defense"] != deoxys_stats2["Special Defense"]

#
# Make sure the trainer shared_ptr copies correctly.
#
def trainer_test():
    trainer1 = pkmn.trainer("Red", "Red", "Male")
    trainer2 = trainer1
    assert compare_ptrs(trainer1, trainer2)

if __name__ == "__main__":
    bag_pocket_test()
    base_pokemon_test()
    item_test()
    move_test()
    prng_test()
    team_pokemon_test()
    trainer_test()