/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_PYTHON_POKEMON_HPP
#define INCLUDED_PKMN_PYTHON_POKEMON_HPP

#include <string>

#include <Python.h>

#include <pkmn/pokemon.hpp>

namespace pkmn
{
    namespace python
    {
        pokemon::sptr import_gen1_pokemon(PyObject* pyList);

        PyObject* export_gen1_pokemon(pokemon::sptr pkmn, bool party = true);

        pokemon::sptr import_gen2_pokemon(PyObject* pyList);

        PyObject* export_gen2_pokemon(pokemon::sptr pkmn, bool party = true);
    }
}

#endif /* INCLUDED_PKMN_PYTHON_POKEMON_HPP */
