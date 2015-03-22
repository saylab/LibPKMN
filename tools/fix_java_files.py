#!/usr/bin/env python
#
# Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

################################################################
# This script is called at build-time to add Doxygen tags to
# SWIG's Java output
################################################################

import datetime
from optparse import OptionParser
import os

def hide_ctors(filename):
    f = open(filename, "r")
    flines = f.readlines()
    f.close()

    if "hide_ctors" in flines[0] or "hide_ctors" in flines[1]:
        return

    # Find protected constructor
    ctor_pos = 0
    found = False
    for i in range(len(flines)):
        if "boolean cMemoryOwn" in flines[i]:
            ctor_pos = i
            found = True
            break

    if not found:
        return

    flines[ctor_pos-1] = "// \\cond\n"

    if "SPtr" not in filename:
        flines[ctor_pos+4] = "// \\endcond\n"

    f = open(filename, "w")
    f.write("// hide_ctors\n")
    for line in flines:
        f.write(line)
    f.close()

def fix_sptr_file(filename):
    f = open(filename, "r")
    flines = f.readlines()[:-1]
    f.close()

    if "fix_sptr_file" in flines[0] or "fix_sptr_file" in flines[1]:
        return

    # Find getGame
    line = flines[0]
    fcn_pos = 0
    for i in range(len(flines)):
        if "getGame" in flines[i]:
            fcn_pos = i
            break

    # From fcn_pos, find where Javadoc comment begins
    javadoc_start_pos = fcn_pos
    line = flines[javadoc_start_pos]
    while "/**" not in line and javadoc_start_pos >= 1:
        javadoc_start_pos -= 1
        line = flines[javadoc_start_pos]

    # If there is no documentation for getGame, just put //cond right before function
    if javadoc_start_pos == 0:
        javadoc_start_pos = fcn_pos

    flines[javadoc_start_pos-1] = "// \\endcond\n"

    # Add constructors to call base class's factory functions
    base_class_filename = filename.replace("SPtr","")
    g = open(base_class_filename, "r")
    glines = g.readlines()
    g.close()

    class_name = filename.split(".")[0]
    for line in glines:
        if ("public static %s make" % class_name) in line:
            make_line = line.rstrip("\n").lstrip(" ")
            fcn_str = "%s\n" % make_line.replace(("static %s make" % class_name), class_name)
            fcn_str += "    %s fromMake = %s.make(" % (class_name, class_name.replace("SPtr",""))
            fcn_str += "%s);\n" % make_line.split("(")[1].split(")")[0].replace("String ","").replace("int ","")
            fcn_str += "    this.swigCPtr = fromMake.swigCPtr;\n"
            fcn_str += "    this.swigCMemOwn = fromMake.swigCMemOwn;\n}\n"
            flines += [fcn_str]

    if class_name == "TrainerSPtr":
        flines += ["/**\n"]
        flines += [" * Public trainer ID compatible with a modern shiny.\n"]
        flines += [" */\n"]
        flines += ["public static final int LIBPKMN_PUBLIC_ID = 46479;\n"]
        flines += ["/**\n"]
        flines += [" * Secret trainer ID compatible with a modern shiny.\n"]
        flines += [" */\n"]
        flines += ["public static final int LIBPKMN_SECRET_ID = 2858;\n"]
        flines += ["/**\n"]
        flines += [" * Full trainer ID compatible with a modern shiny.\n"]
        flines += [" */\n"]
        flines += ["public static final long LIBPKMN_TRAINER_ID = 3046050602L;\n"]
    flines += ["}"]

    f = open(filename, "w")
    f.write("// fix_sptr_file\n")
    for line in flines:
        f.write(line)
    f.close()

if __name__ == "__main__":

    parser = OptionParser()
    parser.add_option("--java-dir", type="string", help="SWIG *.java output directory")
    (options,args) = parser.parse_args()

    os.chdir(options.java_dir)
    for root, dirs, files in os.walk(os.getcwd()):
        for file in files:
            hide_ctors(file)
            if file.endswith("SPtr.java"):
                fix_sptr_file(file)
