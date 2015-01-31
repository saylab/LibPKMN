/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_CONVERSIONS_TEXT_HPP
#define INCLUDED_PKMN_CONVERSIONS_TEXT_HPP

#include <cstdint>

#include <pkmn/config.hpp>
#include <pkmn/types/pkstring.hpp>

namespace pkmn
{
    namespace conversions
    {
        pkmn::pkstring PKMN_API import_gen1_text(const uint8_t* buffer, uint8_t max_len);
        void PKMN_API export_gen1_text(const pkmn::pkstring& text, uint8_t* buffer, uint8_t max_len);

        pkmn::pkstring PKMN_API import_gen2_text(const uint8_t* buffer, uint8_t max_len);
        void PKMN_API export_gen2_text(const pkmn::pkstring& text, uint8_t* buffer, uint8_t max_len);

        pkmn::pkstring PKMN_API import_gen3_text(const uint8_t* buffer, uint8_t max_len);
        void PKMN_API export_gen3_text(const pkmn::pkstring& text, uint8_t* buffer, uint8_t max_len);

        pkmn::pkstring PKMN_API import_gen4_text(const uint16_t* buffer, uint8_t max_len);
        void PKMN_API export_gen4_text(const pkmn::pkstring& text, uint16_t* buffer, uint8_t max_len);

        pkmn::pkstring PKMN_API import_modern_text(const uint16_t* buffer, uint8_t max_len);
        void PKMN_API export_modern_text(const pkmn::pkstring& text, uint16_t* buffer, uint8_t max_len);
    }
}
#endif /* INCLUDED_PKMN_CONVERSIONS_TEXT_HPP */
