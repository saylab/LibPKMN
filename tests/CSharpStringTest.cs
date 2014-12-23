/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;

public class StringTest
{
    // Test database usage
    public static void DatabaseUsageTest()
    {
        try
        {
            String testString = "Torrent";
            String testString2 = PKMN.Database.getAbilityName(PKMN.Database.getAbilityID(testString));
            if(testString != testString2)
                throw new System.Exception("testString != testString2");

            uint testUInt1 = PKMN.Database.getItemID(PKMN.Database.getItemName(PKMN.Items.POKE_BALL));
            if(testUInt1 != PKMN.Items.POKE_BALL)
                throw new System.Exception("testUInt1 != PKMN.Items.POKE_BALL");

            uint testUInt2 = PKMN.Database.getSpeciesID(PKMN.Database.getSpeciesName(PKMN.Species.NIDORAN_M));
            if(testUInt2 != PKMN.Species.NIDORAN_M)
                throw new System.Exception("testUInt2 != PKMN.Species.NIDORAN_M");
        }
        catch(System.Exception ex)
        {
            System.Console.WriteLine("Caught exception:");
            System.Console.WriteLine("   " + ex.Message);
            System.Console.WriteLine(ex.StackTrace);
            System.Environment.Exit(1);
        }
    }

    // Test StringPair workaround (using std::string instead of pkmn::pkstring)
    public static void StringPairWorkaroundTest()
    {
        try
        {
            PKMN.BasePokemonSPtr bPKMN = PKMN.BasePokemon.make("Bulbasaur", "Diamond");
            PKMN.TeamPokemonSPtr tPKMN = PKMN.TeamPokemon.make("Bulbasaur", "Diamond", 50,
                                                               "None","None","None","None");

            PKMN.StringPair abilities = bPKMN.getAbilities();
            if(abilities.first != "Overgrow")
                throw new System.Exception("abilities.first != \"Overgrow\"");
            if(abilities.second != "None")
                throw new System.Exception("abilities.second != \"None\"");

            PKMN.StringPair types = bPKMN.getTypes();
            if(types.first != "Grass")
                throw new System.Exception("types.first != \"Grass\"");
            if(types.second != "Poison")
                throw new System.Exception("types.second != \"Poison\"");

            PKMN.StringPair types2 = tPKMN.getTypes();
            if(types2.first != "Grass")
                throw new System.Exception("types2.first != \"Grass\"");
            if(types2.second != "Poison")
                throw new System.Exception("types2.second != \"Poison\"");
        }
        catch(System.Exception ex)
        {
            System.Console.WriteLine("Caught exception:");
            System.Console.WriteLine("   " + ex.Message);
            System.Console.WriteLine(ex.StackTrace);
            System.Environment.Exit(1);
        }
    }

    public static int Main(string[] args)
    {
        DatabaseUsageTest();
        StringPairWorkaroundTest();

        return 0;
    }
}
