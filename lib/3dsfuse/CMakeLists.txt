#
# Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(WIN32)
    FIND_PACKAGE(Dokan)
    LIBPKMN_REGISTER_COMPONENT("3DS Save Decryption" ENABLE_3DS_DECRYPTION ON "DOKAN_FOUND" OFF)

    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
    ADD_LIBRARY(winfuse SHARED fuse.c)
    TARGET_LINK_LIBRARIES(winfuse ${DOKAN_LIBRARY})

    SET(3DS_DECRYPTION_LIBRARIES
        winfuse
        ${DOKAN_LIBRARY}
    )
ELSE()
    FIND_PACKAGE(FUSE)
    LIBPKMN_REGISTER_COMPONENT("3DS Save Decryption" ENABLE_3DS_DECRYPTION ON "FUSE_FOUND" OFF)

    SET(3DS_DECRYPTION_LIBRARIES
        ${FUSE_LIBRARIES}
    )
ENDIF(WIN32)