/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package org.nc.LibPKMN;

import javax.swing.*;

public class AbilityListComboBox extends JComboBox<String>
{
    public AbilityListComboBox(int gen)
    {
        super();

        WStringVector abilityVec = new WStringVector();
        LibPKMN.getAbilityList(abilityVec, gen);
        for(int i = 0; i < abilityVec.size(); i++)
        {
            addItem(abilityVec.get(i));
        }

        setEditable(false);
    }
}
