/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

import java.lang.RuntimeException;
import nc.PKMN.*;

public class JavaStringTest
{
    // Test database usage
    public static boolean DatabaseUsageTest()
    {
        try
        {
            String testString = "Torrent";
            String testString2 = Database.getAbilityName(Database.getAbilityID(testString));
            if(!testString.equals(testString2))
                throw new RuntimeException("testString != testString2");

            int testInt1 = Database.getItemID(Database.getItemName(Items.POKE_BALL));
            if(testInt1 != Items.POKE_BALL)
                throw new RuntimeException("testInt1 != Items.POKE_BALL");

            int testInt2 = Database.getSpeciesID(Database.getSpeciesName(Species.NIDORAN_M));
            if(testInt2 != Species.NIDORAN_M)
                throw new RuntimeException("testInt2 != Species.NIDORAN_M");

            return true;
        }
        catch(RuntimeException ex)
        {
            System.out.println("\nCaught exception: " + ex.getMessage());
            System.out.println("Stacktrace:");
            for(int i = 0; i < ex.getStackTrace().length; i++)
                System.out.println(" * " + ex.getStackTrace()[i]);

            return false;
        }
    }

    public static void main(String[] args)
    {
        boolean successful = true;

        successful = DatabaseUsageTest();

        System.exit(successful ? 0 : 1);
    }
}
