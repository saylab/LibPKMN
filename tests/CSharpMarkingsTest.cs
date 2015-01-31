/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

public class DictTest
{
    public static bool ToMarkingsTest()
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

    public static bool FromMarkingsTest()
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

        successful = ToMarkingsTest();
        successful = FromMarkingsTest() && successful;

        return successful ? 0 : 1;
    }    
}
