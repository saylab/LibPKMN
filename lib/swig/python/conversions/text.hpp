/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_PYTHON_TEXT_HPP
#define INCLUDED_PKMN_PYTHON_TEXT_HPP

#include <Python.h>

#include <pkmn/types/pkstring.hpp>

namespace pkmn
{
    namespace python
    {
        pkmn::pkstring import_gen1_text(PyObject* pyobject);

        pkmn::pkstring import_gen2_text(PyObject* pyobject);

        pkmn::pkstring import_gen3_text(PyObject* pyobject);
    }
}

#endif /* INCLUDED_PKMN_PYTHON_TEXT_HPP */
