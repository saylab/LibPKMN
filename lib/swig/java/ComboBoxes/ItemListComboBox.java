/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import javax.swing.*;

/**
 * A ComboBox that lists all item available in the given generation.
 *
 * @param gen highest generation whose items to include
 */
public class ItemListComboBox extends JComboBox<String>
{
    public ItemListComboBox(int gen)
    {
        super();

        StringVector itemVec = new StringVector();
        Lists.getItemList(itemVec, gen);
        for(int i = 0; i < itemVec.size(); i++)
        {
            addItem(itemVec.get(i));
        }

        setEditable(false);
    }
}
