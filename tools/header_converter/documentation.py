#!/usr/bin/env python
#
# Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import sys

#import .CamelCase
import CppHeaderParser

"""
Returns namespace + class name (if applicable) + function name (if applicable).
"""
def assemble_full_name(cpp_input, fcn):
    full_name = ""

    if cpp_input.has_key("parent"):
        return "%s::%s" % (assemble_full_name(cpp_input["parent"], False), cpp_input["name"])
    else:
        if fcn:
            return "%s%s" % (cpp_input["namespace"], cpp_input["name"])
        else:
            return "%s::%s" % (cpp_input["namespace"], cpp_input["name"])

    return full_name

def remove_html(string):
    major_version = sys.version_info[0]

    if major_version == 2:
        from HTMLParser import HTMLParser

        class MLStripper(HTMLParser):
            def __init__(self):
                self.reset()
                self.fed = []
            def handle_data(self, d):
                self.fed.append(d)
            def get_data(self):
                return ''.join(self.fed)

        s = MLStripper()
        s.feed(string)
    else:
        from html.parser import HTMLParser

        class MLStripper(HTMLParser):
            def __init__(self):
                self.reset()
                self.strict = False
                self.convert_charrefs= True
                self.fed = []
            def handle_data(self, d):
                self.fed.append(d)
            def get_data(self):
                return ''.join(self.fed)

    return s.get_data()

class documentation():

    def __init__(self, cpp_input):
        if not (cpp_input.has_key("name") and cpp_input.has_key("doxygen") and cpp_input.has_key("namespace")):
            raise Exception("Invalid input.")

        self._input = cpp_input
        self._full_name = assemble_full_name(self._input)
        self._fcn = (type(self._input) == "CppHeaderParser.CppHeaderParser.CppMethod")
        self._short_doc = ""
        self._long_doc = ""
        self._returns = ""
        self._params = dict()

        # Parse Doxygen input
        doc = self._input["doxygen"]
        if doc != "":
            for line in doc.split("\n"):
                if line.startswith("//!"):
                    self._short_doc = line.split("/")[2].split("! ")[1]
                elif line.startswith(" * \\param"):
                    self._params[line.split(" ")[2]] = " ".join(line.split(" ")[3:])
                elif line.startswith(" * \\return"):
                    self._returns = " ".join(line.split(" ")[2:])
                elif line.startswith(" * "):
                    self._long_doc += "%s\n" % line[2:]

    """
    Returns the original C++ Doxygen documentation. CppHeaderParser screws up the input,
    so this returns how it should look.
    """
    def cpp_doc(self):
        output = ""
        if self._short_doc != "":
            output += "//! %s\n" % self._short_doc
        if self._long_doc != "":
            output += "/*!\n"
            for line in self._long_doc:
                output += " * %s\n" % line
        for key in self._params:
            output += " * \\param %s %s\n" % (key, self._params[key])
        if self._returns != "":
            output += " * \\return %s\n" % self._returns
        output += "*/"

        return output

    """
    Returns a C# XML comment corresponding to the given class/function.
    """
    def csharp_doc(self):
        output = ""
        if self._short_doc != "":
            output += "/// <summary>%s</summary>" % self._short_doc
        if self._long_doc != "":
            output += "/// <remarks>\n"
            for line in self._long_doc.split("\n"):
                output += " /// %s\n" % line
            output += "/// </remarks>\n"
        for key in self._params:
            output += " /// <param name=\"%s\">%s</param>\n" % (key, self._params[key])
        if self._returns != "":
            output += " /// <returns>%s</returns>" % self._params

        return output

    def _javadoc(self):
        output = "/**\n * %s\n\n *\n *\n" % self._short_doc
        if self._long_doc != "":
            for line in self._long_doc.split("\n"):
                output += " * %s\n" % line
            output += " *\n"
        for key in self._params:
            output += " * @param %s %s\n" % (key, self._params[key])
        if self._returns != "":
            output += " * @return %s\n" % self._returns
        output += "*/"

        return output

    """
    Returns a SWIG Javadoc line corresponding to the given class/function.
    """
    def swig_javadoc(self):
        if self._fcn:
            return "%%javamethodmodifiers %s() \"\n%s\npublic\";" % (self._full_name, _javadoc())
        else:
            return "%%typemap(javaimports) %s \"\n%s\"" % (self._full_name, _javadoc())

    """
    Returns a SWIG %feature("docstring") line corresponding to the given class/function.
    """
    def swig_python_docstring(self):
        output = "%%feature(\"docstring\") %s \"" % self._full_name
        if self._short_doc != "":
            output += "%s\n\n" % self._short_doc
        if self._long_doc != "":
            output += self._long_doc
        if len(self._params.keys()) > 0:
            output += "\n\nArgs:\n"
            for key in self._params:
                output += "    key : \n"
        output += "\""

        return output
