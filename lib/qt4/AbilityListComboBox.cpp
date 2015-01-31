/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>
#include <vector>

#include <pkmn/lists.hpp>
#include <pkmn/qt4/AbilityListComboBox.hpp>

namespace pkmn
{
    namespace qt4
    {
        AbilityListComboBox::AbilityListComboBox(const uint16_t gen, QWidget* parent):
            QComboBox(parent)
        {
            std::vector<pkstring> abilities_vec;
            get_ability_list(abilities_vec, gen);

            for(uint16_t i = 0; i < abilities_vec.size(); i++)
                addItem(QString::fromUtf16(abilities_vec[i]), QVariant(i));
        }
    }
} /* namespace pkmn */
