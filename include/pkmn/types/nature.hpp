/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_NATURE_HPP
#define INCLUDED_PKMN_NATURE_HPP

#include <string>

#include <pkmn/config.hpp>
#include <pkmn/types/dict.hpp>

namespace pkmn
{
    /*!
     * This class represents a Pokémon's <a href="http://http://bulbapedia.bulbagarden.net/wiki/Nature">nature</a>.
     * This is randomly generated at a Pokémon's instantiation and affects two of its stats, lowering one by 10%
     * and raising another by 10%.
     *
     * There are 25 natures, each of which affects a different pair of stats.
     */
    class PKMN_API nature
    {
        public:

            nature() {};

            //! Constructor that uses the nature's SQLite ID.
            nature(unsigned int id);

            //! Constructor that uses the nature's name.
            nature(const std::string &name);

            ~nature() {};

            //! Return the nature's name.
            std::string get_name();

            //! Return the modifier (0.9, 1.0, or 1.1) for the given stat, using its name.
            const double& operator[](const std::string &key) const;

            //! Return the modifier (0.9, 1.0, or 1.1) for the given stat, using its SQLite ID.
            const double& operator[](const unsigned int &key) const;

            //! Return the nature's SQLite ID.
            unsigned int get_nature_id();

        private:

            unsigned int _id;
            std::string _name;

            pkmn::dict<std::string, double> _modifiers;

            void _populate_dict();
    };
}

#endif /* INCLUDED_PKMN_NATURE_HPP */
