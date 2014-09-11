/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>
#include <vector>

#include <pkmn/lists.hpp>
#include <pkmn/qt4/NatureListComboBox.hpp>

namespace pkmn
{
    namespace qt4
    {
        NatureListComboBox::NatureListComboBox(QWidget* parent): QComboBox(parent)
        {
            std::vector<pkstring> natures_vec;
            get_nature_list(natures_vec);

            for(int i = 0; i < natures_vec.size(); i++)
            {
                addItem(tr(natures_vec[i].const_char()), QVariant(i));
            }
        }
    } /* namespace qt4 */
} /* namespace pkmn */
