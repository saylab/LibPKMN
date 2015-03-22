/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_IO_HPP
#define INCLUDED_PKMN_IO_HPP

#include <pkmn/config.hpp>
#include <pkmn/pokemon.hpp>

#include <pkmn/types/pkstring.hpp>

namespace pkmn
{
    namespace io
    {
        //! Exports a LibPKMN-format Pokémon to a .3gpkm file.
        /*!
         * The .3gpkm file format corresponds to a decrypted Generation III Pokémon.
         *
         * This function takes in a LibPKMN Pokémon and saves it as a .3gpkm file at the
         * given filename.
         *
         * \param pkmn LibPKMN-format Pokémon to export
         * \param filename where to save the .3gpkm file
         */
        void PKMN_API export_to_3gpkm(pokemon::sptr pkmn, const pkmn::pkstring &filename);

        //! Imports a .3gpkm file into LibPKMN.
        /*!
         * The .3gpkm file format corresponds to a decrypted Generation III Pokémon.
         *
         * This function takes in a .3gpkm file and converts it into a LibPKMN Pokémon.
         *
         * \param filename the .3gpkm file to import
         * \return imported LibPKMN-format Pokémon
         */
        pokemon::sptr PKMN_API import_from_3gpkm(const pkmn::pkstring &filename);

        //! Imports a .pkm file into LibPKMN.
        /*!
         * The .pkm file format corresponds to a decrypted Generation IV-V Pokémon.
         *
         * This function takes in a LibPKMN Pokémon and saves it as a .pkm file at the
         * given filename.
         *
         * \param pkmn LibPKMN-format Pokémon to export
         * \param filename where to save the .pkm file
         */
        void PKMN_API export_to_pkm(pokemon::sptr pkmn, const pkmn::pkstring &filename);

        //! Imports a .pkm file into LibPKMN.
        /*!
         * The .pkm file format corresponds to a decrypted Generation IV-V Pokémon.
         *
         * This function takes in a .pkm file and converts it into a LibPKMN Pokémon.
         *
         * \param filename the .pkm file to import
         * \return imported LibPKMN-format Pokémon
         */
        pokemon::sptr PKMN_API import_from_pkm(const pkmn::pkstring &filename);
    }
}

#endif /* INCLUDED_PKMN_IO_HPP */
