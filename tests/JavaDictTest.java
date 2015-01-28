/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

import nc.PKMN.*;
import java.lang.RuntimeException;
import java.util.Arrays;

public class JavaDictTest
{
    /*
     * pkmn::dict<pkmn::pkstring, int>
     */
    public static boolean StringIntDictTest()
    {
        try
        {
            StringIntDict dict = new StringIntDict();
            dict.put("abc", -1);
            dict.put("def", 5);
            dict.put("ghi", 57);

            // Test adding and swapping
            if(dict.get("abc") != -1)
                throw new RuntimeException("dict.get(\"abc\") != -1");
            if(dict.get("def") != 5)
                throw new RuntimeException("dict.get(\"def\") != 5");
            if(dict.get("ghi") != 57)
                throw new RuntimeException("dict.get(\"ghi\") != 57");
            if(dict.get("abc", 5) != -1)
                throw new RuntimeException("dict.get(\"abc\", 5) != -1");
            if(dict.get("ade", -10) != -10)
                throw new RuntimeException("dict.get(\"abe\", -10) != -10");
            dict.swap("abc", "ghi");
            if(dict.get("abc") != 57)
                throw new RuntimeException("dict.get(\"abc\") != 57");
            if(dict.get("ghi") != -1)
                throw new RuntimeException("dict.get(\"ghi\") != -1");

            // Make sure key vector is in correct order
            StringVector keys = dict.KeySet();
            if(keys.size() != 3)
                throw new RuntimeException("keys.size() != 3");
            if(!keys.get(0).equals("abc"))
                throw new RuntimeException("keys.get(0) != \"abc\"");
            if(!keys.get(1).equals("def"))
                throw new RuntimeException("keys.get(0) != \"def\"");
            if(!keys.get(2).equals("ghi"))
                throw new RuntimeException("keys.get(0) != \"ghi\"");

            // Make sure val vector is in correct order
            IntVector vals = dict.values();
            if(vals.size() != 3)
                throw new RuntimeException("vals.size() != 3");
            if(vals.get(0) != 57)
                throw new RuntimeException("vals.get(0) != 57");
            if(vals.get(1) != 5)
                throw new RuntimeException("vals.get(1) != 5");
            if(vals.get(2) != -1)
                throw new RuntimeException("vals.get(2) != -1");

            // Make sure clearing works
            dict.clear();
            if(dict.size() != 0)
                throw new RuntimeException("dict.size() != 0");

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
     * pkmn::dict<pkmn::pkstring, pkmn::pkstring>
     */
    public static boolean StringStringDictTest()
    {
        try
        {
            StringStringDict dict = new StringStringDict();
            dict.put("abc", "rst");
            dict.put("def", "uvw");
            dict.put("ghi", "xyz");

            // Test adding and swapping
            if(!dict.get("abc").equals("rst"))
                throw new RuntimeException("!dict.get(\"abc\").equals(\"rst\")");
            if(!dict.get("def").equals("uvw"))
                throw new RuntimeException("!dict.get(\"def\").equals(\"uvw\")");
            if(!dict.get("ghi").equals("xyz"))
                throw new RuntimeException("!dict.get(\"ghi\").equals(\"xyz\")");
            if(!dict.get("abc", "jkl").equals("rst"))
                throw new RuntimeException("!dict.get(\"abc\", \"jkl\").equals(\"rst\")");
            if(!dict.get("ade", "jkl").equals("jkl"))
                throw new RuntimeException("!dict.get(\"ade\", \"jkl\").equals(\"jkl\")");
            dict.swap("abc", "ghi");
            if(!dict.get("abc").equals("xyz"))
                throw new RuntimeException("!dict.get(\"abc\").equals(\"xyz\")");
            if(!dict.get("ghi").equals("rst"))
                throw new RuntimeException("!dict.get(\"ghi\").equals(\"rst\")");

            // Make sure key vector is in correct order
            StringVector keys = dict.KeySet();
            if(keys.size() != 3)
                throw new RuntimeException("keys.size() != 3");
            if(!keys.get(0).equals("abc"))
                throw new RuntimeException("!keys.get().equals(\"abc\")");
            if(!keys.get(1).equals("def"))
                throw new RuntimeException("!keys.get().equals(\"def\")");
            if(!keys.get(2).equals("ghi"))
                throw new RuntimeException("!keys.get().equals(\"ghi\")");

            // Make sure key vector is in correct order
            StringVector vals = dict.values();
            if(vals.size() != 3)
                throw new RuntimeException("vals.Count != 3");
            if(!vals.get(0).equals("xyz"))
                throw new RuntimeException("!vals.get().equals(\"xyz\")");
            if(!vals.get(1).equals("uvw"))
                throw new RuntimeException("!vals.get().equals(\"uvw\")");
            if(!vals.get(2).equals("rst"))
                throw new RuntimeException("!vals.get().equals(\"rst\"");

            // Make sure clearing works
            dict.clear();
            if(dict.size() != 0)
                throw new RuntimeException("dict.size() != 0");

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
     * pkmn::dict<pkmn::pkstring, uint16_t>
     */
    public static boolean StringUShortDictTest()
    {
        try
        {
            StringUShortDict dict = new StringUShortDict();
            dict.put("abc", 25);
            dict.put("def", 5);
            dict.put("ghi", 57);

            // Test adding and swapping
            if(dict.get("abc") != 25)
                throw new RuntimeException("dict.get(\"abc\") != 25");
            if(dict.get("def") != 5)
                throw new RuntimeException("dict.get(\"def\") != 5");
            if(dict.get("ghi") != 57)
                throw new RuntimeException("dict.get(\"ghi\") != 57");
            if(dict.get("abc", 5) != 25)
                throw new RuntimeException("dict.get(\"abc\", 5) != 25");
            if(dict.get("ade", 250) != 250)
                throw new RuntimeException("dict.get(\"abe\", 250) != 250");
            dict.swap("abc", "ghi");
            if(dict.get("abc") != 57)
                throw new RuntimeException("dict.get(\"abc\") != 57");
            if(dict.get("ghi") != 25)
                throw new RuntimeException("dict.get(\"ghi\") != 25");

            // Make sure key vector is in correct order
            StringVector keys = dict.KeySet();
            if(keys.size() != 3)
                throw new RuntimeException("keys.size() != 3");
            if(!keys.get(0).equals("abc"))
                throw new RuntimeException("keys.get(0) != \"abc\"");
            if(!keys.get(1).equals("def"))
                throw new RuntimeException("keys.get(0) != \"def\"");
            if(!keys.get(2).equals("ghi"))
                throw new RuntimeException("keys.get(0) != \"ghi\"");

            // Make sure val vector is in correct order
            UShortVector vals = dict.values();
            if(vals.size() != 3)
                throw new RuntimeException("vals.size() != 3");
            if(vals.get(0) != 57)
                throw new RuntimeException("vals.get(0) != 57");
            if(vals.get(1) != 5)
                throw new RuntimeException("vals.get(1) != 5");
            if(vals.get(2) != 25)
                throw new RuntimeException("vals.get(2) != 25");

            // Make sure clearing works
            dict.clear();
            if(dict.size() != 0)
                throw new RuntimeException("dict.size() != 0");

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

        successful = StringIntDictTest();
        successful = StringStringDictTest() && successful;
        successful = StringUShortDictTest() && successful;

        System.exit(successful ? 0 : 1);
    }
}
