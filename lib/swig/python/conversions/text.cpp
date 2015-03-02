/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <stdexcept>

#include <pkmn/conversions/text.hpp>

#include "text.hpp"

namespace pkmn
{
    namespace python
    {
        pkmn::pkstring import_gen1_text(PyObject* pyobject)
        {
            if(!PyString_Check(pyobject))
                throw std::runtime_error("Invalid input: not a string.");

            const char* str = PyString_AsString(pyobject);
            return pkmn::conversions::import_gen1_text((const uint8_t*)str, PyString_Size(pyobject));
        }

        pkmn::pkstring import_gen2_text(PyObject* pyobject)
        {
            if(!PyString_Check(pyobject))
                throw std::runtime_error("Invalid input: not a string.");

            const char* str = PyString_AsString(pyobject);
            return pkmn::conversions::import_gen2_text((const uint8_t*)str, PyString_Size(pyobject));
        }

        pkmn::pkstring import_gen3_text(PyObject* pyobject)
        {
            if(!PyString_Check(pyobject))
                throw std::runtime_error("Invalid input: not a string.");

            const char* str = PyString_AsString(pyobject);
            return pkmn::conversions::import_gen3_text((const uint8_t*)str, PyString_Size(pyobject));
        }
    } /* namespace python */
}
