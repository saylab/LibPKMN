/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package org.nc.LibPKMN;

import javax.swing.*;

public class TypeListComboBox extends JComboBox<String>
{
    public TypeListComboBox(long gen)
    {
        super();

        string_vec typeVec = new string_vec();
        LibPKMN.getTypeList(typeVec, gen);
        for(int i = 0; i < typeVec.size(); i++)
        {
            addItem(typeVec.get(i));
        }

        setEditable(false);
    }
}
