#!/usr/bin/env python
#
# Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import Abilities
import Egg_Groups
import Forms
import Genders
import Items
import Move_Classes
import Moves
import Natures
import PokeBalls
import Ribbons
import Species
import Stats
import Types
import Versions
import Version_Groups

from pkmn_python import *
import calculations
import database
import io
@IMPORT_QT4@

SQLITE3_VERSION = "3.7.14.1"
SQLITECPP_VERSION = "0.5.1"
BOOST_VERSION = "@Boost_MAJOR_VERSION@.@Boost_MINOR_VERSION@.@Boost_SUBMINOR_VERSION@"
QT4_VERSION = "@QT_VERSION_MAJOR@.@QT_VERSION_MINOR@.@QT_VERSION_PATCH@"
SWIG_VERSION = "@SWIG_VERSION@"

LIBPKMN_COMMIT = "@LIBPKMN_GIT_DESCRIBE@"
