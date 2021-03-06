/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_NATURESCOMBOBOX_HPP
#define INCLUDED_NATURESCOMBOBOX_HPP

#include <pkmn/config.hpp>

#include <QComboBox>

namespace pkmn
{
    namespace qt4
    {
        //! A ComboBox that lists all 25 natures.
        class PKMN_API NatureListComboBox: public QComboBox
        {
            Q_OBJECT

            public:
                NatureListComboBox(QWidget* parent = NULL);
        };
    }
}

#endif /* INCLUDED_NATURESCOMBOBOX_HPP */
