/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("database_java");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Failed to load database_java with the following error:\n" + e);
      System.exit(1);
    }
  }
%}

%include "exception.i"
%include "CamelCase.i"
%import "pkmn_java.i"

%{
    #include "pkmn/database/lists.hpp"
    #include "pkmn/database/queries.hpp"
%}

%include "pkmn/database/lists.hpp"
%include "pkmn/database/queries.hpp"
