/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_TYPESCOMBOBOX_HPP
#define INCLUDED_TYPESCOMBOBOX_HPP

#include <cstdint>

#include <pkmn/config.hpp>

#include <QComboBox>

namespace pkmn
{
    namespace qt4
    {
        //! A ComboBox that lists all types present in the given generation.
        class PKMN_API TypeListComboBox: public QComboBox
        {
            Q_OBJECT

            public:
                TypeListComboBox(const uint16_t gen, QWidget* parent = NULL);
        };
    }
}

#endif /* INCLUDED_TYPESCOMBOBOX_HPP */
