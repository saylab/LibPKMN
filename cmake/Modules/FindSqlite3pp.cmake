# Find SQLite3pp

FIND_PATH(SQLITE3PP_INCLUDE_DIR sqlite3pp.h
  "$ENV{LIB_DIR}/include"
  "$ENV{LIB_DIR}/include/sqlite3pp"
  #mingw
  c:/msys/local/include
  NO_DEFAULT_PATH
  )
FIND_PATH(SQLITE3PP_INCLUDE_DIR sqlite3pp.h)

FIND_LIBRARY(SQLITE3PP_LIBRARY NAMES sqlite3pp sqlite3pp_i PATHS
  "$ENV{LIB_DIR}/lib"
  #mingw
  c:/msys/local/lib
  NO_DEFAULT_PATH
  )
FIND_LIBRARY(SQLITE3PP_LIBRARY NAMES sqlite3pp)

IF (SQLITE3PP_INCLUDE_DIR AND SQLITE3PP_LIBRARY)
   SET(SQLITE3PP_FOUND TRUE)
ENDIF (SQLITE3PP_INCLUDE_DIR AND SQLITE3PP_LIBRARY)

IF (SQLITE3PP_FOUND)

   IF (NOT SQLITE3PP_FIND_QUIETLY)
      MESSAGE(STATUS "Found SQLite3pp: ${SQLITE3PP_LIBRARY}")
   ENDIF (NOT SQLITE3PP_FIND_QUIETLY)

ELSE (SQLITE3PP_FOUND)

  MESSAGE(FATAL_ERROR "Could not find SQLite3pp")

ENDIF (SQLITE3PP_FOUND)
