/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

import org.nc.LibPKMN.*;

public class JavaStringTest
{
    public static void main(String[] args)
    {
        String abilityFromDatabase = Database.getAbilityName((short)(Abilities.TORRENT));
        System.out.println(abilityFromDatabase);
        if(!abilityFromDatabase.equals("Torrent"))
        {
            throw new RuntimeException("String -> pkmn::pkstring conversion failed.");
        }

        StringVector stringVector = new StringVector();
        LibPKMN.getPokemonList(stringVector, (short)(Versions.RED));
        System.out.println(stringVector.get(3));
        if(!stringVector.get(3).equals("Charmander"))
        {
            throw new RuntimeException("pkmn::pkstring -> String conversion failed.");
        }
    }
}
