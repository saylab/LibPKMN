/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import javax.swing.*;

/**
 * A ComboBox that lists all abilities available in the given generation.
 *
 * @param gen highest generation whose abilities to include
 */
public class AbilityListComboBox extends JComboBox<String>
{
    public AbilityListComboBox(int gen)
    {
        super();

        StringVector abilityVec = new StringVector();
        Lists.getAbilityList(abilityVec, gen);
        for(int i = 0; i < abilityVec.size(); i++)
        {
            addItem(abilityVec.get(i));
        }

        setEditable(false);
    }
}
