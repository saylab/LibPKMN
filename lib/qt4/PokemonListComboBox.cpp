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
#include <pkmn/database/queries.hpp>
#include <pkmn/qt4/PokemonListComboBox.hpp>

namespace pkmn
{
    namespace qt4
    {
        PokemonListComboBox::PokemonListComboBox(const unsigned int version_id, QWidget* parent): QComboBox(parent)
        {
            std::vector<pkstring> pokemon_vec;
            get_pokemon_list(pokemon_vec, version_id);

            for(unsigned int i = 0; i < pokemon_vec.size(); i++)
            {
                QString name;
                #ifdef PKMN_PLATFORM_WIN32
                name = QString::fromUtf16((const uint16_t*)pokemon_vec[i].const_wchar_t());
                #else
                name = QString::fromWCharArray(pokemon_vec[i].const_wchar_t());
                #endif

                addItem(name, QVariant(i));
            }
        }
    } /* namespace qt4 */
} /* namespace pkmn */
