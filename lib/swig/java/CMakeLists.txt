#
# Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(${SWIG_USE_FILE})

FILE(TO_NATIVE_PATH "${LIBPKMN_SOURCE_DIR}/lib/swig" LIBPKMN_SWIG_SOURCE_DIR)
SET(LIBPKMN_SWIG_SOURCE_DIR ${LIBPKMN_SWIG_SOURCE_DIR} CACHE FILEPATH "LibPKMN SWIG source directory")
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN)
SET(JAVA_INSTALL_DIRECTORY share/java)

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
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/VenusaurForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/CharizardForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/BlastoiseForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/AlakazamForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/GengarForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/KangaskhanForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/PinsirForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/GyaradosForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/AerodactylForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/MewtwoForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/PichuForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/AmpharosForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/UnownForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/ScizorForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/HeracrossForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/HoundoomForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/TyranitarForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/BlazikenForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/GardevoirForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/MawileForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/AggronForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/MedichamForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/ManectricForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/CastformForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/BanetteForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/AbsolForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/DeoxysForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/BurmyForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/WormadamForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/CherrimForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/ShellosForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/GastrodonForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/GarchompForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/LucarioForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/AbomasnowForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/RotomForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/GiratinaForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/ShayminForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/ArceusForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/BasculinForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/DarmanitanForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/DeerlingForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/SawsbuckForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/TornadusForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/ThundurusForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/LandorusForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/KyuremForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/KeldeoForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/MeloettaForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/GenesectForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/VivillonForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/FlabebeForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/FloetteForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/FlorgesForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/FurfrouForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/MeowsticForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/AegislashForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/PumpkabooForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/GourgeistForms.java
    ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/XerneasForms.java
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
        ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/${filename}
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
    COMMAND ${Java_JAVAC_EXECUTABLE} -Xlint:unchecked -d ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/nc/PKMN/*.java
    COMMAND ${Java_JAR_EXECUTABLE} cf ${CMAKE_CURRENT_BINARY_DIR}/PKMN.jar -C ${CMAKE_CURRENT_BINARY_DIR} nc
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
