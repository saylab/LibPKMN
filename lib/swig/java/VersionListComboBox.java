/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package org.nc.LibPKMN;

import javax.swing.*;

public class VersionListComboBox extends JComboBox
{
    public VersionListComboBox(long gen)
    {
        super();

        string_vec versionVec = new string_vec();
        LibPKMN.getGameList(versionVec);
        for(int i = 0; i < versionVec.size(); i++)
        {
            addItem(versionVec.get(i));
        }

        setEditable(false);
    }
}
