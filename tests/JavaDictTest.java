/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

import nc.PKMN.*;
import java.lang.RuntimeException;
import java.util.Arrays;

public class JavaDictTest
{
    public static void StringIntDictTest()
    {
        try
        {
            /*
             * pkmn::dict<pkmn::pkstring, int>
             */
            StringIntDict dict = new StringIntDict();
            dict.put("abc", -1);
            dict.put("def", 5);
            dict.put("ghi", 57);

            if(dict.get("abc") != -1)
                throw new RuntimeException("dict.get(\"abc\") != -1");
            if(dict.get("def") != 5)
                throw new RuntimeException("dict.get(\"def\") != 5");
            if(dict.get("ghi") != 57)
                throw new RuntimeException("dict.get(\"ghi\") != 57");
            if(dict.get("abc",5) != -1)
                throw new RuntimeException("dict.get(\"abc\",5) != -1");
            if(dict.get("ade",-10) != -10)
                throw new RuntimeException("dict.get(\"abe\",-10) != -10");
            dict.swap("abc","ghi");
            if(dict.get("abc") != 57)
                throw new RuntimeException("dict.get(\"abc\") != 57");
            if(dict.get("ghi") != -1)
                throw new RuntimeException("dict.get(\"ghi\") != -1");
        }
        catch(RuntimeException ex)
        {
            System.out.println("Caught exception:");
            System.out.println("   " + ex.getMessage());
            System.out.println("   " + Arrays.toString(ex.getStackTrace()));
            System.exit(1);
        }
    }

    public static void StringStringDictTest()
    {
        try
        {
            /*
             * pkmn::dict<pkmn::pkstring, pkmn::pkstring>
             */
            StringStringDict dict = new StringStringDict();
            dict.put("abc", "def");
            dict.put("ghi", "jkl");
            dict.put("mno", "pqr");

            if(!dict.get("abc").equals("def"))
                throw new RuntimeException("!dict.get(\"abc\").equals(\"def\")");
            if(!dict.get("ghi").equals("jkl"))
                throw new RuntimeException("!dict.get(\"ghi\").equals(\"jkl\")");
            if(!dict.get("mno").equals("pqr"))
                throw new RuntimeException("!dict.get(\"mno\").equals(\"pqr\")");
            if(!dict.get("abc","stu").equals("def"))
                throw new RuntimeException("!dict.get(\"abc\",\"stu\").equals(\"def\")");
            if(!dict.get("stu","vwx").equals("vwx"))
                throw new RuntimeException("!dict.get(\"stu\",\"vwx\").equals(\"vwx\")");
            dict.swap("abc","mno");
            if(!dict.get("abc").equals("pqr"))
                throw new RuntimeException("!dict.get(\"abc\").equals(\"pqr\")");
            if(!dict.get("mno").equals("def"))
                throw new RuntimeException("!dict.get(\"mno\").equals(\"def\")");
        }
        catch(RuntimeException ex)
        {
            System.out.println("Caught exception:");
            System.out.println("   " + ex.getMessage());
            System.out.println("   " + Arrays.toString(ex.getStackTrace()));
            System.exit(1);
        }
    }

    public static void StringLongDictTest()
    {
        try
        {
            /*
             * pkmn::dict<pkmn::pkstring, int>
             */
            StringLongDict dict = new StringLongDict();
            dict.put("abc", 10);
            dict.put("def", 5);
            dict.put("ghi", 57);

            if(dict.get("abc") != 10)
                throw new RuntimeException("dict.get(\"abc\") != 10");
            if(dict.get("def") != 5)
                throw new RuntimeException("dict.get(\"def\") != 5");
            if(dict.get("ghi") != 57)
                throw new RuntimeException("dict.get(\"ghi\") != 57");
            if(dict.get("abc",5) != 10)
                throw new RuntimeException("dict.get(\"abc\",5) != 10");
            if(dict.get("ade",100) != 100)
                throw new RuntimeException("dict.get(\"abe\",100) != 100");
            dict.swap("abc","ghi");
            if(dict.get("abc") != 57)
                throw new RuntimeException("dict.get(\"abc\") != 57");
            if(dict.get("ghi") != 10)
                throw new RuntimeException("dict.get(\"ghi\") != 10");
        }
        catch(RuntimeException ex)
        {
            System.out.println("Caught exception:");
            System.out.println("   " + ex.getMessage());
            System.out.println("   " + Arrays.toString(ex.getStackTrace()));
            System.exit(1);
        }
    }

    public static void main(String[] args)
    {
        StringIntDictTest();
        StringStringDictTest();
        StringLongDictTest();
    }
}
