#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/AboutMessageBox.cpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/AboutMessageBox.cpp
@ONLY)

SET(qt4_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/AbilityListComboBox.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/AboutMessageBox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ItemListComboBox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/NatureListComboBox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PokemonListComboBox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TypeListComboBox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/VersionListComboBox.cpp
    ${moc_sources}
PARENT_SCOPE)
