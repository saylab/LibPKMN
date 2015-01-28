/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using System.Collections.Generic;

public class DictTest
{
    /*
     * pkmn::dict<pkmn::pkstring, int>
     */
    public static bool StringIntDictTest()
    {
        try
        {
            PKMN.StringIntDict dict = new PKMN.StringIntDict();
            dict.Add("abc", -1);
            dict.Add("def", 5);
            dict.Add("ghi", 57);

            // Test adding and swapping
            if(dict["abc"] != -1)
                throw new System.Exception("dict[\"abc\"] != -1");
            if(dict["def"] != 5)
                throw new System.Exception("dict[\"def\"] != 5");
            if(dict["ghi"] != 57)
                throw new System.Exception("dict[\"ghi\"] != 57");
            if(dict.at("abc", 5) != -1)
                throw new System.Exception("dict.at(\"abc\", 5) != 57");
            if(dict.at("ade", -10) != -10)
                throw new System.Exception("dict.at(\"ade\", -10) != -10");
            dict.swap("abc", "ghi");
            if(dict["abc"] != 57)
                throw new System.Exception("dict[\"abc\"] != 57");
            if(dict["ghi"] != -1)
                throw new System.Exception("dict[\"ghi\"] != -1");

            // Make sure key vector is in correct order
            PKMN.StringVector keys = dict.keys();
            if(keys.Count != 3)
                throw new System.Exception("keys.Count != 3");
            if(keys[0] != "abc")
                throw new System.Exception("keys[0] != \"abc\"");
            if(keys[1] != "def")
                throw new System.Exception("keys[1] != \"def\"");
            if(keys[2] != "ghi")
                throw new System.Exception("keys[2] != \"ghi\"");

            // Make sure val vector is in correct order
            PKMN.IntVector vals = dict.vals();
            if(vals.Count != 3)
                throw new System.Exception("vals.Count != 3");
            if(vals[0] != 57)
                throw new System.Exception("vals[0] != 57");
            if(vals[1] != 5)
                throw new System.Exception("vals[1] != 5");
            if(vals[2] != -1)
                throw new System.Exception("vals[2] != -1");

            // Make sure clearing works
            dict.Clear();
            if(dict.size() != 0)
                throw new System.Exception("dict.size() != 0");

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

    public static bool StringStringDictTest()
    {
        try
        {
            PKMN.StringStringDict dict = new PKMN.StringStringDict();
            dict.Add("abc", "rst");
            dict.Add("def", "uvw");
            dict.Add("ghi", "xyz");

            // Test adding and swapping
            if(dict["abc"] != "rst")
                throw new System.Exception("dict[\"abc\"] != \"rst\"");
            if(dict["def"] != "uvw")
                throw new System.Exception("dict[\"def\"] != \"uvw\"");
            if(dict["ghi"] != "xyz")
                throw new System.Exception("dict[\"ghi\"] != \"xyz\"");
            if(dict.at("abc", "jkl") != "rst")
                throw new System.Exception("dict.at(\"abc\", \"jkl\") != \"rst\"");
            if(dict.at("ade", "jkl") != "jkl")
                throw new System.Exception("dict.at(\"ade\", \"jkl\") != \"jkl\"");
            dict.swap("abc", "ghi");
            if(dict["abc"] != "xyz")
                throw new System.Exception("dict[\"abc\"] != \"xyz\"");
            if(dict["ghi"] != "rst")
                throw new System.Exception("dict[\"ghi\"] != \"rst\"");

            // Make sure key vector is in correct order
            PKMN.StringVector keys = dict.keys();
            if(keys.Count != 3)
                throw new System.Exception("keys.Count != 3");
            if(keys[0] != "abc")
                throw new System.Exception("keys[0] != \"abc\"");
            if(keys[1] != "def")
                throw new System.Exception("keys[1] != \"def\"");
            if(keys[2] != "ghi")
                throw new System.Exception("keys[2] != \"ghi\"");

            // Make sure key vector is in correct order
            PKMN.StringVector vals = dict.vals();
            if(vals.Count != 3)
                throw new System.Exception("vals.Count != 3");
            if(vals[0] != "xyz")
                throw new System.Exception("vals[0] != \"xyz\"");
            if(vals[1] != "uvw")
                throw new System.Exception("vals[1] != \"uvw\"");
            if(vals[2] != "rst")
                throw new System.Exception("vals[2] != \"rst\"");

            // Make sure clearing works
            dict.Clear();
            if(dict.size() != 0)
                throw new System.Exception("dict.size() != 0");

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

    public static bool StringUShortDictTest()
    {
        try
        {
            PKMN.StringUShortDict dict = new PKMN.StringUShortDict();
            dict.Add("abc", 25);
            dict.Add("def", 5);
            dict.Add("ghi", 57);

            // Test adding and swapping
            if(dict["abc"] != 25)
                throw new System.Exception("dict[\"abc\"] != 25");
            if(dict["def"] != 5)
                throw new System.Exception("dict[\"def\"] != 5");
            if(dict["ghi"] != 57)
                throw new System.Exception("dict[\"ghi\"] != 57");
            if(dict.at("abc", 5) != 25)
                throw new System.Exception("dict.At(\"abc\", 5) != 25");
            if(dict.at("ade", 10) != 10)
                throw new System.Exception("dict.At(\"ade\", 10) != 10");
            dict.swap("abc", "ghi");
            if(dict["abc"] != 57)
                throw new System.Exception("dict[\"abc\"] != 57");
            if(dict["ghi"] != 25)
                throw new System.Exception("dict[\"ghi\"] != 25");

            // Make sure key vector is in correct order
            PKMN.StringVector keys = dict.keys();
            if(keys.Count != 3)
                throw new System.Exception("keys.Count != 3");
            if(keys[0] != "abc")
                throw new System.Exception("keys[0] != \"abc\"");
            if(keys[1] != "def")
                throw new System.Exception("keys[1] != \"def\"");
            if(keys[2] != "ghi")
                throw new System.Exception("keys[2] != \"ghi\"");

            // Make sure key vector is in correct order
            PKMN.UShortVector vals = dict.vals();
            if(vals.Count != 3)
                throw new System.Exception("vals.Count != 3");
            if(vals[0] != 57)
                throw new System.Exception("vals[0] != 57");
            if(vals[1] != 5)
                throw new System.Exception("vals[1] != 5");
            if(vals[2] != 25)
                throw new System.Exception("vals[2] != 25");

            // Make sure clearing works
            dict.Clear();
            if(dict.size() != 0)
                throw new System.Exception("dict.size() != 0");

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

        successful = StringIntDictTest();
        successful = StringStringDictTest() && successful;
        successful = StringUShortDictTest() && successful;

        return successful ? 0 : 1;
    }    
}
