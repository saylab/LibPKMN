/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cstdint>
#include <cstring>
#include <stdexcept>

#include <boost/algorithm/string.hpp>

#include <pkmn/database.hpp>
#include <pkmn/enums.hpp>
#include <pkmn/conversions/pokemon.hpp>

#include "pokemon.hpp"

using namespace pkmn::native;

namespace pkmn
{
    namespace python
    {
        pokemon::sptr import_gen1_pokemon(PyObject* pyobject)
        {
            if(!pyobject)
                throw std::runtime_error("Invalid input: null pointer.");

            // Validate input type
            bool is_string;
            if(PyList_Check(pyobject)) is_string = false;
            else if(PyString_Check(pyobject)) is_string = true;
            else throw std::runtime_error("Invalid input: not a list or string.");

            // Validate input size
            bool is_pc;
            Py_ssize_t len = is_string ? PyString_Size(pyobject) : PyList_Size(pyobject);
            if(len == sizeof(gen1_pc_pokemon_t)) is_pc = true;
            else if(len == sizeof(gen1_party_pokemon_t)) is_pc = false;
            else throw std::runtime_error("Invalid input size.");

            // Read input
            uint8_t* buffer;
            if(is_string)
                buffer = (uint8_t*)PyString_AsString(pyobject);
            else
            {
                buffer = new uint8_t[sizeof(gen1_party_pokemon_t)];
                for(size_t i = 0; i < len; i++)
                    buffer[i] = uint8_t(PyInt_AsLong(PyList_GetItem(pyobject, i)));
            }

            gen1_party_pokemon_t raw = *reinterpret_cast<gen1_party_pokemon_t*>(buffer);
            pokemon::sptr ret;

            // Can't use ternary operator with raw vs. raw.pc, so this is necessary
            if(is_pc) ret = pkmn::conversions::import_gen1_pokemon(raw.pc, buffer, buffer, "Red");
            else      ret = pkmn::conversions::import_gen1_pokemon(raw, buffer, buffer, "Red");

            if(is_string) delete[] buffer;
            ret->set_nickname(boost::algorithm::to_upper_copy(ret->get_pokedex_entry().species_name.std_wstring()));
            ret->set_trainer_name("LIBPKMN");
            return ret;
        }

        PyObject* export_gen1_pokemon(pokemon::sptr pkmn)
        {
            if(pkmn->get_generation() != 1)
                throw std::runtime_error("Invalid input: wrong generation.");

            int len = sizeof(gen1_party_pokemon_t);
            PyObject* pyList = PyList_New(len);

            uint8_t* buffer = new uint8_t[len];
            memcpy(buffer, pkmn->get_native(), len);
            for(size_t i = 0; i < len; i++)
                PyList_SetItem(pyList, i, PyInt_FromLong((long)buffer[i]));

            delete[] buffer;
            Py_INCREF(pyList);

            return pyList;
        }

        pokemon::sptr import_gen2_pokemon(PyObject* pyobject)
        {
            if(!pyobject)
                throw std::runtime_error("Invalid input: null pointer.");

            // Validate input type
            bool is_string;
            if(PyList_Check(pyobject)) is_string = false;
            else if(PyString_Check(pyobject)) is_string = true;
            else throw std::runtime_error("Invalid input: not a list or string.");

            // Validate input size
            bool is_pc;
            Py_ssize_t len = is_string ? PyString_Size(pyobject) : PyList_Size(pyobject);
            if(len == sizeof(gen2_pc_pokemon_t)) is_pc = true;
            else if(len == sizeof(gen2_party_pokemon_t)) is_pc = false;
            else throw std::runtime_error("Invalid input size.");

            // Read input
            uint8_t* buffer;
            if(is_string)
                buffer = (uint8_t*)PyString_AsString(pyobject);
            else
            {
                buffer = new uint8_t[sizeof(gen2_party_pokemon_t)];
                for(size_t i = 0; i < len; i++)
                    buffer[i] = uint8_t(PyInt_AsLong(PyList_GetItem(pyobject, i)));
            }

            gen2_party_pokemon_t raw = *reinterpret_cast<gen2_party_pokemon_t*>(buffer);
            pokemon::sptr ret;

            // Can't use ternary operator with raw vs. raw.pc, so this is necessary
            if(is_pc) ret = pkmn::conversions::import_gen2_pokemon(raw.pc, buffer, buffer, "Gold");
            else      ret = pkmn::conversions::import_gen2_pokemon(raw, buffer, buffer, "Gold");

            if(is_string) delete[] buffer;
            ret->set_nickname(boost::algorithm::to_upper_copy(ret->get_pokedex_entry().species_name.std_wstring()));
            ret->set_trainer_name("LIBPKMN");
            return ret;
        }

        PyObject* export_gen2_pokemon(pokemon::sptr pkmn)
        {
            if(pkmn->get_generation() != 2)
                throw std::runtime_error("Invalid input: wrong generation.");

            int len = sizeof(gen2_party_pokemon_t);
            PyObject* pyList = PyList_New(len);

            uint8_t* buffer = new uint8_t[len];
            memcpy(buffer, pkmn->get_native(), len);
            for(size_t i = 0; i < len; i++)
                PyList_SetItem(pyList, i, PyInt_FromLong((long)buffer[i]));

            delete[] buffer;
            Py_INCREF(pyList);

            return pyList;
        }
    } /* namespace conversions */
} /* namespace pkmn */
