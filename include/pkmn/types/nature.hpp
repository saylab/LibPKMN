/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_NATURE_HPP
#define INCLUDED_PKMN_NATURE_HPP

#include <pkmn/config.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/pkstring.hpp>

namespace pkmn
{
    /*!
     * This class represents a Pokémon's <a href="http://http://bulbapedia.bulbagarden.net/wiki/Nature">nature</a>.
     * This is randomly generated at a Pokémon's instantiation and affects two of its stats, lowering one by 10%
     * and raising another by 10%.
     *
     * There are 25 natures, each of which affects a different pair of stats.
     */
    class PKMN_API nature_t
    {
        public:

            nature_t() {};

            //! Constructor that uses the nature's SQLite ID.
            nature_t(unsigned int id);

            //! Constructor that uses the nature's name.
            nature_t(const pkmn::pkstring &name);

            ~nature_t() {};

            //! Return the nature's name.
            pkmn::pkstring get_name();

            //! Return the modifier (0.9, 1.0, or 1.1) for the given stat, using its name.
            const float& operator[](const pkmn::pkstring &key) const;

            //! Return the modifier (0.9, 1.0, or 1.1) for the given stat, using its SQLite ID.
            const float& operator[](const unsigned int &key) const;

            //! Return the nature's SQLite ID.
            unsigned int get_nature_id();

        private:

            unsigned int _id;
            pkmn::pkstring _name;

            pkmn::dict<pkmn::pkstring, float> _modifiers;

            void _populate_dict();
    };
}

#endif /* INCLUDED_PKMN_NATURE_HPP */
