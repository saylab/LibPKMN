/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("io_java");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Failed to load io_java with the following error:\n" + e);
      System.exit(1);
    }
  }
%}

%include "exception.i"
%include "CamelCase.i"
%import "pkmn_java.i"

%{
    #include "pkmn/io.hpp"
%}

%include "pkmn/io.hpp"
