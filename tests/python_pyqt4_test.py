#!/usr/bin/env python
#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import python as LibPKMN

import sys

try:
    from PyQt4 import QtCore,QtGui
except: # Assume qt4_swig was never built
    sys.exit(0)

if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)

    # Test AbilityListComboBox
    abilities = LibPKMN.pkstring_vec()
    LibPKMN.get_ability_list(abilities, 6)
    abilityBox = LibPKMN.qt4.AbilityListComboBox(6)
    assert(len(abilities) == abilityBox.count())

    # Test ItemListComboBox
    items = LibPKMN.pkstring_vec()
    LibPKMN.get_item_list(items, LibPKMN.Versions.Y)
    itemBox = LibPKMN.qt4.ItemListComboBox(LibPKMN.Versions.Y)
    assert(len(items) == itemBox.count())

    # Test NatureListComboBox
    natures = LibPKMN.pkstring_vec()
    LibPKMN.get_nature_list(natures)
    natureBox = LibPKMN.qt4.NatureListComboBox()
    assert(len(natures) == natureBox.count())

    # Test PokemonListComboBox
    pokemons = LibPKMN.pkstring_vec()
    LibPKMN.get_pokemon_list(pokemons, LibPKMN.Versions.Y)
    pokemonBox = LibPKMN.qt4.PokemonListComboBox(LibPKMN.Versions.Y)
    assert(len(pokemons) == pokemonBox.count())

    # Test TypeListComboBox
    types = LibPKMN.pkstring_vec()
    LibPKMN.get_type_list(types, LibPKMN.Versions.Y)
    typeBox = LibPKMN.qt4.TypeListComboBox(LibPKMN.Versions.Y)
    assert(len(types) == typeBox.count())

    # Test VersionListComboBox
    versions = LibPKMN.pkstring_vec()
    LibPKMN.get_version_list(versions)
    versionBox = LibPKMN.qt4.VersionListComboBox()
    assert(len(versions) == versionBox.count())
