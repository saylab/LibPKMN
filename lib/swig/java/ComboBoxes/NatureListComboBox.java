/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import javax.swing.*;

/**
 * A ComboBox that lists all 25 natures.
 */
public class NatureListComboBox extends JComboBox<String>
{
    public NatureListComboBox()
    {
        super();

        StringVector natureVec = new StringVector();
        Lists.getNatureList(natureVec);
        for(int i = 0; i < natureVec.size(); i++)
        {
            addItem(natureVec.get(i));
        }

        setEditable(false);
    }
}
