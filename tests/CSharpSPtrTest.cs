/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

public class CopySPtrTest
{
    /*
     * Make sure two instances of BagSPtr properly point to the same underlying
     * bag::sptr. By adding items through each bag directly and through each of
     * their pockets, we can query all of them to make sure the change can be
     * seen with each method.
     */
    public static bool BagPocketTest()
    {
        try
        {
            PKMN.BagSPtr bag1 = PKMN.Bag.make("Gold");
            PKMN.BagSPtr bag2 = bag1;
            if(bag1 != bag2)
                throw new System.Exception("bag1 != bag2");

            PKMN.PocketDict bag1Pockets = bag1.getPockets();
            PKMN.PocketDict bag2Pockets = bag2.getPockets();

            // Add items with each possible method
            bag1.addItem("Potion", 5);
            bag2.addItem("Master Ball", 10);
            bag1Pockets["KeyItems"].addItem("Bicycle", 1);
            bag2Pockets["TM/HM"].addItem("TM01", 15);

            // Make sure bag1 reports proper amounts
            if(bag1.getItemAmount("Potion") != 5)
                throw new System.Exception("bag1.getItemAmount(\"Potion\") != 5");
            if(bag1.getItemAmount("Master Ball") != 10)
                throw new System.Exception("bag1.getItemAmount(\"Master Ball\") != 10");
            if(bag1.getItemAmount("Bicycle") != 1)
                throw new System.Exception("bag1.getItemAmount(\"Bicycle\") != 1");
            if(bag1.getItemAmount("TM01") != 15)
                throw new System.Exception("bag1.getItemAmount(\"TM01\") != 15");

            // Make sure bag2 reports proper amounts
            if(bag2.getItemAmount("Potion") != 5)
                throw new System.Exception("bag2.getItemAmount(\"Potion\") != 5");
            if(bag2.getItemAmount("Master Ball") != 10)
                throw new System.Exception("bag2.getItemAmount(\"Master Ball\") != 10");
            if(bag2.getItemAmount("Bicycle") != 1)
                throw new System.Exception("bag2.getItemAmount(\"Bicycle\") != 1");
            if(bag2.getItemAmount("TM01") != 15)
                throw new System.Exception("bag2.getItemAmount(\"TM01\") != 15");

            // Make sure bag1Pockets reports proper amounts
            if(bag1Pockets["Items"].getItemAmount("Potion") != 5)
                throw new System.Exception("bag1Pockets[\"Items\"].getItemAmount(\"Potion\") != 5");
            if(bag1Pockets["Balls"].getItemAmount("Master Ball") != 10)
                throw new System.Exception("bag1Pockets[\"Balls\"].getItemAmount(\"Master Ball\") != 10");
            if(bag1Pockets["KeyItems"].getItemAmount("Bicycle") != 1)
                throw new System.Exception("bag1Pockets[\"KeyItems\"].getItemAmount(\"Bicycle\") != 1");
            if(bag1Pockets["TM/HM"].getItemAmount("TM01") != 15)
                throw new System.Exception("bag1Pockets[\"TM/HM\"].getItemAmount(\"TM01\") != 15");

            // Make sure bag2Pockets reports proper amounts
            if(bag2Pockets["Items"].getItemAmount("Potion") != 5)
                throw new System.Exception("bag2Pockets[\"Items\"].getItemAmount(\"Potion\") != 5");
            if(bag2Pockets["Balls"].getItemAmount("Master Ball") != 10)
                throw new System.Exception("bag2Pockets[\"Balls\"].getItemAmount(\"Master Ball\") != 10");
            if(bag2Pockets["KeyItems"].getItemAmount("Bicycle") != 1)
                throw new System.Exception("bag2Pockets[\"KeyItems\"].getItemAmount(\"Bicycle\") != 1");
            if(bag2Pockets["TM/HM"].getItemAmount("TM01") != 15)
                throw new System.Exception("bag2Pockets[\"TM/HM\"].getItemAmount(\"TM01\") != 15");

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

    /*
     * Make sure the = operator correctly points the new PokedexSPtr
     * to the same underlying pkmn::pokedex::sptr instance.
     */
    public static bool PokedexTest()
    {
        try
        {
            PKMN.PokedexSPtr pokedex1 = PKMN.Pokedex.make("Red");
            PKMN.PokedexSPtr pokedex2 = pokedex1;
            if(pokedex1 != pokedex2)
                throw new System.Exception("pokedex1 != pokedex2");

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

    /*
     * Make sure the = operator correctly points the new PokemonSPtr
     * to the same underlying pkmn::pokemon::sptr instance.
     */
    public static bool PokemonTest()
    {
        try
        {
            PKMN.PokemonSPtr pokemon1 = PKMN.Pokemon.make("Shaymin", "Platinum", 50,
                                                          "None", "None",
                                                          "None", "None");
            PKMN.PokemonSPtr pokemon2 = pokemon1;
            if(pokemon1 != pokemon2)
                throw new System.Exception("pokemon1 != pokemon2");

            // Set form with pokemon1
            pokemon1.setForm("Sky");
            if(pokemon2.getForm() != "Sky")
                throw new System.Exception("pokemon2.getForm() != \"Sky\"");

            // Set form with pokemon2
            pokemon2.setForm("Land");
            if(pokemon1.getForm() != "Land")
                throw new System.Exception("pokemon1.getForm() != \"Land\"");

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

    /*
     * Make sure the = operator correctly points the new PRNGSPtr
     * to the same underlying pkmn::prng::sptr instance.
     *
     * We will seed each instance with the same value and check to
     * see if the other produces the same output.
     */
    public static bool PRNGTest()
    {
        try
        {
            PKMN.PRNGSPtr prng1 = PKMN.PRNG.make(3);
            PKMN.PRNGSPtr prng2 = prng1;
            if(prng1 != prng2)
                throw new System.Exception("prng1 != prng2");

            // Set value with prng1
            prng1.seedARNG(123456);
            uint ARNG1Output1 = prng1.ARNG();
            prng1.seedARNG(123456);
            uint ARNG2Output1 = prng2.ARNG();
            if(ARNG1Output1 != ARNG2Output1)
                throw new System.Exception("ARNG1Output1 != ARNG2Output1");

            // Set value with prng2
            prng2.seedARNG(654321);
            uint ARNG1Output2 = prng1.ARNG();
            prng2.seedARNG(654321);
            uint ARNG2Output2 = prng2.ARNG();
            if(ARNG1Output2 != ARNG2Output2)
                throw new System.Exception("ARNG1Output2 != ARNG2Output2");

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

    /*
     * Make sure the = operator correctly points the new TrainerSPtr
     * to the same underlying pkmn::trainer::sptr instance.
     */
    public static bool TrainerTest()
    {
        try
        {
            PKMN.TrainerSPtr trainer1 = PKMN.Trainer.make("Red", "Red", "Male");
            PKMN.TrainerSPtr trainer2 = trainer1;
            if(trainer1 != trainer2)
                throw new System.Exception("trainer1 != trainer2");

            // Set name with trainer1
            trainer1.setName("Blue");
            if(trainer2.getName() != "Blue")
                throw new System.Exception("trainer2.getName() != \"Blue\"");

            // Set name with trainer2
            trainer2.setName("Red");
            if(trainer1.getName() != "Red")
                throw new System.Exception("trainer1.getName() != \"Red\"");

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

        successful = BagPocketTest() && successful;
        // TODO: GameSaveTest
        successful = PokedexTest() && successful;
        successful = PokemonTest() && successful;
        successful = PRNGTest() && successful;
        successful = TrainerTest() && successful;

        return successful ? 0 : 1;
    }    
}
