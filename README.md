LibPKMN
===================================

The goal of LibPKMN is to be the most portable and universal library available for interacting with the main series Pokémon games, providing a high-level API that allows users to edit Pokémon game saves without needing the technical knowledge of what goes on under the hood. This game can interact with almost any Generation 1-5 .sav file, giving users access to party data, bag contents, and more. Without any games present, LibPKMN can also be used for team formation and strategy analysis.

This library can be built with any modern C++ compiler, and its API can be accessed from C++, C#, Java, and Python, with more to come.

## Feature Roadmap

 * Generation 2 game editing (rpokesav submodule)
 * Communication with Gen 4-5 retail games (nds_wifi branch)
 * Qt4 Pokémon Creator
 * Lua API
 * Game save generator

## Dependencies

 * CMake 2.8 or above (http://www.cmake.org)
 * Boost 1.48 or above (http://www.boost.org)
 * SQLite3 (http://www.sqlite.org)
 * Qt4 (http://www.qt-project.org)

## Git Submodules

 * LibPKMN Images (https://github.com/ncorgan/Pokemon-sim-images) - Pokemon sprites and icons
 * SQLiteCpp (https://github.com/SRombauts/SQLiteCpp) - SQLite3 C++ wrapper
 * Retro Pokesav (https://github.com/ncorgan/rpokesav) - Gen 1 save editing library
 * Pokehack (https://github.com/Grazfather/pokehack) - Gen 3 save editing library
 * PokeLib-NC (https://github.com/ncorgan/PokeLib-NC) - Gen 4 save editing library
 * PKMDS (https://github.com/codemonkey85/PKMDS) - Gen 5-6 save editing library

## Tested With

 * C++
    * GCC 4.6.3 (Ubuntu 12.04)
    * GCC 4.7.3 (Cygwin 1.7.25)
    * Clang 3.0 (Ubuntu 12.04)
    * Microsoft Visual Studio 2010 (Windows 7)
 * SWIG 2.0.10
 * Python 2.7.2
 * Java
    * JDK 1.7.0.40
 * C#
    * .NET v4.0.30319

## Acknowledgements

Pokémon © 2002-2014 Pokémon. © 1995-2014 Nintendo/Creatures Inc./GAME FREAK inc. TM, ® and Pokémon character names are trademarks of Nintendo. 

Pokémon info, algorithms, etc., all taken from Bulbapedia (http://bulbapedia.bulbagarden.net/wiki/Main_Page)
SQLite Database used courtesy of Alex Munroe (http://www.veekun.com).

Special thanks to Michael Bond (http://github.com/codemonkey85) for consulting on game mechanics.
 
README last updated: 2013-01-18
