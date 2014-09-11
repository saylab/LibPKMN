/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("database_java_swig");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Failed to load database_java_swig with the following error:\n" + e);
      System.exit(1);
    }
  }
%}

%include "exception.i"
%include "CamelCase.i"
%import "libpkmn_java_swig.i"

%{
    #include "pkmn/database/lists.hpp"
    #include "java_databasewrapper.hpp"
%}

%include "pkmn/database/lists.hpp"
%include "java_databasewrapper.hpp"
