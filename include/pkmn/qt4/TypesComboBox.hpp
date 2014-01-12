/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TYPESCOMBOBOX_HPP
#define INCLUDED_TYPESCOMBOBOX_HPP

#include <pkmn/config.hpp>

#include <QComboBox>

namespace pkmn
{
    namespace qt4
    {
        class PKMN_API TypesComboBox: public QComboBox
        {
            Q_OBJECT

            public:
                TypesComboBox(QWidget* parent = 0, int gen = 5);

            /*signals:

            slots:

            private:*/
        };
    }
}

#endif /* INCLUDED_TYPESCOMBOBOX_HPP */