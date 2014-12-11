/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

import org.nc.PKMN.*;
import java.lang.RuntimeException;

public class JavaSPtrTest
{
    /*
     * Make sure the bag shared_ptr copies correctly.
     */
    public static void BagPocketTest()
    {
        try
        {
            BagSPtr bag1 = Bag.make("Gold");
            BagSPtr bag2 = bag1;
            if(bag1 != bag2)
                throw new RuntimeException("bag1 != bag2");

            /*
             * SWIG + C# reports pockets from bags from the same pointer having
             * different values, so check equality through item additions.
             */
            PocketDict bag1Pockets = bag1.getPockets();
            PocketDict bag2Pockets = bag2.getPockets();

            bag1Pockets.at("Items").addItem("Potion", 10);
            bag1Pockets.at("Balls").addItem("Master Ball", 10);
            bag1Pockets.at("KeyItems").addItem("Bicycle", 1);
            bag1Pockets.at("TM/HM").addItem("TM01", 10);
            if(bag2Pockets.at("Items").getItemAmount("Potion") != 10)
                throw new RuntimeException("bag2Pockets.at(\"Items\").getItemAmount(\"Potion\") != 10");
            if(bag2Pockets.at("Balls").getItemAmount("Master Ball") != 10)
                throw new RuntimeException("bag2Pockets.at(\"Balls\").getItemAmount(\"Master Ball\") != 10");
            if(bag2Pockets.at("KeyItems").getItemAmount("Bicycle") != 1)
                throw new RuntimeException("bag2Pockets.at(\"KeyItems\").getItemAmount(\"Bicycle\") != 1");
            if(bag2Pockets.at("TM/HM").getItemAmount("TM01") != 10)
                throw new RuntimeException("bag2Pockets.at(\"TM/HM\").getItemAmount(\"TM01\") != 10");
        }
        catch(RuntimeException ex)
        {
            System.out.println("Caught exception:");
            System.out.println("   " + ex.getMessage());
            System.out.println(ex.getStackTrace());
            System.exit(1);
        }
    }

    /*
     * Make sure the base_pokemon shared_ptr copies correctly.
     */
    public static void BasePokemonTest()
    {
        try
        {
            BasePokemonSPtr basePokemon1 = BasePokemon.make("Bulbasaur", "Ruby");
            BasePokemonSPtr basePokemon2 = basePokemon1;
            if(basePokemon1 != basePokemon2)
                throw new RuntimeException("basePokemon1 != basePokemon2");
        }
        catch(RuntimeException ex)
        {
            System.out.println("Caught exception:");
            System.out.println("   " + ex.getMessage());
            System.out.println(ex.getStackTrace());
            System.exit(1);
        }
    }

    /*
     * Make sure the item shared_ptr copies correctly.
     */
    public static void ItemTest()
    {
        try
        {
            ItemSPtr item1 = Item.make("Potion", "Diamond");
            ItemSPtr item2 = item1;
            if(item1 != item2)
                throw new RuntimeException("item1 != item2");
        }
        catch(RuntimeException ex)
        {
            System.out.println("Caught exception:");
            System.out.println("   " + ex.getMessage());
            System.out.println(ex.getStackTrace());
            System.exit(1);
        }
    }

    /*
     * Make sure the move shared_ptr copies correctly.
     */
    public static void MoveTest()
    {
        try
        {
            MoveSPtr move1 = Move.make("Tackle", "Diamond");
            MoveSPtr move2 = move1;
            if(move1 != move2)
                throw new RuntimeException("move1 != move2");
        }
        catch(RuntimeException ex)
        {
            System.out.println("Caught exception:");
            System.out.println("   " + ex.getMessage());
            System.out.println(ex.getStackTrace());
            System.exit(1);
        }
    }

    /*
     * Make sure the prng shared_ptr copies correctly.
     */
    public static void PRNGTest()
    {
        try
        {
            PRNGSPtr prng1 = PRNG.make(3);
            PRNGSPtr prng2 = prng1;
            if(prng1 != prng2)
                throw new RuntimeException("prng1 != prng2");
        }
        catch(RuntimeException ex)
        {
            System.out.println("Caught exception:");
            System.out.println("   " + ex.getMessage());
            System.out.println(ex.getStackTrace());
            System.exit(1);
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
            TeamPokemonSPtr deoxys = TeamPokemon.make("Deoxys", "Diamond", 50,
                                                                      "None", "None", "None", "None");
            TeamPokemonSPtr deoxys2 = deoxys;
            if(deoxys != deoxys2)
                throw new RuntimeException("deoxys != deoxys2");

            StringUIntDict deoxysStats1 = deoxys.getStats();

            BasePokemonSPtr deoxysBase1 = deoxys.getBasePokemon(false); // Same as deoxys
            BasePokemonSPtr deoxysBase2 = deoxys.getBasePokemon(true); // Copies from deoxys

            deoxysBase1.setForm("Attack");
            deoxysBase2.setForm("Defense");

            StringUIntDict deoxysStats2 = deoxys.getStats();

            if(deoxysBase1 == deoxysBase2)
                throw new RuntimeException("deoxysBase1 == deoxysBase2");
            if(deoxys.getFormID() != DeoxysForms.ATTACK)
                throw new RuntimeException("deoxys.getFormID() != DeoxysForms.ATTACK");
            if(deoxysBase1.getFormID() != DeoxysForms.ATTACK)
                throw new RuntimeException("deoxysBase1.getFormID() != DeoxysForms.ATTACK");
            if(deoxys.getFormID() != deoxysBase1.getFormID())
                throw new RuntimeException("deoxys.getFormID() != deoxysBase1.getFormID()");
            if(deoxysBase2.getFormID() != DeoxysForms.DEFENSE)
                throw new RuntimeException("deoxysBase2.getFormID() != DeoxysForms.DEFENSE");
            if(deoxysStats1.at("Attack") == deoxysStats2.at("Attack"))
                throw new RuntimeException("deoxysStats1.at(\"Attack\") == deoxysStats2.at(\"Attack\")");
            if(deoxysStats1.at("Defense") == deoxysStats2.at("Defense"))
                throw new RuntimeException("deoxysStats1.at(\"Defense\") == deoxysStats2.at(\"Defense\")");
            if(deoxysStats1.at("Special Attack") == deoxysStats2.at("Special Attack"))
                throw new RuntimeException("deoxysStats1.at(\"Special Attack\") == deoxysStats2.at(\"Special Attack\")");
            if(deoxysStats1.at("Special Defense") == deoxysStats2.at("Special Defense"))
                throw new RuntimeException("deoxysStats1.at(\"Special Defense\") == deoxysStats2.at(\"Special Defense\")");
        }
        catch(RuntimeException ex)
        {
            System.out.println("Caught exception:");
            System.out.println("   " + ex.getMessage());
            System.out.println(ex.getStackTrace());
            System.exit(1);
        }
    }

    /*
     * Make sure the trainer shared_ptr copies correctly.
     */
    public static void TrainerTest()
    {
        try
        {
            TrainerSPtr trainer1 = Trainer.make("Red", "Red", "Gold");
            TrainerSPtr trainer2 = trainer1;
            if(trainer1 != trainer2)
                throw new RuntimeException("trainer1 != trainer2");
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
        BagPocketTest();
        BasePokemonTest();
        ItemTest();
        MoveTest();
        PRNGTest();
        TeamPokemonTest();
        TrainerTest();
    }    
}
