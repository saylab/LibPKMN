cmake_minimum_required(VERSION 2.6)
project(PKMNSIM)

set(pkmnsim_VERSION_MAJOR 0)
set(pkmnsim_VERSION_MINOR 1)

FIND_PACKAGE(Boost COMPONENTS program_options)
IF(Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
    LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF(Boost_FOUND)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(lib)
add_subdirectory(include)

add_executable(poketest poketest.cpp)
target_link_libraries(poketest pkmnsim boost_program_options)