/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>
#include <vector>

#include <pkmn/qt4/AbilityListComboBox.hpp>
#include <pkmn/lists.hpp>

namespace pkmn
{
    namespace qt4
    {
        AbilityListComboBox::AbilityListComboBox(unsigned int gen, QWidget* parent): QComboBox(parent)
        {
            std::vector<pkstring> abilities_vec;
            get_ability_list(abilities_vec, gen);

            for(unsigned int i = 0; i < abilities_vec.size(); i++)
            {
                addItem(tr(abilities_vec[i].const_char()), QVariant(i));
            }
        }
    }
} /* namespace pkmn */
