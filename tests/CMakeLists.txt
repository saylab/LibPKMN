#
# Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN)

#Get images dir
SET(IMAGES_PATH ${LIBPKMN_BINARY_DIR}/share/libpkmn/images)
FILE(TO_NATIVE_PATH ${IMAGES_PATH} IMAGES_PATH)

#Get SQLite database dir
SET(DATABASE_PATH ${LIBPKMN_BINARY_DIR}/sqlite3/libpkmn.db)
FILE(TO_NATIVE_PATH ${DATABASE_PATH} DATABASE_PATH)

#Source/binary dirs
FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR} TESTS_SOURCE_DIR)
FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_BINARY_DIR} TESTS_BINARY_DIR)

INCLUDE_DIRECTORIES(${LIBPKMN_SOURCE_DIR}/lib)
INCLUDE_DIRECTORIES(${LIBPKMN_SOURCE_DIR}/lib/swig/cs/types)
INCLUDE_DIRECTORIES(${LIBPKMN_SOURCE_DIR}/PokeLib-NC/include)
INCLUDE_DIRECTORIES(${LIBPKMN_SOURCE_DIR}/PKMDS/include)

MACRO(LIBPKMN_ADD_TEST test_name test_cmd)
    IF(WIN32)
        SET(TEST_CMD ${test_cmd})
        STRING(REPLACE "/" "\\" TEST_CMD "${TEST_CMD}")
        STRING(REPLACE "/" "\\" LIBPKMN_BINARY_DIR "${LIBPKMN_BINARY_DIR}")
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/unit_test_template.bat.in
            ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat
        @ONLY)
        ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat)
    ELSE()
        SET(TEST_CMD ${test_cmd})
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/unit_test_template.sh.in
            ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh
        @ONLY)
        ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh)
    ENDIF(WIN32)
ENDMACRO(LIBPKMN_ADD_TEST)

MACRO(LIBPKMN_ADD_CS_TEST test_name)
    SET(TEST_NAME ${test_name})
    IF(WIN32)
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/cs_unit_test_template.bat.in
            ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat
        @ONLY)
        ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat)
    ELSE()
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/cs_unit_test_template.sh.in
            ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh
        @ONLY)
        ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh)
    ENDIF(WIN32)
ENDMACRO(LIBPKMN_ADD_CS_TEST)

MACRO(LIBPKMN_ADD_JAVA_TEST test_name)
    SET(TEST_NAME ${test_name})
    IF(WIN32)
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/java_unit_test_template.bat.in
            ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat
        @ONLY)
        ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat)
    ELSE()
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/java_unit_test_template.sh.in
            ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh
        @ONLY)
        ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh)
    ENDIF(WIN32)
ENDMACRO(LIBPKMN_ADD_JAVA_TEST)

MACRO(LIBPKMN_ADD_PYTHON_TEST test_name)
    SET(TEST_NAME ${test_name})

    IF(WIN32)
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/python_unit_test_template.bat.in
            ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat
        @ONLY)
        ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bat)
    ELSE()
        SET(TEST_CMD "${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${test_name}.py")
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/unit_test_template.sh.in
            ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh
        @ONLY)
        ADD_TEST(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name}.sh)
    ENDIF(WIN32)
ENDMACRO(LIBPKMN_ADD_PYTHON_TEST)

##################################################
# C++ tests
##################################################
SET(cpp_test_sources
    datetime_test.cpp
    dict_test.cpp
    io_test.cpp
    markings_test.cpp
    pkstring_test.cpp
    prng_test.cpp
    sptr_test.cpp
    text_conversion_test.cpp
)

FOREACH(cpp_test ${cpp_test_sources})
    GET_FILENAME_COMPONENT(cpp_test_name ${cpp_test} NAME_WE)
    ADD_EXECUTABLE(${cpp_test_name} ${cpp_test})
    TARGET_LINK_LIBRARIES(${cpp_test_name} pkmn ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY})

    IF(WIN32)
        LIBPKMN_ADD_TEST(${cpp_test_name} "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${cpp_test_name}.exe")
    ELSE()
        LIBPKMN_ADD_TEST(${cpp_test_name} "${CMAKE_CURRENT_BINARY_DIR}/${cpp_test_name}")
    ENDIF(WIN32)
ENDFOREACH(cpp_test ${cpp_test_sources})

##################################################
# C# tests
##################################################
IF(ENABLE_CSHARP)
    SET(CSHARP_PLATFORM "x86")
    SET(CSHARP_BINARY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    SET(LIBRARY_PATH "${LIBPKMN_BINARY_DIR}/lib/swig/cs" CACHE FILEPATH "C# library directory")
    SET(PKMN_CS_DLL "${CSHARP_BINARY_DIRECTORY}/pkmn-cs.dll" CACHE FILEPATH "libpkmn-cs.dll location")

    SET(cs_test_names
        CSharpDictTest
        CSharpMarkingsTest
        CSharpSPtrTest
        CSharpStringTest
    )

    FOREACH(cs_test ${cs_test_names})
        CSHARP_ADD_EXECUTABLE(
            ${cs_test} "${PKMN_CS_DLL}" "System.Windows.Forms.dll" "${CMAKE_CURRENT_SOURCE_DIR}/${cs_test}.cs"
        )
        ADD_DEPENDENCIES(${cs_test} pkmn-cs)
        LIBPKMN_ADD_CS_TEST(${cs_test})
    ENDFOREACH(cs_test ${cs_test_names})
ENDIF(ENABLE_CSHARP)

##################################################
# Java tests
##################################################
IF(ENABLE_JAVA)
    FILE(TO_NATIVE_PATH "${LIBPKMN_BINARY_DIR}/lib/swig/java/PKMN.jar" PKMN_JAR)
    SET(PKMN_JAR ${PKMN_JAR} CACHE FILEPATH "PKMN.jar location")
    FILE(TO_NATIVE_PATH "${Java_JAVA_EXECUTABLE}" JAVA_EXEC)
    SET(JAVA_EXEC ${JAVA_EXEC} CACHE FILEPATH "Java executable")

    SET(java_test_names
        JavaDictTest
        JavaSPtrTest
        JavaStringTest
    )

    FOREACH(java_test ${java_test_names})
        ADD_CUSTOM_COMMAND(
            DEPENDS PKMN_jar
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${java_test}.java
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${java_test}.class
            COMMAND ${Java_JAVAC_EXECUTABLE} -classpath ${PKMN_JAR} -d ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${java_test}.java
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Building Java program ${java_test}"
        )
        ADD_CUSTOM_TARGET(
            ${java_test} ALL
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${java_test}.class
        )
        LIBPKMN_ADD_JAVA_TEST(${java_test})
    ENDFOREACH(java_test ${java_test_names})
ENDIF(ENABLE_JAVA)

##################################################
# Python tests
##################################################
IF(ENABLE_PYTHON)
    SET(python_test_names
        python_dict_test
        python_pkstring_test
        python_sptr_test
    )
    IF(${PYTHON_VERSION_MAJOR} EQUAL 2)
        LIST(APPEND python_test_names
            python_conversions_test
        )
    ENDIF(${PYTHON_VERSION_MAJOR} EQUAL 2)

    FILE(TO_NATIVE_PATH "${LIBPKMN_BINARY_DIR}/lib/swig" LIBPKMN_SWIG_DIR)
    SET(LIBPKMN_SWIG_DIR ${LIBPKMN_SWIG_DIR} CACHE FILEPATH "LibPKMN SWIG build directory")
    SET(PYTHON_PATH "${LIBPKMN_SWIG_DIR}")
    FILE(TO_NATIVE_PATH "${PYTHON_PATH}" PYTHON_PATH)

    FILE(TO_NATIVE_PATH "${PYTHON_EXECUTABLE}" PYTHON_EXEC)
    SET(PYTHON_EXEC ${PYTHON_EXEC} CACHE FILEPATH "Python executable")

    FOREACH(python_test ${python_test_names})
        LIBPKMN_ADD_PYTHON_TEST(${python_test})
    ENDFOREACH(python_test ${python_test_names})
ENDIF(ENABLE_PYTHON)
