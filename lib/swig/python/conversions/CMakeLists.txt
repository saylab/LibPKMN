#
# Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

CMAKE_POLICY(SET CMP0022 OLD)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PYTHON_INCLUDE_DIRS}
)

SET(python_conversions_libraries
    pkmn
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARIES}
)

SET(pkmn_conversions_python_sources
    pokemon.cpp
    text.cpp
)

ADD_LIBRARY(pkmn_conversions_python STATIC ${pkmn_conversions_python_sources})
TARGET_LINK_LIBRARIES(pkmn_conversions_python ${python_conversions_libraries})

INCLUDE(LibPKMNPython)
PYTHON_BUILD_SWIG_MODULE(conversions_python pkmn/conversions)
