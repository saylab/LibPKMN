#
# Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
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
