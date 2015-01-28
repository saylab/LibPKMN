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

#
# pkmn::dict<pkmn::pkstring, int>
#
def string_int_dict_test():
    try:
        pkmn_dict = pkmn.string_int_dict()
        pkmn_dict["abc"] = -1
        pkmn_dict["def"] = 5
        pkmn_dict["ghi"] = 57

        # Test adding and swapping
        if pkmn_dict["abc"] != -1:
            raise Exception("pkmn_dict[\"abc\"] != -1")
        if pkmn_dict["def"] != 5:
            raise Exception("pkmn_dict[\"def\"] != 5")
        if pkmn_dict["ghi"] != 57:
            raise Exception("pkmn_dict[\"abc\"] != 57")
        if pkmn_dict.at("abc", 5) != -1:
            raise Exception("pkmn_dict.at(\"abc\",5) != -1")
        if pkmn_dict.at("ade", -10) != -10:
            raise Exception("pkmn_dict.at(\"ade\",-10) != -10")
        pkmn_dict.swap("abc", "ghi")
        if pkmn_dict["abc"] != 57:
            raise Exception("pkmn_dict[\"abc\"] != 57")
        if pkmn_dict["ghi"] != -1:
            raise Exception("pkmn_dict[\"ghi\"] != -1")

        # Make sure key vector is in correct order
        keys = pkmn_dict.keys()
        if len(keys) != 3:
            raise Exception("len(keys) != 3")
        if keys[0] != "abc":
            raise Exception("keys[0] != \"abc\"")
        if keys[1] != "def":
            raise Exception("keys[1] != \"def\"")
        if keys[2] != "ghi":
            raise Exception("keys[2] != \"ghi\"")

        # Make sure val vector is in correct order
        vals = pkmn_dict.vals()
        if len(vals) != 3:
            raise Exception("len(vals) != 3")
        if vals[0] != 57:
            raise Exception("vals[0] != 57")
        if vals[1] != 5:
            raise Exception("vals[1] != 5")
        if vals[2] != -1:
            raise Exception("vals[2] != -1")

        # Make sure clearing works
        pkmn_dict.clear()
        if pkmn_dict.size() != 0:
            raise Exception("pkmn_dict.size() != 0")
        return True
    except:
        print
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

#
# pkmn::dict<pkmn::pkstring, pkmn::pkstring>
#
def string_string_dict_test():
    try:
        pkmn_dict = pkmn.string_string_dict()
        pkmn_dict["abc"] = "rst"
        pkmn_dict["def"] = "uvw"
        pkmn_dict["ghi"] = "xyz"

        # Test adding and swapping
        if pkmn_dict["abc"] != "rst":
            raise Exception("pkmn_dict[\"abc\"] != \"rst\"")
        if pkmn_dict["def"] != "uvw":
            raise Exception("pkmn_dict[\"def\"] != \"uvw\"")
        if pkmn_dict["ghi"] != "xyz":
            raise Exception("pkmn_dict[\"abc\"] != \"xyz\"")
        if pkmn_dict.at("abc", "uvw") != "rst":
            raise Exception("pkmn_dict.at(\"abc\", \"uvw\") != \"rst\"")
        if pkmn_dict.at("ade", "rst") != "rst":
            raise Exception("pkmn_dict.at(\"ade\", \"rst\") != \"rst\"")
        pkmn_dict.swap("abc", "ghi")
        if pkmn_dict["abc"] != "xyz":
            raise Exception("pkmn_dict[\"abc\"] != \"xyz\"")
        if pkmn_dict["ghi"] != "rst":
            raise Exception("pkmn_dict[\"ghi\"] != \"rst\"")

        # Make sure key vector is in correct order
        keys = pkmn_dict.keys()
        if len(keys) != 3:
            raise Exception("len(keys) != 3")
        if keys[0] != "abc":
            raise Exception("keys[0] != \"abc\"")
        if keys[1] != "def":
            raise Exception("keys[1] != \"def\"")
        if keys[2] != "ghi":
            raise Exception("keys[2] != \"ghi\"")

        # Make sure val vector is in correct order
        vals = pkmn_dict.vals()
        if len(vals) != 3:
            raise Exception("len(vals) != 3")
        if vals[0] != "xyz":
            raise Exception("vals[0] != \"xyz\"")
        if vals[1] != "def":
            raise Exception("vals[1] != \"def\"")
        if vals[2] != "abc":
            raise Exception("vals[2] != \"abc\"")

        # Make sure clearing works
        pkmn_dict.clear()
        if pkmn_dict.size() != 0:
            raise Exception("pkmn_dict.size() != 0")
        return True
    except:
        print
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

#
# pkmn::dict<pkmn::pkstring, uint16_t>
#
def string_ushort_dict_test():
    try:
        pkmn_dict = pkmn.string_int_dict()
        pkmn_dict["abc"] = 25
        pkmn_dict["def"] = 5
        pkmn_dict["ghi"] = 57

        # Test adding and swapping
        if pkmn_dict["abc"] != 25:
            raise Exception("pkmn_dict[\"abc\"] != 25")
        if pkmn_dict["def"] != 5:
            raise Exception("pkmn_dict[\"def\"] != 5")
        if pkmn_dict["ghi"] != 57:
            raise Exception("pkmn_dict[\"abc\"] != 57")
        if pkmn_dict.at("abc", 5) != 25:
            raise Exception("pkmn_dict.at(\"abc\",5) != 25")
        if pkmn_dict.at("ade", 250) != 250:
            raise Exception("pkmn_dict.at(\"ade\",250) != 250")
        pkmn_dict.swap("abc", "ghi")
        if pkmn_dict["abc"] != 57:
            raise Exception("pkmn_dict[\"abc\"] != 57")
        if pkmn_dict["ghi"] != 25:
            raise Exception("pkmn_dict[\"ghi\"] != 25")

        # Make sure key vector is in correct order
        keys = pkmn_dict.keys()
        if len(keys) != 3:
            raise Exception("len(keys) != 3")
        if keys[0] != "abc":
            raise Exception("keys[0] != \"abc\"")
        if keys[1] != "def":
            raise Exception("keys[1] != \"def\"")
        if keys[2] != "ghi":
            raise Exception("keys[2] != \"ghi\"")

        # Make sure val vector is in correct order
        vals = pkmn_dict.vals()
        if len(vals) != 3:
            raise Exception("len(vals) != 3")
        if vals[0] != 57:
            raise Exception("vals[0] != 57")
        if vals[1] != 5:
            raise Exception("vals[1] != 5")
        if vals[2] != 25:
            raise Exception("vals[2] != 25")

        # Make sure clearing works
        pkmn_dict.clear()
        if pkmn_dict.size() != 0:
            raise Exception("pkmn_dict.size() != 0")
        return True
    except:
        print
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

if __name__ == "__main__":

    successful = string_int_dict_test()
    successful = string_string_dict_test() and successful
    successful = string_ushort_dict_test() and successful

    sys.exit(0 if successful else 1)
