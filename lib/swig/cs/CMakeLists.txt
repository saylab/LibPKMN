#
# Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(${CSHARP_USE_FILE})
INCLUDE(${SWIG_USE_FILE})

FILE(TO_NATIVE_PATH "${PKMNSIM_SOURCE_DIR}/lib/swig" PKMNSIM_SWIG_SOURCE_DIR)
SET(PKMNSIM_SWIG_SOURCE_DIR ${PKMNSIM_SWIG_SOURCE_DIR} CACHE FILEPATH "Pokemon-sim SWIG source directory")

SET(PKMNSIM_CSHARP_INCLUDE_DIRS
    ${PKMNLIBEX_SOURCE_DIR}/Pokemon-sim/include
    ${PKMNSIM_SWIG_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
)

SET(PKMNSIM_CSHARP_LIBRARIES
    pkmnsim
)

INCLUDE_DIRECTORIES(${PKMNSIM_CSHARP_INCLUDE_DIRS})
SET(CSHARP_SOURCE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/CSharpSources CACHE INTERNAL "")
SET(CMAKE_SWIG_OUTDIR ${CSHARP_SOURCE_DIRECTORY})
SET(CSHARP_BINARY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/CSharpBinaries CACHE INTERNAL "")
FILE(MAKE_DIRECTORY ${CSHARP_SOURCE_DIRECTORY})
FILE(MAKE_DIRECTORY ${CSHARP_BINARY_DIRECTORY})

INCLUDE(UseSWIG)
macro(CSHARP_BUILD_MODULE swig_module_name dll_name cs_module_name)
    FOREACH(CMAKE_CONFIGURATION_TYPE ${CMAKE_CONFIGURATION_TYPES})
        STRING(TOUPPER ${CMAKE_CONFIGURATION_TYPE} CMAKE_CONFIGURATION_TYPE)
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_module_name}_TARGET_NAME}
            PROPERTIES LIBRARY_OUTPUT_DIRECTORY_${CMAKE_CONFIGURATION_TYPE} "${CSHARP_BINARY_DIRECTORY}")
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_module_name}_TARGET_NAME}
            PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${CMAKE_CONFIGURATION_TYPE} "${CSHARP_BINARY_DIRECTORY}")
    ENDFOREACH()

    SET_SOURCE_FILES_PROPERTIES(${swig_module_name}.i PROPERTIES CPLUSPLUS ON)
    IF(UNIX)
        SET(CMAKE_SWIG_FLAGS -module ${cs_module_name} -namespace \"PKMNsimSharp\" -dllimport \"lib${dll_name}\" ${CMAKE_SWIG_GLOBAL_FLAGS} ${CMAKE_GLOBAL_FLAGS})
    ELSEIF(WIN32)
        SET(CMAKE_SWIG_FLAGS -module ${cs_module_name} -namespace \"PKMNsimSharp\" -dllimport \"${dll_name}\" ${CMAKE_SWIG_GLOBAL_FLAGS} ${CMAKE_GLOBAL_FLAGS})
    ENDIF(UNIX)

    SWIG_ADD_MODULE(${swig_module_name} csharp ${swig_module_name}.i)
    IF(UNIX)
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_module_name}_REAL_NAME} PROPERTIES PREFIX "lib")
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_module_name}_REAL_NAME} PROPERTIES SUFFIX ".so")
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_module_name}_REAL_NAME} PROPERTIES COMPILE_FLAGS "-std=c++0x")
    ELSEIF(WIN32)
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_module_name}_REAL_NAME} PROPERTIES PREFIX "")
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_module_name}_REAL_NAME} PROPERTIES SUFFIX ".dll")
    ENDIF(UNIX)
    SWIG_LINK_LIBRARIES(${swig_module_name} ${PKMNSIM_CSHARP_LIBRARIES})

    IF(WIN32)
        INSTALL(
            TARGETS ${SWIG_MODULE_${swig_module_name}_REAL_NAME}
            DESTINATION ${RUNTIME_DIR}
            COMPONENT "PKMNsim#"
        )
    ELSE(WIN32)
        INSTALL(
            TARGETS ${SWIG_MODULE_${swig_module_name}_REAL_NAME}
            DESTINATION ${LIBRARY_DIR}
            COMPONENT "PKMNsim#"
        )
    ENDIF(WIN32)
endmacro(CSHARP_BUILD_MODULE)

####################################################################
# Build SWIG modules for different parts of LibPKMNsim
####################################################################
CSHARP_BUILD_MODULE(pkmnsimsharp_swig pkmnsimsharp_swig PKMNsim)
CSHARP_BUILD_MODULE(database_cs_swig database_cs_swig Database)
CSHARP_BUILD_MODULE(io_cs_swig io_cs_swig IO)
CSHARP_BUILD_MODULE(analysis_cs_swig analysis_cs_swig Analysis)

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/AssemblyInfo.cs.in
    ${CMAKE_CURRENT_BINARY_DIR}/CSharpSources/AssemblyInfo.cs
COPYONLY)
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/Enums.cs.in
    ${CMAKE_CURRENT_BINARY_DIR}/CSharpSources/Enums.cs
COPYONLY)

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pkmnsim-sharp.dll
    COMMAND ${CSHARP_COMPILER} ARGS "/t:library" "/platform:x86" "/out:${CMAKE_CURRENT_BINARY_DIR}/pkmnsim-sharp.dll" "*.cs"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/CSharpSources
    DEPENDS ${SWIG_MODULE_pkmnsimsharp_swig_REAL_NAME}
    COMMENT "Building pkmnsim-sharp.dll"
)

ADD_CUSTOM_TARGET(
    pkmnsim-sharp ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/pkmnsim-sharp.dll
)


INSTALL(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/pkmnsim-sharp.dll
    DESTINATION ${RUNTIME_DIR}
    COMPONENT "PKMNsim#"
)