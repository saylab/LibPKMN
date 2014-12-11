#
# Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(${SWIG_USE_FILE})

FILE(TO_NATIVE_PATH "${LIBPKMN_SOURCE_DIR}/lib/swig" LIBPKMN_SWIG_SOURCE_DIR)
SET(LIBPKMN_SWIG_SOURCE_DIR ${LIBPKMN_SWIG_SOURCE_DIR} CACHE FILEPATH "LibPKMN SWIG source directory")
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN)
SET(JAVA_INSTALL_DIRECTORY share/java)

SET(JAVA_ENUM_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/Abilities.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/EggGroups.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/Genders.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/Items.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/Moves.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/MoveClasses.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/Natures.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/Species.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/Stats.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/PokeBalls.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/Types.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/Versions.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/Version_Groups.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/VenusaurForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/CharizardForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/BlastoiseForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/AlakazamForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/GengarForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/KangaskhanForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/PinsirForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/GyaradosForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/AerodactylForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/MewtwoForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/PichuForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/AmpharosForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/UnownForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/ScizorForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/HeracrossForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/HoundoomForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/TyranitarForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/BlazikenForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/GardevoirForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/MawileForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/AggronForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/MedichamForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/ManectricForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/CastformForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/BanetteForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/AbsolForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/DeoxysForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/BurmyForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/WormadamForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/CherrimForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/ShellosForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/GastrodonForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/GarchompForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/LucarioForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/AbomasnowForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/RotomForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/GiratinaForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/ShayminForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/ArceusForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/BasculinForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/DarmanitanForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/DeerlingForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/SawsbuckForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/TornadusForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/ThundurusForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/LandorusForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/KyuremForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/KeldeoForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/MeloettaForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/GenesectForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/VivillonForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/FlabebeForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/FloetteForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/FlorgesForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/FurfrouForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/MeowsticForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/AegislashForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/PumpkabooForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/GourgeistForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/XerneasForms.java
)

#Generate enums
ADD_CUSTOM_COMMAND(
    OUTPUT ${JAVA_ENUM_FILES}
    COMMAND ${PYTHON_EXECUTABLE} ${LIBPKMN_SOURCE_DIR}/tools/generate_enums.py --language=java --include-path=${LIBPKMN_SOURCE_DIR}/include/pkmn/enums.hpp --output-dir=${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN
    WORKING_DIRECTORY ${LIBPKMN_SOURCE_DIR}/tools
    COMMENT "Generating Java enums"
)
ADD_CUSTOM_TARGET(
    java_enums ALL
    DEPENDS ${LIBPKMN_SOURCE_DIR}/include/pkmn/enums.hpp
    DEPENDS ${JAVA_ENUM_FILES}
)

#Copy GUI files into JAR directory
SET(gui_files
    ${CMAKE_CURRENT_SOURCE_DIR}/AbilityListComboBox.java
    ${CMAKE_CURRENT_SOURCE_DIR}/NatureListComboBox.java
    ${CMAKE_CURRENT_SOURCE_DIR}/PokemonListComboBox.java
    ${CMAKE_CURRENT_SOURCE_DIR}/TypeListComboBox.java
    ${CMAKE_CURRENT_SOURCE_DIR}/VersionListComboBox.java
)
FOREACH(gui_file ${gui_files})
    GET_FILENAME_COMPONENT(filename ${gui_file} NAME)

    CONFIGURE_FILE(
        ${gui_file}
        ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/${filename}
    @ONLY)
ENDFOREACH(gui_file ${gui_files})

INCLUDE(UseSWIG)
INCLUDE(LibPKMNJava)

JAVA_BUILD_SWIG_MODULE(pkmn_java PKMN)
JAVA_BUILD_SWIG_MODULE(buildinfo_java BuildInfo)
JAVA_BUILD_SWIG_MODULE(calculations_java Calculations)
JAVA_BUILD_SWIG_MODULE(database_java Database)
JAVA_BUILD_SWIG_MODULE(io_java IO)
JAVA_BUILD_SWIG_MODULE(lists_java Lists)
JAVA_BUILD_SWIG_MODULE(paths_java Paths)

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/PKMN.jar
    COMMENT "Creating PKMN.jar"
    COMMAND ${Java_JAVAC_EXECUTABLE} -Xlint:unchecked -d ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/org/nc/PKMN/*.java
    COMMAND ${Java_JAR_EXECUTABLE} cf ${CMAKE_CURRENT_BINARY_DIR}/PKMN.jar -C ${CMAKE_CURRENT_BINARY_DIR} org
    DEPENDS ${SWIG_MODULE_pkmn_java_REAL_NAME}
    DEPENDS ${SWIG_MODULE_buildinfo_java_REAL_NAME}
    DEPENDS ${SWIG_MODULE_calculations_java_REAL_NAME}
    DEPENDS ${SWIG_MODULE_database_java_REAL_NAME}
    DEPENDS ${SWIG_MODULE_io_java_REAL_NAME}
    DEPENDS ${SWIG_MODULE_lists_java_REAL_NAME}
    DEPENDS ${SWIG_MODULE_paths_java_REAL_NAME}
    DEPENDS ${gui_files}
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
