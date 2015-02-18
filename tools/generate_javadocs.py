#!/usr/bin/env python
#
# Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

################################################################
# This script is called at build-time to generate the Javadoc
# SWIG commands.
################################################################

import header_converter
from header_converter import CppHeaderParser
import datetime
from optparse import OptionParser
import os

header_text = """/*
 * This file was generated: %s
 */""" % datetime.datetime.now()

ignored_files = ["config.hpp",
                 "dict.hpp",
                 "pkstring.hpp",
                 "shared_ptr.hpp"]

def get_javadocs(header):
    output = ""

    for fcn in header.functions:
        if "operator" not in fcn["name"].lower() and "anon" not in fcn["name"].lower() and fcn.has_key("doxygen"):
            doc = header_converter.documentation(fcn)
            output += "%s\n" % header_converter.documentation(fcn).swig_javadoc()

    for cls in header.classes:
        if header.classes[cls].has_key("doxygen"):
            output += "%s\n" % header_converter.documentation(header.classes[cls]).swig_javadoc()

        for fcn in header.classes[cls]["methods"]["public"]:
            if "operator" not in fcn["name"].lower() and not fcn["destructor"] and fcn.has_key("doxygen"):
                output += "%s\n" % header_converter.documentation(fcn).swig_javadoc()

        #for var in header.classes[cls]["properties"]["public"]:
        #    output += "%s\n" % header_converter.documentation(var).swig_javadoc()

    return output

if __name__ == "__main__":

    parser = OptionParser()
    parser.add_option("--include-dir", type="string", help="LibPKMN include directory")
    parser.add_option("--output-dir", type="string", help="CamelCase.i output directory")
    (options,args) = parser.parse_args()

    output = header_text + "\n\n"

    os.chdir(options.include_dir)
    for root, dirs, files in os.walk(os.getcwd()):
        for file in files:
            if file.endswith(".hpp") and file not in ignored_files and not root.endswith("conversions") and not root.endswith("native") and not root.endswith("qt4"):
                output += "%s\n" % get_javadocs(CppHeaderParser.CppHeader(os.path.join(root, file)))

    os.chdir(options.output_dir)
    f = open("pkmn_javadocs.i", 'w')
    f.write(output)
    f.close()
