/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_ITEMLISTCOMBOBOX_HPP
#define INCLUDED_ITEMLISTCOMBOBOX_HPP

#include <cstdint>

#include <pkmn/config.hpp>

#include <QComboBox>

namespace pkmn
{
    namespace qt4
    {
        //! A ComboBox that lists all Pokémon available in the given version.
        class PKMN_API ItemListComboBox: public QComboBox
        {
            Q_OBJECT

            public:
                ItemListComboBox(const uint16_t version_id, QWidget* parent = NULL);
        };
    }
}

#endif /* INCLUDED_ITEMLISTCOMBOBOX_HPP */
