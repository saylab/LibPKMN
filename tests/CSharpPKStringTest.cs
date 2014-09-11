/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using LibPKMN;
using System;

public class PKStringTest
{
    public static int Main(string[] args)
    {
        //Testing printing on command line
        PKString test_text1 = new PKString("Test1");
        System.Console.WriteLine("PKString: " + test_text1);

        //Testing string -> PKString
        string test_string1 = "Test2";
        PKString test_text2 = test_string1; //implicit
        if(test_text2 != "Test2")
        {
            throw new System.Exception("C# string -> PKString implicit conversion failed!");
        }
        System.Console.WriteLine("C# string -> PKString (implicit): " + test_text2);
        PKString test_text3 = (PKString)test_string1; //explicit
        if(test_text3 != "Test2")
        {
            throw new System.Exception("C# string -> PKString explicit conversion failed!");
        }
        System.Console.WriteLine("C# string -> PKString (explicit): " + test_text3);

        //Testing PKString -> string
        PKString test_text4 = new PKString("Test3");
        string test_string2 = test_text4.ToString();
        if(test_string2 != "Test3")
        {
            throw new System.Exception("PKString.ToString() failed!");
        }
        System.Console.WriteLine("PKString.ToString(): " + test_string2);
        string test_string3 = test_text4;
        if(test_string3 != "Test3")
        {
            throw new System.Exception("PKString -> C# string implicit conversion failed!");
        }
        System.Console.WriteLine("PKString -> C# string (implicit): " + test_string3);
        string test_string4 = (string)test_text4;
        if(test_string4 != "Test3")
        {
            throw new System.Exception("PKString -> C# string explicit conversion failed!");
        }
        System.Console.WriteLine("PKString -> C# string (explicit): " + test_string4);

        return 0;
    }    
}
