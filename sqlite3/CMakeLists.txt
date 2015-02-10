#
# Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

MESSAGE(STATUS "")
INCLUDE(FindThreads)

ADD_LIBRARY(sqlite3 STATIC sqlite3.c)
TARGET_LINK_LIBRARIES(sqlite3 ${CMAKE_THREAD_LIBS_INIT})
IF(UNIX)
    TARGET_LINK_LIBRARIES(sqlite3 dl)
ENDIF(UNIX)

IF(NOT CMAKE_CROSSCOMPILING)
    ADD_EXECUTABLE(sqlite3-shell shell.c)
    TARGET_LINK_LIBRARIES(sqlite3-shell sqlite3)
    EXPORT(TARGETS sqlite3-shell APPEND FILE ${LIBPKMN_BINARY_DIR}/ImportExecutables.cmake)
ELSEIF(IMPORT_EXECUTABLES)
    INCLUDE(${IMPORT_EXECUTABLES})
ENDIF(NOT CMAKE_CROSSCOMPILING)

FILE(TO_NATIVE_PATH "${CMAKE_INSTALL_PREFIX}/${PKG_DATA_DIR}/libpkmn.db" DATABASE_PATH)
SET(LIBPKMN_DATABASE_PATH ${DATABASE_PATH} CACHE FILEPATH "Database path")

####################################################################
# Database dump is distributed instead of a binary, so compile and
# install in correct location
####################################################################
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/database_concat.py.in
    ${CMAKE_CURRENT_BINARY_DIR}/database_concat.py
@ONLY)
ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libpkmn.db
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/libpkmn_db_dump
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/libpkmn_db_additions
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/libpkmn.db
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/database_concat.py
    COMMAND sqlite3-shell ${CMAKE_CURRENT_BINARY_DIR}/libpkmn.db ".read ${CMAKE_CURRENT_BINARY_DIR}/libpkmn_db_dump"
    COMMENT "Compiling LibPKMN SQLite database"
)
ADD_CUSTOM_TARGET(
    libpkmn.db ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/libpkmn.db
)

INSTALL(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/libpkmn.db
    DESTINATION ${PKG_DATA_DIR}
    COMPONENT "Database"
)
