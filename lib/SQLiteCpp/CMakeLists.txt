#
# Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

MESSAGE(STATUS "Building SQLiteCpp as part of LibPKMN.")
MESSAGE(STATUS "")

#SQLite needs to be separate because of Clang's compiler flag restrictions
ADD_LIBRARY(sqlite3 STATIC sqlite3.c)
IF(UNIX)
    SET_TARGET_PROPERTIES(sqlite3 PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF(UNIX)

SET(SQLiteCpp_sources
    Column.cpp
    Database.cpp
    Statement.cpp
    Transaction.cpp
)
ADD_LIBRARY(SQLiteCpp STATIC ${SQLiteCpp_sources})
TARGET_LINK_LIBRARIES(SQLiteCpp sqlite3)
IF(UNIX)
    IF(CMAKE_COMPILER_IS_GNUCXX)
        SET_TARGET_PROPERTIES(SQLiteCpp PROPERTIES COMPILE_FLAGS "-fPIC -std=c++0x")
    ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        SET_TARGET_PROPERTIES(SQLiteCpp PROPERTIES COMPILE_FLAGS "-fPIC -std=c++11")
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)
ELSEIF(MINGW)
    SET_TARGET_PROPERTIES(SQLiteCpp PROPERTIES COMPILE_FLAGS "-std=c++11")
ENDIF(UNIX)
