/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cstdint>
#include <string>
#include <vector>

#include <pkmn/lists.hpp>
#include <pkmn/database.hpp>
#include <pkmn/qt4/PokemonListComboBox.hpp>

namespace pkmn
{
    namespace qt4
    {
        PokemonListComboBox::PokemonListComboBox(const unsigned int version_id, QWidget* parent):
            QComboBox(parent)
        {
            std::vector<pkstring> pokemon_vec;
            get_pokemon_list(pokemon_vec, version_id);

            for(unsigned int i = 0; i < pokemon_vec.size(); i++)
                addItem(QString::fromUtf16(pokemon_vec[i]), QVariant(i));
        }
    } /* namespace qt4 */
} /* namespace pkmn */
