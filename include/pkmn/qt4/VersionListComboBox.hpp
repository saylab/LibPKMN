/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_VERSIONSCOMBOBOX_HPP
#define INCLUDED_VERSIONSCOMBOBOX_HPP

#include <pkmn/config.hpp>

#include <QComboBox>

namespace pkmn
{
    namespace qt4
    {
        //! A ComboBox that lists all versions.
        class PKMN_API VersionListComboBox: public QComboBox
        {
            Q_OBJECT

            public:
                VersionListComboBox(QWidget* parent = NULL);
        };
    }
}

#endif /* INCLUDED_VERSIONSCOMBOBOX_HPP */
