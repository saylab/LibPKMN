/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

public class CopySPtrTest
{
    /*
     * Make sure the bag shared_ptr copies correctly.
     */
    public static void BagPocketTest()
    {
        try
        {
            PKMN.BagSPtr bag1 = PKMN.Bag.make("Gold");
            PKMN.BagSPtr bag2 = bag1;
            if(bag1 != bag2)
                throw new System.Exception("bag1 != bag2");

            /*
             * SWIG + C# reports pockets from bags from the same pointer having
             * different values, so check equality through item additions.
             */
            PKMN.PocketDict bag1Pockets = bag1.getPockets();
            PKMN.PocketDict bag2Pockets = bag2.getPockets();

            bag1Pockets["Items"].addItem("Potion", 10);
            bag1Pockets["Balls"].addItem("Master Ball", 10);
            bag1Pockets["KeyItems"].addItem("Bicycle", 1);
            bag1Pockets["TM/HM"].addItem("TM01", 10);
            if(bag2Pockets["Items"].getItemAmount("Potion") != 10)
                throw new System.Exception("bag2Pockets[\"Items\"].getItemAmount(\"Potion\") != 10");
            if(bag2Pockets["Balls"].getItemAmount("Master Ball") != 10)
                throw new System.Exception("bag2Pockets[\"Balls\"].getItemAmount(\"Master Ball\") != 10");
            if(bag2Pockets["KeyItems"].getItemAmount("Bicycle") != 1)
                throw new System.Exception("bag2Pockets[\"KeyItems\"].getItemAmount(\"Bicycle\") != 1");
            if(bag2Pockets["TM/HM"].getItemAmount("TM01") != 10)
                throw new System.Exception("bag2Pockets[\"TM/HM\"].getItemAmount(\"TM01\") != 10");
        }
        catch(System.Exception ex)
        {
            System.Console.WriteLine("Caught exception:");
            System.Console.WriteLine("   " + ex.Message);
            System.Console.WriteLine(ex.StackTrace);
            System.Environment.Exit(1);
        }
    }

    /*
     * Make sure the base_pokemon shared_ptr copies correctly.
     */
    public static void BasePokemonTest()
    {
        try
        {
            PKMN.BasePokemonSPtr basePokemon1 = PKMN.BasePokemon.make("Bulbasaur", "Ruby");
            PKMN.BasePokemonSPtr basePokemon2 = basePokemon1;
            if(basePokemon1 != basePokemon2)
                throw new System.Exception("basePokemon1 != basePokemon2");
        }
        catch(System.Exception ex)
        {
            System.Console.WriteLine("Caught exception:");
            System.Console.WriteLine("   " + ex.Message);
            System.Console.WriteLine(ex.StackTrace);
            System.Environment.Exit(1);
        }
    }

    /*
     * Make sure the item shared_ptr copies correctly.
     */
    public static void ItemTest()
    {
        try
        {
            PKMN.ItemSPtr item1 = PKMN.Item.make("Potion", "Diamond");
            PKMN.ItemSPtr item2 = item1;
            if(item1 != item2)
                throw new System.Exception("item1 != item2");
        }
        catch(System.Exception ex)
        {
            System.Console.WriteLine("Caught exception:");
            System.Console.WriteLine("   " + ex.Message);
            System.Console.WriteLine(ex.StackTrace);
            System.Environment.Exit(1);
        }
    }

    /*
     * Make sure the move shared_ptr copies correctly.
     */
    public static void MoveTest()
    {
        try
        {
            PKMN.MoveSPtr move1 = PKMN.Move.make("Tackle", "Diamond");
            PKMN.MoveSPtr move2 = move1;
            if(move1 != move2)
                throw new System.Exception("move1 != move2");
        }
        catch(System.Exception ex)
        {
            System.Console.WriteLine("Caught exception:");
            System.Console.WriteLine("   " + ex.Message);
            System.Console.WriteLine(ex.StackTrace);
            System.Environment.Exit(1);
        }
    }

    /*
     * Make sure the prng shared_ptr copies correctly.
     */
    public static void PRNGTest()
    {
        try
        {
            PKMN.PRNGSPtr prng1 = PKMN.PRNG.make(3);
            PKMN.PRNGSPtr prng2 = prng1;
            if(prng1 != prng2)
                throw new System.Exception("prng1 != prng2");
        }
        catch(System.Exception ex)
        {
            System.Console.WriteLine("Caught exception:");
            System.Console.WriteLine("   " + ex.Message);
            System.Console.WriteLine(ex.StackTrace);
            System.Environment.Exit(1);
        }
    }

    /*
     * Make sure the team_pokemon shared_ptr copies correctly.
     * Make sure team_pokemon_modernimpl's signals/slots work correctly.
     */
    public static void TeamPokemonTest()
    {
        try
        {
            PKMN.TeamPokemonSPtr deoxys = PKMN.TeamPokemon.make("Deoxys", "Diamond", 50,
                                                                      "None", "None", "None", "None");
            PKMN.TeamPokemonSPtr deoxys2 = deoxys;
            if(deoxys != deoxys2)
                throw new System.Exception("deoxys != deoxys2");

            PKMN.StringUIntDict deoxysStats1 = deoxys.getStats();

            PKMN.BasePokemonSPtr deoxysBase1 = deoxys.getBasePokemon(false); // Same as deoxys
            PKMN.BasePokemonSPtr deoxysBase2 = deoxys.getBasePokemon(true); // Copies from deoxys

            deoxysBase1.setForm("Attack");
            deoxysBase2.setForm("Defense");

            PKMN.StringUIntDict deoxysStats2 = deoxys.getStats();

            if(deoxysBase1 == deoxysBase2)
                throw new System.Exception("deoxysBase1 == deoxysBase2");
            if(deoxys.getFormID() != PKMN.PokemonForms.Deoxys.ATTACK)
                throw new System.Exception("deoxys.getFormID() != PKMN.PokemonForms.Deoxys.ATTACK");
            if(deoxysBase1.getFormID() != PKMN.PokemonForms.Deoxys.ATTACK)
                throw new System.Exception("deoxysBase1.getFormID() != PKMN.PokemonForms.Deoxys.ATTACK");
            if(deoxys.getFormID() != deoxysBase1.getFormID())
                throw new System.Exception("deoxys.getFormID() != deoxysBase1.getFormID()");
            if(deoxysBase2.getFormID() != PKMN.PokemonForms.Deoxys.DEFENSE)
                throw new System.Exception("deoxysBase2.getFormID() != PKMN.PokemonForms.Deoxys.DEFENSE");
            if(deoxysStats1["Attack"] == deoxysStats2["Attack"])
                throw new System.Exception("deoxysStats1[\"Attack\"] == deoxysStats2[\"Attack\"]");
            if(deoxysStats1["Defense"] == deoxysStats2["Defense"])
                throw new System.Exception("deoxysStats1[\"Defense\"] == deoxysStats2[\"Defense\"]");
            if(deoxysStats1["Special Attack"] == deoxysStats2["Special Attack"])
                throw new System.Exception("deoxysStats1[\"Special Attack\"] == deoxysStats2[\"Special Attack\"]");
            if(deoxysStats1["Special Defense"] == deoxysStats2["Special Defense"])
                throw new System.Exception("deoxysStats1[\"Special Defense\"] == deoxysStats2[\"Special Defense\"]");
        }
        catch(System.Exception ex)
        {
            System.Console.WriteLine("Caught exception:");
            System.Console.WriteLine("   " + ex.Message);
            System.Console.WriteLine(ex.StackTrace);
            System.Environment.Exit(1);
        }
    }

    /*
     * Make sure the trainer shared_ptr copies correctly.
     */
    public static void TrainerTest()
    {
        try
        {
            PKMN.TrainerSPtr trainer1 = PKMN.Trainer.make("Red", "Red", "Gold");
            PKMN.TrainerSPtr trainer2 = trainer1;
            if(trainer1 != trainer2)
                throw new System.Exception("trainer1 != trainer2");
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
        BagPocketTest();
        BasePokemonTest();
        ItemTest();
        MoveTest();
        PRNGTest();
        TeamPokemonTest();
        TrainerTest();

        return 0;
    }    
}
