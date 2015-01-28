/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

import nc.PKMN.*;
import java.lang.RuntimeException;
import java.util.Arrays;

public class JavaSPtrTest
{
    /*  
     * Make sure two instances of BagSPtr properly point to the same underlying
     * bag::sptr. By adding items through each bag directly and through each of
     * their pockets, we can query all of them to make sure the change can be
     * seen with each method.
     */
    public static boolean BagPocketTest()
    {
        try
        {
            BagSPtr bag1 = Bag.make("Gold");
            BagSPtr bag2 = bag1;
            if(bag1 != bag2)
                throw new RuntimeException("bag1 != bag2");

            PocketDict bag1Pockets = bag1.getPockets();
            PocketDict bag2Pockets = bag2.getPockets();

            // Add items with each possible method
            bag1.addItem("Potion", 5);
            bag2.addItem("Master Ball", 10);
            bag1Pockets.get("KeyItems").addItem("Bicycle", 1);
            bag2Pockets.get("TM/HM").addItem("TM01", 15);

            // Make sure bag1 reports proper amounts
            if(bag1.getItemAmount("Potion") != 5)
                throw new RuntimeException("bag1.getItemAmount(\"Potion\") != 5");
            if(bag1.getItemAmount("Master Ball") != 10)
                throw new RuntimeException("bag1.getItemAmount(\"Master Ball\") != 10");
            if(bag1.getItemAmount("Bicycle") != 1)
                throw new RuntimeException("bag1.getItemAmount(\"Bicycle\") != 1");
            if(bag1.getItemAmount("TM01") != 15)
                throw new RuntimeException("bag1.getItemAmount(\"TM01\") != 15");

            // Make sure bag2 reports proper amounts
            if(bag2.getItemAmount("Potion") != 5)
                throw new RuntimeException("bag2.getItemAmount(\"Potion\") != 5");
            if(bag2.getItemAmount("Master Ball") != 10)
                throw new RuntimeException("bag2.getItemAmount(\"Master Ball\") != 10");
            if(bag2.getItemAmount("Bicycle") != 1)
                throw new RuntimeException("bag2.getItemAmount(\"Bicycle\") != 1");
            if(bag2.getItemAmount("TM01") != 15)
                throw new RuntimeException("bag2.getItemAmount(\"TM01\") != 15");

            // Make sure bag1Pockets reports proper amounts
            if(bag1Pockets.get("Items").getItemAmount("Potion") != 5)
                throw new RuntimeException("bag1Pockets.get(\"Items\").getItemAmount(\"Potion\") != 5");
            if(bag1Pockets.get("Balls").getItemAmount("Master Ball") != 10)
                throw new RuntimeException("bag1Pockets.get(\"Balls\").getItemAmount(\"Master Ball\") != 10");
            if(bag1Pockets.get("KeyItems").getItemAmount("Bicycle") != 1)
                throw new RuntimeException("bag1Pockets.get(\"KeyItems\").getItemAmount(\"Bicycle\") != 1");
            if(bag1Pockets.get("TM/HM").getItemAmount("TM01") != 15)
                throw new RuntimeException("bag1Pockets.get(\"TM/HM\").getItemAmount(\"TM01\") != 15");

            // Make sure bag1Pockets reports proper amounts
            if(bag2Pockets.get("Items").getItemAmount("Potion") != 5)
                throw new RuntimeException("bag2Pockets.get(\"Items\").getItemAmount(\"Potion\") != 5");
            if(bag2Pockets.get("Balls").getItemAmount("Master Ball") != 10)
                throw new RuntimeException("bag2Pockets.get(\"Balls\").getItemAmount(\"Master Ball\") != 10");
            if(bag2Pockets.get("KeyItems").getItemAmount("Bicycle") != 1)
                throw new RuntimeException("bag2Pockets.get(\"KeyItems\").getItemAmount(\"Bicycle\") != 1");
            if(bag2Pockets.get("TM/HM").getItemAmount("TM01") != 15)
                throw new RuntimeException("bag2Pockets.get(\"TM/HM\").getItemAmount(\"TM01\") != 15");

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

    /*
     * Make sure the = operator correclty points the new PokedexSPtr
     * to the same underlying pkmn::pokedex::sptr instance.
     */
    public static boolean PokedexTest()
    {
        try
        {
            PokedexSPtr pokedex1 = Pokedex.make("Red");
            PokedexSPtr pokedex2 = pokedex1;
            if(pokedex1 != pokedex2)
                throw new RuntimeException("pokedex1 != pokedex2");

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

    /*
     * Make sure the = operator correclty points the new PokemonSPtr
     * to the same underlying pkmn::pokemon::sptr instance.
     */
    public static boolean PokemonTest()
    {
        try
        {
            PokemonSPtr pokemon1 = Pokemon.make("Houndoom", "X", 50,
                                                "None", "None",
                                                "None", "None");
            PokemonSPtr pokemon2 = pokemon1;
            if(pokemon1 != pokemon2)
                throw new RuntimeException("pokemon1 != pokemon2");

            // Set form with pokemon1
            pokemon1.setForm("Mega");
            if(!pokemon2.getForm().equals("Mega"))
                throw new RuntimeException("!pokemon2.getForm().equals(\"Mega\"");

            // Set form with pokemon2
            pokemon2.setForm("Mega");
            if(!pokemon1.getForm().equals("Mega"))
                throw new RuntimeException("!pokemon1.getForm().equals(\"Mega\"");

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

    /*
     * Make sure the = operator correctly points the new PRNGSPtr
     * to the same underlying pkmn::prng::sptr instance.
     *
     * We will seed each instance with the same value and check to
     * see if the other produces the same output.
     */
    public static boolean PRNGTest()
    {
        try
        {
            PRNGSPtr prng1 = PRNG.make(3);
            PRNGSPtr prng2 = prng1;
            if(prng1 != prng2)
                throw new RuntimeException("prng1 != prng2");

            // Set value with prng1
            prng1.seedARNG(123456);
            long ARNG1Output1 = prng1.ARNG();
            prng1.seedARNG(123456);
            long ARNG2Output1 = prng2.ARNG();
            if(ARNG1Output1 != ARNG2Output1)
                throw new RuntimeException("ARNG1Output1 != ARNG2Output1");

            // Set value with prng2
            prng2.seedARNG(654321);
            long ARNG1Output2 = prng1.ARNG();
            prng2.seedARNG(654321);
            long ARNG2Output2 = prng2.ARNG();
            if(ARNG1Output2 != ARNG2Output2)
                throw new RuntimeException("ARNG1Output2 != ARNG2Output2");

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

    /*
     * Make sure the trainer shared_ptr copies correctly.
     */
    public static boolean TrainerTest()
    {
        try
        {
            TrainerSPtr trainer1 = Trainer.make("Red", "Red", "Male");
            TrainerSPtr trainer2 = trainer1;
            if(trainer1 != trainer2)
                throw new RuntimeException("trainer1 != trainer2");

            // Set name with trainer1
            trainer1.setName("Blue");
            if(trainer2.getName() != "Blue")
                throw new RuntimeException("trainer2.getName() != \"Blue\"");

            // Set name with trainer2
            trainer2.setName("Red");
            if(trainer1.getName() != "Red")
                throw new RuntimeException("trainer1.getName() != \"Red\"");

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

        successful = BagPocketTest() && successful;
        // TODO: GameSaveTest
        successful = PokedexTest() && successful;
        successful = PokemonTest() && successful;
        successful = PRNGTest() && successful;
        successful = TrainerTest() && successful;

        System.exit(successful ? 0 : 1);
    }    
}
