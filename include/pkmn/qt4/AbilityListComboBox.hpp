/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_ABILITIESCOMBOBOX_HPP
#define INCLUDED_ABILITIESCOMBOBOX_HPP

#include <cstdint>

#include <pkmn/config.hpp>

#include <QComboBox>

namespace pkmn
{
    namespace qt4
    {
        //! A ComboBox that lists all abilities available in the given generation.
        class PKMN_API AbilityListComboBox: public QComboBox
        {
            Q_OBJECT

            public:
                AbilityListComboBox(const uint16_t gen, QWidget* parent = NULL);
        };
    }
}

#endif /* INCLUDED_ABILITIESCOMBOBOX_HPP */
