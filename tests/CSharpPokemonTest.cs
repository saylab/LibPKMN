/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

public class PokemonTest
{
    static int[] nativeSizes = {0,44,48,100,380,380,500};

    public static bool Gen1Test()
    {
        try
        {
            PKMN.PokemonSPtr pokemon = PKMN.Pokemon.make("Pikachu", "Red", 50, "Tackle",
                                                         "Tail Whip", "Thunder", "Surf");

            byte[] nativePokemon = pokemon.getNative();
            if(nativePokemon.Length != nativeSizes[1])
                throw new System.Exception("nativePokemon.Length != nativeSizes[1]");

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

    public static bool Gen2Test()
    {
        try
        {
            PKMN.PokemonSPtr pokemon = PKMN.Pokemon.make("Pikachu", "Gold", 50, "Tackle",
                                                         "Tail Whip", "Thunder", "Surf");

            byte[] nativePokemon = pokemon.getNative();
            if(nativePokemon.Length != nativeSizes[2])
                throw new System.Exception("nativePokemon.Length != nativeSizes[2]");

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

    public static bool Gen3Test()
    {
        try
        {
            PKMN.PokemonSPtr pokemon = PKMN.Pokemon.make("Pikachu", "Emerald", 50, "Tackle",
                                                         "Tail Whip", "Thunder", "Surf");

            byte[] nativePokemon = pokemon.getNative();
            if(nativePokemon.Length != nativeSizes[3])
                throw new System.Exception("nativePokemon.Length != nativeSizes[3]");

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

    public static bool NDSTest()
    {
        try
        {
            PKMN.PokemonSPtr pokemon = PKMN.Pokemon.make("Pikachu", "Platinum", 50, "Tackle",
                                                         "Tail Whip", "Thunder", "Surf");

            byte[] nativePokemon = pokemon.getNative();
            if(nativePokemon.Length != nativeSizes[4])
                throw new System.Exception("nativePokemon.Length != nativeSizes[4]");

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

        successful = Gen1Test() && successful;
        successful = Gen2Test() && successful;
        successful = Gen3Test() && successful;
        successful = NDSTest() && successful;

        return successful ? 0 : 1;
    }    
}
