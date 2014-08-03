/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package org.nc.LibPKMN;

import javax.swing.*;

public class NatureListComboBox extends JComboBox
{
    public NatureListComboBox(long gen)
    {
        super();

        string_vec natureVec = new string_vec();
        LibPKMN.getNatureList(natureVec);
        for(int i = 0; i < natureVec.size(); i++)
        {
            addItem(natureVec.get(i));
        }

        setEditable(false);
    }
}
