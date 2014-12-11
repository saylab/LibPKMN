/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

public class DictTest
{
    public static void ToMarkingsTest()
    {
        try
        {
            PKMN.Markings mark = new PKMN.Markings();
            mark.circle = true;
            mark.triangle = false;
            mark.square = true;
            mark.heart = false;
            mark.star = true;
            mark.diamond = false;

            byte markInt = mark;
            if(markInt != 21)
                throw new System.Exception("markInt (" + markInt + ") != 0b010101");
        }
        catch(System.Exception ex)
        {
            System.Console.WriteLine("Caught exception:");
            System.Console.WriteLine("   " + ex.Message);
            System.Console.WriteLine(ex.StackTrace);
            System.Environment.Exit(1);
        }
    }

    public static void FromMarkingsTest()
    {
        try
        {
            byte markInt = 21;
            PKMN.Markings mark = markInt;

            if(!mark.circle)
                throw new System.Exception("mark.circle != true");
            if(mark.triangle)
                throw new System.Exception("mark.triangle != false");
            if(!mark.square)
                throw new System.Exception("mark.square != true");
            if(mark.heart)
                throw new System.Exception("mark.heart != false");
            if(!mark.star)
                throw new System.Exception("mark.star != true");
            if(mark.diamond)
                throw new System.Exception("mark.diamond != false");
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
        ToMarkingsTest();
        FromMarkingsTest();

        return 0;
    }    
}
