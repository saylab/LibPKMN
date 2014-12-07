/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_ABOUTMESSAGEBOX_HPP
#define INCLUDED_ABOUTMESSAGEBOX_HPP

#include <pkmn/config.hpp>

#include <QMessageBox>

namespace pkmn
{
    namespace qt4
    {
        //! An "About" MessageBox that can be used in LibPKMN's Qt4 applications.
        class PKMN_API AboutMessageBox: public QMessageBox
        {
            Q_OBJECT

            public:
                AboutMessageBox(QWidget* parent = NULL);
        };
    }
}

#endif /* INCLUDED_ABOUTMESSAGEBOX_HPP */
