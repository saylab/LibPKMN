/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import javax.swing.*;

/**
 * A ComboBox that lists all types present in the given generation.
 *
 * @param gen generation whose types to return
 */
public class TypeListComboBox extends JComboBox<String>
{
    public TypeListComboBox(int gen)
    {
        super();

        StringVector typeVec = new StringVector();
        Lists.getTypeList(typeVec, gen);
        for(int i = 0; i < typeVec.size(); i++)
        {
            addItem(typeVec.get(i));
        }

        setEditable(false);
    }
}
