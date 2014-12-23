#!/usr/bin/env python
#
# Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import python as pkmn

if __name__ == "__main__":

    #############################################
    # pkmn::dict<pkmn::pkstring, int>
    #############################################
    dict1 = pkmn.string_int_dict()
    dict1["abc"] = -1
    dict1["def"] = 5
    dict1["ghi"] = 57
    assert(dict1["abc"] == -1)
    assert(dict1["def"] == 5)
    assert(dict1["ghi"] == 57)
    assert(dict1.at("abc",5) == -1)
    assert(dict1.at("ade",-10) == -10)
    dict1.swap("abc","ghi")
    assert(dict1["abc"] == 57)
    assert(dict1["ghi"] == -1)

    #############################################
    # pkmn::dict<pkmn::pkstring, pkmn::pkstring>
    #############################################
    dict2 = pkmn.string_string_dict()
    dict2["abc"] = "def"
    dict2["ghi"] = "jkl"
    dict2["mno"] = "pqr"
    assert(dict2["abc"] == "def")
    assert(dict2["ghi"] == "jkl")
    assert(dict2["mno"] == "pqr")
    assert(dict2.at("abc","stu") == "def")
    assert(dict2.at("stu","vwx") == "vwx")
    dict2.swap("abc","mno")
    assert(dict2["abc"] == "pqr")
    assert(dict2["mno"] == "def")

    #############################################
    # pkmn::dict<pkmn::pkstring, unsigned int>
    #############################################
    dict3 = pkmn.string_uint_dict()
    dict3["abc"] = 10
    dict3["def"] = 5
    dict3["ghi"] = 57
    assert(dict3["abc"] == 10)
    assert(dict3["def"] == 5)
    assert(dict3["ghi"] == 57)
    assert(dict3.at("abc",5) == 10)
    assert(dict3.at("ade",2) == 2)
    dict3.swap("abc","ghi")
    assert(dict3["abc"] == 57)
    assert(dict3["ghi"] == 10)
