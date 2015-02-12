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
        pokemon::sptr import_gen1_pokemon(PyObject* pyList)
        {
            if(!pyList)
                throw std::runtime_error("Invalid input.");

            if(!PyList_Check(pyList))
                throw std::runtime_error("Invalid input.");

            Py_ssize_t len = PyList_Size(pyList);
            if(len == sizeof(gen1_pc_pokemon_t))
            {
                uint8_t* buffer = new uint8_t[sizeof(gen1_pc_pokemon_t)];

                for(size_t i = 0; i < len; i++)
                    buffer[i] = uint8_t(PyInt_AsLong(PyList_GetItem(pyList, i)));

                gen1_pc_pokemon_t raw = *reinterpret_cast<gen1_pc_pokemon_t*>(buffer);

                // Use buffer for the nickname+otname buffers since we're setting those anyway
                pokemon::sptr ret = pkmn::conversions::import_gen1_pokemon(raw, buffer, buffer,
                                                                           "Red");
                ret->set_nickname(boost::algorithm::to_upper_copy(ret->get_pokedex_entry().species_name.std_wstring()));
                ret->set_trainer_name("LIBPKMN");
                delete[] buffer;

                return ret;
            }
            else if(len != sizeof(gen1_party_pokemon_t))
            {
                uint8_t* buffer = new uint8_t[sizeof(gen1_party_pokemon_t)];

                for(size_t i = 0; i < len; i++)
                    buffer[i] = uint8_t(PyInt_AsLong(PyList_GetItem(pyList, i)));

                gen1_party_pokemon_t raw = *reinterpret_cast<gen1_party_pokemon_t*>(buffer);

                // Use buffer for the nickname+otname buffers since we're setting those anyway
                pokemon::sptr ret = pkmn::conversions::import_gen1_pokemon(raw, buffer, buffer,
                                                                           "Red");
                ret->set_nickname(boost::algorithm::to_upper_copy(ret->get_pokedex_entry().species_name.std_wstring()));
                ret->set_trainer_name("LIBPKMN");
                delete[] buffer;

            }
            else
                throw std::runtime_error("Invalid input.");
        }

        PyObject* export_gen1_pokemon(pokemon::sptr pkmn, bool party)
        {
            if(pkmn->get_generation() != 1)
                throw std::runtime_error("Invalid input.");

            int len = party ? sizeof(gen1_party_pokemon_t)
                            : sizeof(gen1_pc_pokemon_t);
            PyObject* pyList = PyList_New(len);

            uint8_t* buffer = new uint8_t[len];
            memcpy(buffer, pkmn->get_native(), len);
            for(size_t i = 0; i < len; i++)
                PyList_SetItem(pyList, i, PyInt_FromLong((long)buffer[i]));

            delete[] buffer;

            return pyList;
        }

        pokemon::sptr import_gen2_pokemon(PyObject* pyList)
        {
            if(!pyList)
                throw std::runtime_error("Invalid input.");

            if(!PyList_Check(pyList))
                throw std::runtime_error("Invalid input.");

            Py_ssize_t len = PyList_Size(pyList);
            if(len == sizeof(gen2_pc_pokemon_t))
            {
                uint8_t* buffer = new uint8_t[sizeof(gen2_pc_pokemon_t)];

                for(size_t i = 0; i < len; i++)
                    buffer[i] = uint8_t(PyInt_AsLong(PyList_GetItem(pyList, i)));

                gen2_pc_pokemon_t raw = *reinterpret_cast<gen2_pc_pokemon_t*>(buffer);

                // Use buffer for the nickname+otname buffers since we're setting those anyway
                pokemon::sptr ret = pkmn::conversions::import_gen2_pokemon(raw, buffer, buffer,
                                                                           "Crystal");
                ret->set_nickname(boost::algorithm::to_upper_copy(ret->get_pokedex_entry().species_name.std_wstring()));
                ret->set_trainer_name("LIBPKMN");
                delete[] buffer;

                return ret;
            }
            else if(len != sizeof(gen2_party_pokemon_t))
            {
                uint8_t* buffer = new uint8_t[sizeof(gen2_party_pokemon_t)];

                for(size_t i = 0; i < len; i++)
                    buffer[i] = uint8_t(PyInt_AsLong(PyList_GetItem(pyList, i)));

                gen2_party_pokemon_t raw = *reinterpret_cast<gen2_party_pokemon_t*>(buffer);

                // Use buffer for the nickname+otname buffers since we're setting those anyway
                pokemon::sptr ret = pkmn::conversions::import_gen2_pokemon(raw, buffer, buffer,
                                                                           "Crystal");
                ret->set_nickname(boost::algorithm::to_upper_copy(ret->get_pokedex_entry().species_name.std_wstring()));
                ret->set_trainer_name("LIBPKMN");
                delete[] buffer;

            }
            else
                throw std::runtime_error("Invalid input.");
        }

        PyObject* export_gen2_pokemon(pokemon::sptr pkmn, bool party)
        {
            if(pkmn->get_generation() != 1)
                throw std::runtime_error("Invalid input.");

            int len = party ? sizeof(gen2_party_pokemon_t)
                            : sizeof(gen2_pc_pokemon_t);
            PyObject* pyList = PyList_New(len);

            uint8_t* buffer = new uint8_t[len];
            memcpy(buffer, pkmn->get_native(), len);
            for(size_t i = 0; i < len; i++)
                PyList_SetItem(pyList, i, PyInt_FromLong((long)buffer[i]));

            delete[] buffer;

            return pyList;
        }
    } /* namespace conversions */
} /* namespace pkmn */
