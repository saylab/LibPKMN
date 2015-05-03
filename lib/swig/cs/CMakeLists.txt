#
# Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(${CSHARP_USE_FILE})
INCLUDE(${SWIG_USE_FILE})

FILE(TO_NATIVE_PATH "${LIBPKMN_SOURCE_DIR}/lib/swig" LIBPKMN_SWIG_SOURCE_DIR)
SET(LIBPKMN_SWIG_SOURCE_DIR ${LIBPKMN_SWIG_SOURCE_DIR} CACHE FILEPATH "LibPKMN SWIG source directory")

SET(LIBPKMN_CSHARP_INCLUDE_DIRS
    ${LIBPKMN_SOURCE_DIR}/include
    ${LIBPKMN_BINARY_DIR}/lib/swig
    ${LIBPKMN_SWIG_SOURCE_DIR}
    ${LIBPKMN_SWIG_SOURCE_DIR}/cs
    ${LIBPKMN_SWIG_SOURCE_DIR}/cs/types
    ${LIBPKMN_SWIG_SOURCE_DIR}/types
    ${Boost_INCLUDE_DIRS}
)

SET(LIBPKMN_CSHARP_LIBRARIES
    pkmn
)

INCLUDE_DIRECTORIES(${LIBPKMN_CSHARP_INCLUDE_DIRS})
SET(CSHARP_SOURCE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/CSharpSources CACHE INTERNAL "")
SET(CMAKE_SWIG_OUTDIR ${CSHARP_SOURCE_DIRECTORY})
FILE(MAKE_DIRECTORY ${CSHARP_SOURCE_DIRECTORY})

SET(CSHARP_ENUM_FILES
    ${CSHARP_SOURCE_DIRECTORY}/Abilities.cs
    ${CSHARP_SOURCE_DIRECTORY}/EggGroups.cs
    ${CSHARP_SOURCE_DIRECTORY}/Genders.cs
    ${CSHARP_SOURCE_DIRECTORY}/Items.cs
    ${CSHARP_SOURCE_DIRECTORY}/MoveClasses.cs
    ${CSHARP_SOURCE_DIRECTORY}/Moves.cs
    ${CSHARP_SOURCE_DIRECTORY}/Natures.cs
    ${CSHARP_SOURCE_DIRECTORY}/PokeBalls.cs
    ${CSHARP_SOURCE_DIRECTORY}/PokemonForms.cs
    ${CSHARP_SOURCE_DIRECTORY}/Species.cs
    ${CSHARP_SOURCE_DIRECTORY}/Stats.cs
    ${CSHARP_SOURCE_DIRECTORY}/Types.cs
    ${CSHARP_SOURCE_DIRECTORY}/VersionGroups.cs
    ${CSHARP_SOURCE_DIRECTORY}/Versions.cs
)

#Generate enums
ADD_CUSTOM_COMMAND(
    OUTPUT ${CSHARP_ENUM_FILES}
    COMMAND ${PYTHON_EXECUTABLE} ${LIBPKMN_SOURCE_DIR}/tools/generate_enums.py --language=cs --include-path=${LIBPKMN_SOURCE_DIR}/include/pkmn/enums.hpp --output-dir=${CSHARP_SOURCE_DIRECTORY}
    WORKING_DIRECTORY ${LIBPKMN_SOURCE_DIR}/tools
    COMMENT "Generating C# enums"
)
ADD_CUSTOM_TARGET(
    cs_enums ALL
    DEPENDS ${LIBPKMN_SOURCE_DIR}/include/pkmn/enums.hpp
    DEPENDS ${CSHARP_ENUM_FILES}
)

####################################################################
# Build SWIG modules for different parts of LibPKMN
####################################################################
INCLUDE(UseSWIG)
INCLUDE(LibPKMNCSharp)
CSHARP_BUILD_SWIG_MODULE(pkmn_cs pkmn_cs PKMN)
CSHARP_BUILD_SWIG_MODULE(buildinfo_cs buildinfo_cs BuildInfo)
CSHARP_BUILD_SWIG_MODULE(calculations_cs calculations_cs Calculations)
CSHARP_BUILD_SWIG_MODULE(database_cs database_cs Database)
CSHARP_BUILD_SWIG_MODULE(lists_cs lists_cs Lists)
CSHARP_BUILD_SWIG_MODULE(paths_cs paths_cs Paths)

SET(cs_sources
    AbilityListComboBox.cs
    AssemblyInfo.cs
    NatureListComboBox.cs
    PokemonListComboBox.cs
    TypeListComboBox.cs
    VersionListComboBox.cs
)
FOREACH(src ${cs_sources})
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/${src}
        ${CMAKE_CURRENT_BINARY_DIR}/CSharpSources/${src}
    COPYONLY)
ENDFOREACH(src ${cs_sources})

SET(CSHARP_PLATFORM "x86")
MAKE_DIRECTORY(${LIBPKMN_BINARY_DIR}/tests)
SET(CSHARP_BINARY_DIRECTORY ${LIBPKMN_BINARY_DIR}/tests) # Hacky solution
CSHARP_ADD_LIBRARY(
    pkmn-cs "System.Windows.Forms.dll" "${CMAKE_CURRENT_BINARY_DIR}/CSharpSources/*cs"
)
ADD_DEPENDENCIES(pkmn-cs
    ${SWIG_MODULE_pkmn_cs_REAL_NAME}
    ${SWIG_MODULE_buildinfo_cs_REAL_NAME}
    ${SWIG_MODULE_calculations_cs_REAL_NAME}
    ${SWIG_MODULE_database_cs_REAL_NAME}
    ${SWIG_MODULE_lists_cs_REAL_NAME}
    ${SWIG_MODULE_paths_cs_REAL_NAME}
)

INSTALL(
    FILES ${CSHARP_BINARY_DIRECTORY}/pkmn-cs.dll
    DESTINATION ${RUNTIME_DIR}
    COMPONENT "LibPKMN_CS"
)
