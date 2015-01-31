/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>
#include <vector>

#include <pkmn/lists.hpp>
#include <pkmn/qt4/TypeListComboBox.hpp>

namespace pkmn
{
    namespace qt4
    {
        TypeListComboBox::TypeListComboBox(const uint16_t gen, QWidget* parent):
            QComboBox(parent)
        {
            std::vector<pkstring> types_vec;
            get_type_list(types_vec, gen);

            for(int i = 0; i < types_vec.size(); i++)
                addItem(QString::fromUtf16(types_vec[i]), QVariant(i));
        }
    } /* namespace qt4 */
} /* namespace pkmn */
