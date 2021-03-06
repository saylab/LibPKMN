#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

#Extract Git commits from LibPKMN and all submodules

FIND_PACKAGE(Git)

IF(GIT_FOUND AND EXISTS ${LIBPKMN_SOURCE_DIR}/.git)
    MESSAGE(STATUS "")
    MESSAGE(STATUS "Extracting compilation info from Git and build dependencies...")
    EXECUTE_PROCESS(
        COMMAND ${GIT_EXECUTABLE} describe --always --abbrev=8 --long
        OUTPUT_VARIABLE LIBPKMN_GIT_DESCRIBE OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${LIBPKMN_SOURCE_DIR}
    )
    EXECUTE_PROCESS(
        COMMAND ${GIT_EXECUTABLE} describe --always --abbrev=8 --long
        OUTPUT_VARIABLE LIBPKMN_IMAGES_GIT_DESCRIBE OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${LIBPKMN_SOURCE_DIR}/share/libpkmn/images
    )
    EXECUTE_PROCESS(
        COMMAND ${GIT_EXECUTABLE} describe --always --abbrev=8 --long
        OUTPUT_VARIABLE PKMDS_GIT_DESCRIBE OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${LIBPKMN_SOURCE_DIR}/PKMDS
    )
ELSE()
    SET(LIBPKMN_GIT_DESCRIBE "unknown commit")
    SET(LIBPKMN_IMAGES_GIT_DESCRIBE "unknown commit")
    SET(PKMDS_GIT_DESCRIBE "unknown commit")
ENDIF(GIT_FOUND AND EXISTS ${LIBPKMN_SOURCE_DIR}/.git)

#Make values persistent so this only needs to be run once
SET(LIBPKMN_GIT_DESCRIBE "${LIBPKMN_GIT_DESCRIBE}" CACHE STRING "LibPKMN git describe output")
SET(LIBPKMN_IMAGES_GIT_DESCRIBE "${LIBPKMN_IMAGES_GIT_DESCRIBE}" CACHE STRING "LibPKMN Images git describe output")
SET(PKMDS_GIT_DESCRIBE "${PKMDS_GIT_DESCRIBE}" CACHE STRING "PKMDS git describe output")
