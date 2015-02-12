#
# Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(${LIBPKMN_JAVA_INCLUDE_DIRS})

########################################################################
# Build and install Java SWIG modules
# Assumes LIBPKMN_SWIG_SOURCE_DIR is set
########################################################################
MACRO(JAVA_BUILD_SWIG_MODULE swig_source java_module_name)
    FOREACH(dir ${LIBPKMN_JAVA_INCLUDE_DIRS})
        LIST(APPEND CMAKE_SWIG_FLAGS "-I${dir}")
    ENDFOREACH(dir ${LIBPKMN_JAVA_INCLUDE_DIRS})

    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/${swig_source}.i
        ${CMAKE_CURRENT_BINARY_DIR}/${swig_source}.i
    @ONLY)
    SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_BINARY_DIR}/${swig_source}.i PROPERTIES CPLUSPLUS ON)

    SET(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN)
    SET(CMAKE_SWIG_FLAGS -module ${java_module_name} -package "nc.PKMN" ${CMAKE_GLOBAL_SWIG_FLAGS})
    SWIG_ADD_MODULE(${swig_source} java ${CMAKE_CURRENT_BINARY_DIR}/${swig_source}.i)
    ADD_DEPENDENCIES(${SWIG_MODULE_${swig_source}_REAL_NAME} java_enums)
    ADD_DEPENDENCIES(${SWIG_MODULE_${swig_source}_REAL_NAME} CamelCase_i)
    IF(UNIX)
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_source}_REAL_NAME} PROPERTIES PREFIX "lib")
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_source}_REAL_NAME} PROPERTIES SUFFIX ".so")
    ELSE()
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_source}_REAL_NAME} PROPERTIES PREFIX "")
        SET_TARGET_PROPERTIES(${SWIG_MODULE_${swig_source}_REAL_NAME} PROPERTIES SUFFIX ".dll")
    ENDIF(UNIX)
    SWIG_LINK_LIBRARIES(${swig_source} pkmn)

    IF(WIN32)
        INSTALL(
            TARGETS ${SWIG_MODULE_${swig_source}_REAL_NAME}
            DESTINATION ${RUNTIME_DIR}
            COMPONENT "LibPKMN_Java"
        )
    ELSE()
        INSTALL(
            TARGETS ${SWIG_MODULE_${swig_source}_REAL_NAME}
            DESTINATION ${LIBRARY_DIR}
            COMPONENT "LibPKMN_Java"
        )
    ENDIF(WIN32)
ENDMACRO(JAVA_BUILD_SWIG_MODULE)
