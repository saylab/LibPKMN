/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using LibPKMN;
using System;

public class StringTest
{
    public static int Main(string[] args)
    {
        string abilityFromDatabase = Database.getAbilityName(Abilities.TORRENT);
        if(abilityFromDatabase != "Torrent")
        {
            throw new System.Exception("string -> pkmn::pkstring conversion failed.");
        }

        string_vec stringVector = new string_vec();
        LibPKMN.LibPKMN.getPokemonList(stringVector, Versions.RED);
        if(stringVector[3] != "Charmander")
        {
            throw new System.Exception("pkmn::pkstring -> string conversion failed.");
        }

        return 0;
    }    
}
