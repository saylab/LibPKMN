#!/usr/bin/env python
#
# Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

from . import Abilities
from . import Egg_Groups
from . import Forms
from . import Genders
from . import Items
from . import Move_Classes
from . import Moves
from . import Natures
from . import PokeBalls
from . import Species
from . import Stats
from . import Types
from . import Versions
from . import Version_Groups

from .pkmn_python import *
from . import calculations
from . import conversions
from . import database
from . import io
@IMPORT_QT4@

SQLITE3_VERSION = "3.7.14.1"
SQLITECPP_VERSION = "0.5.1"
BOOST_VERSION = "@Boost_MAJOR_VERSION@.@Boost_MINOR_VERSION@.@Boost_SUBMINOR_VERSION@"
QT4_VERSION = "@QT_VERSION_MAJOR@.@QT_VERSION_MINOR@.@QT_VERSION_PATCH@"
SWIG_VERSION = "@SWIG_VERSION@"

LIBPKMN_COMMIT = "@LIBPKMN_GIT_DESCRIBE@"
