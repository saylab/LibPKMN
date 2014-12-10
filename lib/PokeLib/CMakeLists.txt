#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

MESSAGE(STATUS "Building PokeLib as part of LibPKMN.")
MESSAGE(STATUS "")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(pokelib_sources
    Box.cpp
    Party.cpp
    Pokedex.cpp
    Pokemon.cpp
    PokeText.cpp
    PRNG.cpp
    Save.cpp
    Toolkit.cpp
    Trainer.cpp
)
ADD_LIBRARY(pokelib STATIC ${pokelib_sources})
IF(UNIX)
    IF(CMAKE_COMPILER_IS_GNUCXX)
        SET_TARGET_PROPERTIES(pokelib PROPERTIES COMPILE_FLAGS "-fPIC -std=c++0x")
    ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        SET_TARGET_PROPERTIES(pokelib PROPERTIES COMPILE_FLAGS "-fPIC -std=c++11")
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)
ELSEIF(MINGW)
    SET_TARGET_PROPERTIES(pokelib PROPERTIES COMPILE_FLAGS "-std=c++11")
ENDIF(UNIX)
