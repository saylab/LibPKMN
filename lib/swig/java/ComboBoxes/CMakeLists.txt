#
# Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(gui_files
    ${CMAKE_CURRENT_SOURCE_DIR}/AbilityListComboBox.java
    ${CMAKE_CURRENT_SOURCE_DIR}/ItemListComboBox.java
    ${CMAKE_CURRENT_SOURCE_DIR}/NatureListComboBox.java
    ${CMAKE_CURRENT_SOURCE_DIR}/PokemonListComboBox.java
    ${CMAKE_CURRENT_SOURCE_DIR}/TypeListComboBox.java
    ${CMAKE_CURRENT_SOURCE_DIR}/VersionListComboBox.java
)
FOREACH(gui_file ${gui_files})
    GET_FILENAME_COMPONENT(filename ${gui_file} NAME)

    CONFIGURE_FILE(
        ${gui_file}
        ${CMAKE_CURRENT_BINARY_DIR}/../nc/PKMN/${filename}
    @ONLY)
ENDFOREACH(gui_file ${gui_files})
