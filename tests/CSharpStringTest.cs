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
            String testString2 = LibPKMN.Database.getAbilityName(LibPKMN.Database.getAbilityID(testString));
            if(testString != testString2)
                throw new System.Exception("testString != testString2");

            uint testUInt1 = LibPKMN.Database.getItemID(LibPKMN.Database.getItemName(LibPKMN.Items.POKE_BALL));
            if(testUInt1 != LibPKMN.Items.POKE_BALL)
                throw new System.Exception("testUInt1 != LibPKMN.Items.POKE_BALL");

            uint testUInt2 = LibPKMN.Database.getSpeciesID(LibPKMN.Database.getSpeciesName(LibPKMN.Species.NIDORAN_M));
            if(testUInt2 != LibPKMN.Species.NIDORAN_M)
                throw new System.Exception("testUInt2 != LibPKMN.Species.NIDORAN_M");
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

        return 0;
    }
}
