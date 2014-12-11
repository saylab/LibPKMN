/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

import java.lang.RuntimeException;
import org.nc.PKMN.*;

public class JavaStringTest
{
    // Test database usage
    public static void DatabaseUsageTest()
    {
        try
        {
            String testString = "Torrent";
            String testString2 = Database.getAbilityName(Database.getAbilityID(testString));
            if(!testString.equals(testString2))
                throw new RuntimeException("testString != testString2");

            long testLong1 = Database.getItemID(Database.getItemName(Items.POKE_BALL));
            if(testLong1 != Items.POKE_BALL)
                throw new RuntimeException("testLong1 != Items.POKE_BALL");

            long testLong2 = Database.getSpeciesID(Database.getSpeciesName(Species.NIDORAN_M));
            if(testLong2 != Species.NIDORAN_M)
                throw new RuntimeException("testLong2 != Species.NIDORAN_M");
        }
        catch(RuntimeException ex)
        {
            System.out.println("Caught exception:");
            System.out.println("   " + ex.getMessage());
            System.out.println(ex.getStackTrace());
            System.exit(1);
        }
    }

    public static void main(String[] args)
    {
        DatabaseUsageTest();
    }
}
