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
#include <pkmn/conversions/pokemon.hpp>
#include <pkmn/types/pkstring.hpp>

namespace pkmn
{
    /*
     * Easiest way to get SWIG to see these functions
     */
    namespace conversions
    {
        extern pokemon::sptr gen1_to_gen2(pokemon::sptr src);
        extern pokemon::sptr gen2_to_gen1(pokemon::sptr src);
        extern pokemon::sptr gen3_to_gen4(pokemon::sptr src);
        extern pokemon::sptr gen4_to_gen5(pokemon::sptr src);
    }

    namespace python
    {
        pokemon::sptr import_gen1_pokemon(PyObject* pyobject, const pkmn::pkstring &nickname = "",
                                                              const pkmn::pkstring &trainer_name = "");

        PyObject* export_gen1_pokemon(pokemon::sptr pkmn);

        pokemon::sptr import_gen2_pokemon(PyObject* pyobject, const pkmn::pkstring &nickname = "",
                                                              const pkmn::pkstring &trainer_name = "");

        PyObject* export_gen2_pokemon(pokemon::sptr pkmn);

        pokemon::sptr import_gen3_pokemon(PyObject* pyobject, const pkmn::pkstring &version,
                                                              bool is_encrypted = true);

        PyObject* export_gen3_pokemon(pokemon::sptr pkmn, bool encrypt = true);

        pokemon::sptr import_nds_pokemon(PyObject* pyobject, const pkmn::pkstring &version,
                                                             bool is_encrypted = true);

        PyObject* export_nds_pokemon(pokemon::sptr pkmn, bool encrypt = true);

    }
}

#endif /* INCLUDED_PKMN_PYTHON_POKEMON_HPP */
