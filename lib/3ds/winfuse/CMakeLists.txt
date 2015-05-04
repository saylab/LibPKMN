#
# Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(OpenSSL)

IF(WIN32)
    ADD_LIBRARY(winfuse SHARED fuse.c)
    TARGET_LINK_LIBRARIES(winfuse ${DOKAN_LIBRARY})
    INSTALL(TARGETS winfuse
        RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT Runtime #.dll
    )
ENDIF(WIN32)
