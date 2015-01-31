/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>
#include <vector>

#include <pkmn/qt4/VersionListComboBox.hpp>
#include <pkmn/lists.hpp>

namespace pkmn
{
    namespace qt4
    {
        VersionListComboBox::VersionListComboBox(QWidget* parent):
            QComboBox(parent)
        {
            std::vector<pkstring> games_vec;
            get_version_list(games_vec);

            for(uint16_t i = 0; i < games_vec.size(); i++)
                addItem(QString::fromUtf16(games_vec[i]), QVariant(i));
        }
    } /* namespace qt4 */
} /* namespace pkmn */
