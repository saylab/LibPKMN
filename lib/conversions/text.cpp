/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <string>

#include "text.hpp"

#define INDEX_OF(arr, val) std::distance(arr, std::find(arr, arr+255, val))

namespace pkmn
{
    namespace conversions
    {
        pkmn::pkstring import_gen1_text(const uint8_t* buffer, uint8_t max_len)
        {
            std::wstring str;
            for(size_t i = 0; i < max_len; i++)
            {
                if(!buffer or buffer[i] == 0x50) break;
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
                if(!buffer or buffer[i] == 0x50) break;
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
                if(!buffer or buffer[i] > 0xF9) break;
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
    }
}
