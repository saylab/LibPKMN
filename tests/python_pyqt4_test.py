#!/usr/bin/env python
#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import python as pkmn

import sys

try:
    from PyQt4 import QtCore,QtGui
except: # Assume qt4_swig was never built
    sys.exit(0)

if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)

    # Test AbilityListComboBox
    abilities = pkmn.pkstring_vec()
    pkmn.get_ability_list(abilities, 6)
    abilityBox = pkmn.qt4.AbilityListComboBox(6)
    assert(len(abilities) == abilityBox.count())

    # Test ItemListComboBox
    items = pkmn.pkstring_vec()
    pkmn.get_item_list(items, pkmn.Versions.Y)
    itemBox = pkmn.qt4.ItemListComboBox(pkmn.Versions.Y)
    assert(len(items) == itemBox.count())

    # Test NatureListComboBox
    natures = pkmn.pkstring_vec()
    pkmn.get_nature_list(natures)
    natureBox = pkmn.qt4.NatureListComboBox()
    assert(len(natures) == natureBox.count())

    # Test PokemonListComboBox
    pokemons = pkmn.pkstring_vec()
    pkmn.get_pokemon_list(pokemons, pkmn.Versions.Y)
    pokemonBox = pkmn.qt4.PokemonListComboBox(pkmn.Versions.Y)
    assert(len(pokemons) == pokemonBox.count())

    # Test TypeListComboBox
    types = pkmn.pkstring_vec()
    pkmn.get_type_list(types, pkmn.Versions.Y)
    typeBox = pkmn.qt4.TypeListComboBox(pkmn.Versions.Y)
    assert(len(types) == typeBox.count())

    # Test VersionListComboBox
    versions = pkmn.pkstring_vec()
    pkmn.get_version_list(versions)
    versionBox = pkmn.qt4.VersionListComboBox()
    assert(len(versions) == versionBox.count())
