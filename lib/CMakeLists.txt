#
# Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

#Place database location into relevant source files
FILE(TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX}/${PKG_DATA_DIR} LIBPKMN_PKG_DATA_PATH)

IF(ENABLE_LIBPKMN)
    MESSAGE(STATUS "")

    INCLUDE_DIRECTORIES(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/SQLiteCpp
        ${LIBPKMN_SOURCE_DIR}/sqlite3
    )

    ADD_SUBDIRECTORY(SQLiteCpp)
    ADD_SUBDIRECTORY(conversions)
    ADD_SUBDIRECTORY(io)
    ADD_SUBDIRECTORY(native)
    ADD_SUBDIRECTORY(pokedex)
    ADD_SUBDIRECTORY(qt4)
    ADD_SUBDIRECTORY(types)
    
    #Build-time info
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/build_info.cpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/build_info.cpp
    @ONLY)
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/paths.cpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/paths.cpp
    @ONLY)

    LIST(APPEND libpkmn_libs
        ${Boost_LIBRARIES}
        ${QT_LIBRARIES}
        ${QT_QTMAIN_LIBRARY}
        sqlite3
    )

    #
    # The MOC generation must be done in this directory or CMake
    # won't see it in this scope.
    #
    SET(qt4_headers
        ${LIBPKMN_SOURCE_DIR}/include/pkmn/qt4/AbilityListComboBox.hpp
        ${LIBPKMN_SOURCE_DIR}/include/pkmn/qt4/AboutMessageBox.hpp
        ${LIBPKMN_SOURCE_DIR}/include/pkmn/qt4/ItemListComboBox.hpp
        ${LIBPKMN_SOURCE_DIR}/include/pkmn/qt4/NatureListComboBox.hpp
        ${LIBPKMN_SOURCE_DIR}/include/pkmn/qt4/PokemonListComboBox.hpp
        ${LIBPKMN_SOURCE_DIR}/include/pkmn/qt4/TypeListComboBox.hpp
        ${LIBPKMN_SOURCE_DIR}/include/pkmn/qt4/VersionListComboBox.hpp
    )
    INCLUDE(${QT_USE_FILE})
    QT4_WRAP_CPP(qt4_moc_sources ${qt4_headers})

    LIST(APPEND libpkmn_sources
        ${CMAKE_CURRENT_BINARY_DIR}/build_info.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/paths.cpp
        calculations.cpp
        database.cpp
        pokemon_impl.cpp
        pokemon_gen1impl.cpp
        pokemon_gen2impl.cpp
        pokemon_gen3impl.cpp
        pokemon_ndsimpl.cpp
        pokemon_gen6impl.cpp
        pocket_impl.cpp
        bag_impl.cpp
        trainer_impl.cpp
        pokedex_impl.cpp
        game_save_impl.cpp
        game_save_gen1impl.cpp
        game_save_gen2impl.cpp
        game_save_gen3impl.cpp
        game_save_gen4impl.cpp
        lists.cpp
        copy_sptrs.cpp
        ${SQLiteCpp_sources}
        ${conversions_sources}
        ${io_sources}
        ${native_sources}
        ${pokedex_sources}
        ${qt4_sources}
        ${qt4_moc_sources}
        ${types_sources}
    )

    ####################################################################
    # Add DLL resource file when using MSVC
    ####################################################################
    IF(MSVC)
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/pkmn.rc.in
            ${CMAKE_CURRENT_BINARY_DIR}/pkmn.rc
        @ONLY)
        LIST(APPEND libpkmn_sources ${CMAKE_CURRENT_BINARY_DIR}/pkmn.rc)
    ENDIF(MSVC)

    ADD_LIBRARY(pkmn SHARED ${libpkmn_sources})
    SET_TARGET_PROPERTIES(pkmn PROPERTIES DEFINE_SYMBOL "PKMN_DLL_EXPORTS")
    IF(NOT MSVC)
        ADD_DEFINITIONS(-Dpkmn_EXPORTS)
        cotire(pkmn)
    ENDIF(NOT MSVC)
    TARGET_LINK_LIBRARIES(pkmn ${libpkmn_libs})

    INSTALL(TARGETS pkmn
        LIBRARY DESTINATION ${LIBRARY_DIR} COMPONENT Libraries #.so
        ARCHIVE DESTINATION ${LIBRARY_DIR} COMPONENT Libraries #.lib
        RUNTIME DESTINATION ${RUNTIME_DIR} COMPONENT Runtime #.dll
    )
    IF(NOT DISABLE_SWIG)
        ADD_SUBDIRECTORY(swig)
    ENDIF(NOT DISABLE_SWIG)

    IF(UNIX)
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/pkmn.pc.in
            ${CMAKE_CURRENT_BINARY_DIR}/pkmn.pc
        @ONLY)

        INSTALL(
            FILES ${CMAKE_CURRENT_BINARY_DIR}/pkmn.pc
            DESTINATION ${LIBRARY_DIR}/pkgconfig
            COMPONENT libraries
        )
    ENDIF(UNIX)
ENDIF(ENABLE_LIBPKMN)
