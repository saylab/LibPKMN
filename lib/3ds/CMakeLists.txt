#
# Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

FIND_PACKAGE(OpenSSL)

IF(WIN32)
    FIND_PACKAGE(Dokan)
    LIBPKMN_REGISTER_COMPONENT("3DS Save Decryption" ENABLE_3DS_DECRYPTION ON "DOKAN_FOUND;OPENSSL_FOUND" OFF)

    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
    ADD_LIBRARY(winfuse SHARED fuse.c)
    TARGET_LINK_LIBRARIES(winfuse ${DOKAN_LIBRARY})
    INSTALL(TARGETS winfuse
        RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT Runtime #.dll
    )

    SET(3DS_DECRYPTION_LIBRARIES
        winfuse
        ${OPENSSL_LIBRARIES}
        ${DOKAN_LIBRARY}
    )
ELSE()
    FIND_PACKAGE(FUSE)
    LIBPKMN_REGISTER_COMPONENT("3DS Save Decryption" ENABLE_3DS_DECRYPTION ON "FUSE_FOUND;OPENSSL_FOUND" OFF)

    SET(3DS_DECRYPTION_LIBRARIES
        ${OPENSSL_LIBRARIES}
        ${FUSE_LIBRARIES}
    )
ENDIF(WIN32)

SET(3ds_sources
    crypto.c
    decrypt.c
    fs.c
    fuse_glue.c
    helper.c
    utils.c
    wearlevel.c
)
ADD_LIBRARY(3ds SHARED ${3ds_sources})
TARGET_LINK_LIBRARIES(3ds ${3DS_DECRYPTION_LIBRARIES})
INSTALL(TARGETS 3ds
    LIBRARY DESTINATION ${LIBRARY_DIR} COMPONENT Runtime #.so
    RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT Runtime #.dll
)