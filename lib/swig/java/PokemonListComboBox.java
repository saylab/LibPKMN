/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package org.nc.LibPKMN;

import javax.swing.*;

public class PokemonListComboBox extends JComboBox<String>
{
    public PokemonListComboBox(int version_id)
    {
        super();

        StringVector pokemonVec = new StringVector();
        LibPKMN.getPokemonList(pokemonVec, version_id);
        for(int i = 0; i < pokemonVec.size(); i++)
        {
            addItem(pokemonVec.get(i));
        }

        setEditable(false);
    }
}
