/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <cstring>
#include <string>

#include <pkmn/conversions/text.hpp>

#include "PokeText.h"

#define INDEX_OF(arr, val) std::distance(arr, std::find(arr, arr+255, val))

namespace pkmn
{
    namespace conversions
    {

        /*
         * Character map for Generation I
         *
         * Source: http://bulbapedia.bulbagarden.net/wiki/Save_data_structure_in_Generation_I#Text_data
         *
         * There are no Unicode values for the "PK" and "MN" in-game characters, so we will use the
         * "<" and ">" characters, as they are not used in-game.
         */
        const wchar_t gen1_char_map[] =
        {
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',0x20,
            0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50,
            0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x28,0x29,0x3A,0x3B,0x5B,0x5D,
            0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,0x70,
            0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,'\0','\0','\0','\0','\0','\0',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
            '\0',0x3C,0x3E,0x2D,'\0','\0',0x3F,0x21,0x2E,'\0','\0','\0','\0','\0','\0',0x2642,
            '\0',0xD7,'\0',0x2F,0x2C,0x2640,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39
        };

        /*
         * Character map for Generation II
         *
         * Source: http://bulbapedia.bulbagarden.net/wiki/Save_data_structure_in_Generation_II#Text_data
         *
         * There are no Unicode values for the "PK" and "MN" in-game characters, so we will use the
         * "<" and ">" characters, as they are not used in-game.
         */
        const wchar_t gen2_char_map[] =
        {
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',0x20,
            0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50,
            0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x28,0x29,0x3A,0x3B,0x5B,0x5D,
            0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,0x70,
            0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,'\0','\0','\0','\0','\0','\0',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
            '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
            '\0',0x3C,0x3E,0x2D,'\0','\0',0x3F,0x21,0x2E,'\0','\0','\0','\0','\0','\0','\0',
            '\0',0xD7,'\0',0x2F,0x2C,'\0',0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39
        };

        /*
         * Character map for Generation III (English)
         *
         * Source: http://bulbapedia.bulbagarden.net/wiki/Character_encoding_in_Generation_III 
         *
         * There are no Unicode values for certain characters, so we will use substitutes, as listed below:
         *
         *  * "$" -> Pokemon dollar
         *  * "<" -> "PK"
         *  * ">" -> "MN"
         *  * "*" -> "PO"
         *  * "~" -> "KE"
         */
        const wchar_t gen3_char_map[] =
        {
            0x20,0xC0,0xC1,0xC2,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0x3053,0xCD,0xCE,0xD2,0xD3,0xD4,
            0x152,0xD9,0xDA,0xDB,0xD1,0x1E9E,0xE0,0xE1,0x306D,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0x307E,
            0xEE,0xEF,0xF2,0xF3,0xF4,0x152,0xF9,0xFA,0xFB,0xF1,0xBA,0xAA,0x3F,0x26,0x2B,0x3042,
            0x3043,0x3045,0x3047,0x3049,0x2F,0x3D,0x3087,0x304C,0x304E,0x3050,0x3052,0x3054,0x3056,0x3058,0x305A,0x305C,
            0x305E,0x3060,0x3062,0x3065,0x3067,0x3069,0x3070,0x3073,0x3076,0x3079,0x307C,0x3071,0x3074,0x3077,0x307A,0x307D,
            0x3063,0xBF,0xA1,0x3C,0x3E,0x2A,0x7E,0x3F,0x3F,0x3F,0xCD,0x25,0x28,0x29,0x30BB,0x30BD,
            0x30BF,0x30C1,0x30C4,0x30C6,0x30C8,0x30CA,0x30CB,0x30CC,0xC2,0x30CE,0x30CF,0x30D2,0x30D5,0x30D8,0x30DB,0xCD,
            0x30DF,0x30E0,0x30E1,0x30E2,0x30E4,0x30E6,0x30E8,0x30E9,0x30EA,0x2B06,0x2B07,0x2B05,0x27A1,0x30F2,0x30F3,0x30A1,
            0x30A3,0x30A5,0x30A7,0x30A9,0x30E3,0x30E5,0x30E7,0x30AC,0x30AE,0x30B0,0x30B2,0x30B4,0x30B6,0x30B8,0x30BA,0x30BC,
            0x30BE,0x30C0,0x30C2,0x30C5,0x30C7,0x30C9,0x30D0,0x30D3,0x30D6,0x30D9,0x30DC,0x30D1,0x30D4,0x30D7,0x30DA,0x30DD,
            0x30C3,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x21,0x3F,0x3E,0x3D,0x30FB,
            0x2026,0x201C,0x201D,0x2018,0x2019,0x2642,0x2640,0x24,0x2C,0xD7,0x2F,0x41,0x42,0x43,0x44,0x45,
            0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50,0x51,0x52,0x53,0x54,0x55,
            0x56,0x57,0x58,0x59,0x5A,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,
            0x6C,0x6D,0x6E,0x6F,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x78,0x7A,0x25B6,
            0x3A,0xC4,0xD6,0xDC,0xE4,0xF6,0xF6,0x2B06,0x2B07,0x2B05,'\0','\0','\0','\0','\n','\0'
        };

        pkmn::pkstring import_gen1_text(const uint8_t* buffer, uint8_t max_len)
        {
            std::wstring str;
            for(size_t i = 0; i < max_len; i++)
            {
                if(!buffer[i] or buffer[i] == 0x50) break;
                else str += gen1_char_map[buffer[i]];
            }
            return pkmn::pkstring(str);
        }

        void export_gen1_text(const pkmn::pkstring &text, uint8_t* buffer, uint8_t max_len)
        {
            std::wstring str = text.std_wstring();
            for(size_t i = 0; i < max_len; i++)
            {
                if(i >= str.size()) buffer[i] = 0x50;
                else if(str[i] == 0x20) buffer[i] = 0x7F;
                else buffer[i] = INDEX_OF(gen1_char_map, str[i]);
            }
        }

        pkmn::pkstring import_gen2_text(const uint8_t* buffer, uint8_t max_len)
        {
            std::wstring str;
            for(size_t i = 0; i < max_len; i++)
            {
                if(!buffer[i] or buffer[i] == 0x50) break;
                else str += gen2_char_map[buffer[i]];
            }
            return pkmn::pkstring(str);
        }

        void export_gen2_text(const pkmn::pkstring &text, uint8_t* buffer, uint8_t max_len)
        {
            std::wstring str = text.std_wstring();
            for(size_t i = 0; i < max_len; i++)
            {
                if(i >= str.size()) buffer[i] = 0x50;
                else if(str[i] == 0x20) buffer[i] = 0x7F;
                else buffer[i] = INDEX_OF(gen2_char_map, str[i]);
            }
        }

        pkmn::pkstring import_gen3_text(const uint8_t* buffer, uint8_t max_len)
        {
            std::wstring str;
            for(size_t i = 0; i < max_len; i++)
            {
                if(!buffer[i] or buffer[i] > 0xF9) break;
                else str += gen3_char_map[buffer[i]];
            }
            return pkmn::pkstring(str);
        }

        void export_gen3_text(const pkmn::pkstring &text, uint8_t* buffer, uint8_t max_len)
        {
            std::wstring str = text.std_wstring();
            for(size_t i = 0; i < max_len; i++)
            {
                if(i >= str.size()) buffer[i] = 0xFF;
                else buffer[i] = INDEX_OF(gen3_char_map, str[i]);
            }
        }

        pkmn::pkstring import_gen4_text(const uint16_t* buffer, uint8_t max_len)
        {
            std::basic_string<uint16_t> str;
            for(size_t i = 0; i < max_len; i++)
            {
                if(!buffer[i] or buffer[i] == 0xFFFF) break;
                else str += buffer[i];
            }

            PokeLib::poketext pokelib_text = PokeLib::PokeText::toPoketext(str.c_str(), str.size());
            return pkmn::pkstring(PokeLib::PokeText::convertFrom(pokelib_text));
        }

        void export_gen4_text(const pkmn::pkstring &text, uint16_t* buffer, uint8_t max_len)
        {
            PokeLib::poketext pokelib_text = PokeLib::PokeText::convertTo(text);
            for(size_t i = 0; i < max_len; i++)
            {
                buffer[i] = (i < text.length()) ? pokelib_text[i] : 0xFFFF;
            }
        }

        pkmn::pkstring import_modern_text(const uint16_t* buffer, uint8_t max_len)
        {
            std::basic_string<uint16_t> str;
            for(size_t i = 0; i < max_len; i++)
            {
                if(!buffer[i] or buffer[i] == 0xFFFF) break;
                else str += buffer[i];
            }
            return pkmn::pkstring(str);
        }

        void export_modern_text(const pkmn::pkstring &text, uint16_t* buffer, uint8_t max_len)
        {
            std::basic_string<uint16_t> str = (const uint16_t*)pkmn::pkstring(text);
            for(size_t i = 0; i < max_len; i++)
            {
                buffer[i] = (i < text.length()) ? str[i] : 0xFFFF;
            }
        }
    }
}
