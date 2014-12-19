LibPKMN
===================================

The goal of LibPKMN is to be the most portable and universal library available for interacting with the main series Pokémon games, providing a high-level API that allows users to edit Pokémon game saves without needing the technical knowledge of what goes on under the hood. This game can interact with almost any Generation 1-5 .sav file, giving users access to party data, bag contents, and more. Without any games present, LibPKMN can also be used for team formation and strategy analysis.

This library can be built with any modern C++ compiler, and its API can be accessed from C++, C#, Java, and Python, with more to come.

## Feature Roadmap

 * Qt4 Pokémon Creator
 * Lua API
 * Game save generator

## Build Dependencies

 * CMake 2.8 or above (http://www.cmake.org)
 * Boost 1.48 or above (http://www.boost.org)
 * SQLite3 (http://www.sqlite.org)
 * Qt4 (http://www.qt-project.org)
 * Python (http://www.python.org)
 * PLY (https://pypi.python.org/pypi/ply)

## Build Instructions

To build in a UNIX-like environment, run these commands:

<pre>
cmake -DCMAKE_INSTALL_PREFIX=(desired install prefix) (LibPKMN source dir)
make install
</pre>

To build with MSVC, run:

<pre>
cmake -DCMAKE_INSTALL_PREFIX=(desired install prefix) (LibPKMN source dir)
devenv.com LibPKMN.sln /build release /project install
</pre>

The generated MSVC solution file can also be built from the MSVC IDE. The ALL_BUILD project will compile the project, and the INSTALL project will install the files in the install prefix.

If CMake does not automatically find the dependencies, this means they are not in the PATH. In this case, you can manually specify their locations from the CMake GUI.

## Git Submodules

 * LibPKMN Images (https://github.com/ncorgan/LibPKMN-images) - Pokemon sprites and icons
 * PokeLib-NC (https://github.com/ncorgan/PokeLib-NC) - Gen 4 save editing library
 * PKMDS (https://github.com/codemonkey85/PKMDS) - Gen 5-6 save editing library

## Acknowledgements

Pokémon © 2002-2014 Pokémon. © 1995-2014 Nintendo/Creatures Inc./GAME FREAK inc. TM, ® and Pokémon character names are trademarks of Nintendo. 

Pokémon info, algorithms, etc., all taken from Bulbapedia (http://bulbapedia.bulbagarden.net/wiki/Main_Page)
SQLite Database used courtesy of Alex Munroe (http://www.veekun.com).

Special thanks to Michael Bond (http://github.com/codemonkey85) for C# testing and consulting on game mechanics.

README last updated: 2014-12-18
