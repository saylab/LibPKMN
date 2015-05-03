#
# Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(${SWIG_USE_FILE})

FILE(TO_NATIVE_PATH "${LIBPKMN_SOURCE_DIR}/lib/swig" LIBPKMN_SWIG_SOURCE_DIR)
SET(LIBPKMN_SWIG_SOURCE_DIR ${LIBPKMN_SWIG_SOURCE_DIR} CACHE FILEPATH "LibPKMN SWIG source directory")
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN)
SET(JAVA_INSTALL_DIRECTORY share/java)

SET(LIBPKMN_JAVA_INCLUDE_DIRS
    ${LIBPKMN_SOURCE_DIR}/include
    ${LIBPKMN_BINARY_DIR}/lib/swig
    ${LIBPKMN_SWIG_SOURCE_DIR}
    ${LIBPKMN_SWIG_SOURCE_DIR}/java
    ${LIBPKMN_SWIG_SOURCE_DIR}/java/types
    ${LIBPKMN_SWIG_SOURCE_DIR}/types
    ${JAVA_INCLUDE_PATH}
    ${JNI_INCLUDE_DIRS}
)

SET(JAVA_ENUM_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/Abilities.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/EggGroups.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/Genders.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/Items.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/Moves.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/MoveClasses.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/Natures.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/Species.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/Stats.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/PokeBalls.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/Types.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/Versions.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/VersionGroups.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/PokemonForms.java
)

#Generate enums
ADD_CUSTOM_COMMAND(
    OUTPUT ${JAVA_ENUM_FILES}
    COMMAND ${PYTHON_EXECUTABLE} ${LIBPKMN_SOURCE_DIR}/tools/generate_enums.py --language=java --include-path=${LIBPKMN_SOURCE_DIR}/include/pkmn/enums.hpp --output-dir=${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN
    WORKING_DIRECTORY ${LIBPKMN_SOURCE_DIR}/tools
    COMMENT "Generating Java enums"
)
ADD_CUSTOM_TARGET(
    java_enums ALL
    DEPENDS ${LIBPKMN_SOURCE_DIR}/include/pkmn/enums.hpp
    DEPENDS ${JAVA_ENUM_FILES}
)

# Generate javadocs
ADD_CUSTOM_COMMAND(
    DEPENDS ${LIBPKMN_SOURCE_DIR}/tools/generate_javadocs.py
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pkmn_javadocs.i
    COMMAND ${PYTHON_EXECUTABLE} ${LIBPKMN_SOURCE_DIR}/tools/generate_javadocs.py --include-dir=${LIBPKMN_SOURCE_DIR}/include --output-dir=${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${PYTHON_EXECUTABLE} ${LIBPKMN_SOURCE_DIR}/tools/generate_javadocs.py --include-dir=${LIBPKMN_SOURCE_DIR}/lib/swig/java/types --output-dir=${CMAKE_CURRENT_BINARY_DIR} --append
    WORKING_DIRECTORY ${LIBPKMN_SOURCE_DIR}/tools
    COMMENT "Generating Javadocs"
)
ADD_CUSTOM_TARGET(
    javadocs ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/pkmn_javadocs.i
)

# Copy GUI files into JAR directory
ADD_SUBDIRECTORY(ComboBoxes)

INCLUDE(UseSWIG)
INCLUDE(LibPKMNJava)

JAVA_BUILD_SWIG_MODULE(pkmn_java PKMN)
JAVA_BUILD_SWIG_MODULE(buildinfo_java BuildInfo)
JAVA_BUILD_SWIG_MODULE(calculations_java Calculations)
JAVA_BUILD_SWIG_MODULE(database_java Database)
JAVA_BUILD_SWIG_MODULE(lists_java Lists)
JAVA_BUILD_SWIG_MODULE(paths_java Paths)

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/PKMN.jar
    COMMENT "Creating PKMN.jar"
    COMMAND ${PYTHON_EXECUTABLE} ${LIBPKMN_SOURCE_DIR}/tools/fix_java_files.py --java-dir=${LIBPKMN_BINARY_DIR}/lib/swig/java/nc/PKMN
    COMMAND ${Java_JAVAC_EXECUTABLE} -Xlint:unchecked -d ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/*.java
    COMMAND ${Java_JAR_EXECUTABLE} cf ${CMAKE_CURRENT_BINARY_DIR}/PKMN.jar -C ${CMAKE_CURRENT_BINARY_DIR} nc
    DEPENDS ${SWIG_MODULE_pkmn_java_REAL_NAME}
    DEPENDS ${SWIG_MODULE_buildinfo_java_REAL_NAME}
    DEPENDS ${SWIG_MODULE_calculations_java_REAL_NAME}
    DEPENDS ${SWIG_MODULE_database_java_REAL_NAME}
    DEPENDS ${SWIG_MODULE_lists_java_REAL_NAME}
    DEPENDS ${SWIG_MODULE_paths_java_REAL_NAME}
    DEPENDS java_enums
)
ADD_CUSTOM_TARGET(
    PKMN_jar ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/PKMN.jar
)
INSTALL(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/PKMN.jar
    DESTINATION ${JAVA_INSTALL_DIRECTORY}
    COMPONENT "LibPKMN_Java"
)
