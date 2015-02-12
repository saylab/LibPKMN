#
# Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

# Look for libraries and programs associated with different languages and
# register components accordingly.

FIND_PACKAGE(SWIG)
IF(SWIG_FOUND)
    FIND_PACKAGE(PythonLibs 2)

    # If Python 3 is being used, make sure it's Python 3.3 or above.
    IF(${PYTHON_VERSION_MAJOR} EQUAL 3 AND ${PYTHON_VERSION_MINOR} LESS 3)
        MESSAGE(FATAL_ERROR "If you're using Python 3, you must use Python 3.3 or above.")
    ENDIF(${PYTHON_VERSION_MAJOR} EQUAL 3 AND ${PYTHON_VERSION_MINOR} LESS 3)

    FIND_PACKAGE(CSharp)
    FIND_PACKAGE(Java)
    FIND_PACKAGE(JNI)
ENDIF(SWIG_FOUND)

LIBPKMN_REGISTER_COMPONENT("LibPKMN - Python API" ENABLE_PYTHON ON
                           "ENABLE_LIBPKMN;SWIG_FOUND;PYTHONLIBS_FOUND" OFF)
LIBPKMN_REGISTER_COMPONENT("LibPKMN - C# API" ENABLE_CSHARP ON
                           "ENABLE_LIBPKMN;CSHARP_FOUND" OFF)
LIBPKMN_REGISTER_COMPONENT("LibPKMN - Java API" ENABLE_JAVA ON
                           "ENABLE_LIBPKMN;JAVA_FOUND;JNI_FOUND" OFF)

####################################################################
# Generate CamelCase file if generating C# or Java wrappers
####################################################################
IF(ENABLE_CSHARP OR ENABLE_JAVA)
    FILE(GLOB_RECURSE header_files ${LIBPKMN_SOURCE_DIR}/include "*hpp")

    ADD_CUSTOM_COMMAND(
        DEPENDS ${LIBPKMN_SOURCE_DIR}/tools/generate_CamelCase.py
        DEPENDS ${header_files}
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/CamelCase.i
        COMMAND ${PYTHON_EXECUTABLE} ${LIBPKMN_SOURCE_DIR}/tools/generate_CamelCase.py --include-dir=${LIBPKMN_SOURCE_DIR}/include --output-dir=${CMAKE_CURRENT_BINARY_DIR}
        WORKING_DIRECTORY ${LIBPKMN_SOURCE_DIR}/tools
        COMMENT "Generating CamelCase.i"
    )
    ADD_CUSTOM_TARGET(
        CamelCase_i ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/CamelCase.i
    )
ENDIF(ENABLE_CSHARP OR ENABLE_JAVA)

####################################################################
# Add subdirectories as appropriate based on enabled SWIG wrappers
####################################################################

IF(ENABLE_PYTHON)
    ADD_SUBDIRECTORY(python)
ENDIF(ENABLE_PYTHON)
IF(ENABLE_CSHARP)
    ADD_SUBDIRECTORY(cs)
ENDIF(ENABLE_CSHARP)
IF(ENABLE_JAVA)
    ADD_SUBDIRECTORY(java)
ENDIF(ENABLE_JAVA)
