/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;

public class StringTest
{
    // Test database usage
    public static bool DatabaseUsageTest()
    {
        try
        {
            String testString = "Torrent";
            String testString2 = PKMN.Database.getAbilityName(PKMN.Database.getAbilityID(testString));
            if(testString != testString2)
                throw new System.Exception("testString != testString2");

            ushort testUShort1 = PKMN.Database.getItemID(PKMN.Database.getItemName((ushort)PKMN.Items.POKE_BALL));
            if(testUShort1 != PKMN.Items.POKE_BALL)
                throw new System.Exception("testUShort1 != PKMN.Items.POKE_BALL");

            ushort testUShort2 = PKMN.Database.getSpeciesID(PKMN.Database.getSpeciesName((ushort)PKMN.Species.NIDORAN_M));
            if(testUShort2 != PKMN.Species.NIDORAN_M)
                throw new System.Exception("testUShort2 != PKMN.Species.NIDORAN_M");

            return true;
        }
        catch(System.Exception ex)
        {
            System.Console.WriteLine("\nCaught exception: " + ex.Message);
            System.Console.WriteLine("Stacktrace:");
            System.Console.WriteLine(ex.StackTrace);
            return false;
        }
    }

    public static int Main(string[] args)
    {
        bool successful = true;

        successful = DatabaseUsageTest();

        return successful ? 0 : 1;
    }
}
