#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

MESSAGE(STATUS "Building PKMDS as part of LibPKMN.")
MESSAGE(STATUS "")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(pkmds_sources
    pkmds_g5.cpp
    pkmds_g5_sqlite.cpp
    pkmds_sql.cpp
    stdafx.cpp
)
ADD_LIBRARY(pkmds STATIC ${pkmds_sources})
IF(UNIX)
    IF(CMAKE_COMPILER_IS_GNUCXX)
        SET_TARGET_PROPERTIES(pkmds PROPERTIES COMPILE_FLAGS "-fPIC -std=c++0x")
    ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        SET_TARGET_PROPERTIES(pkmds PROPERTIES COMPILE_FLAGS "-fPIC -std=c++11")
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)
ELSEIF(MINGW)
    SET_TARGET_PROPERTIES(pkmds PROPERTIES COMPILE_FLAGS "-std=c++11")
ENDIF(UNIX)
