/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import javax.swing.*;

/**
 * A ComboBox that lists all versions.
 */
public class VersionListComboBox extends JComboBox<String>
{
    public VersionListComboBox()
    {
        super();

        StringVector versionVec = new StringVector();
        Lists.getVersionList(versionVec);
        for(int i = 0; i < versionVec.size(); i++)
        {
            addItem(versionVec.get(i));
        }

        setEditable(false);
    }
}
