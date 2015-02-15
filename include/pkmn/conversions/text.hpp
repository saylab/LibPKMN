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
        //! Import text from the Generation I character map into a pkstring
        pkmn::pkstring PKMN_API import_gen1_text(const uint8_t* buffer, uint8_t max_len);

        //! Export a pkstring into a Generation I string
        void PKMN_API export_gen1_text(const pkmn::pkstring &text, uint8_t* buffer, uint8_t max_len);

        //! Import text from the Generation II character map into a pkstring
        pkmn::pkstring PKMN_API import_gen2_text(const uint8_t* buffer, uint8_t max_len);

        //! Export a pkstring into a Generation II string
        void PKMN_API export_gen2_text(const pkmn::pkstring &text, uint8_t* buffer, uint8_t max_len);

        //! Import text from the Generation III character map into a pkstring
        pkmn::pkstring PKMN_API import_gen3_text(const uint8_t* buffer, uint8_t max_len);

        //! Export a pkstring into a Generation III string
        void PKMN_API export_gen3_text(const pkmn::pkstring &text, uint8_t* buffer, uint8_t max_len);

        //! Import text from the Generation IV character map into a pkstring
        pkmn::pkstring PKMN_API import_gen4_text(const uint16_t* buffer, uint8_t max_len);

        //! Export a pkstring into a Generation IV string
        void PKMN_API export_gen4_text(const pkmn::pkstring &text, uint16_t* buffer, uint8_t max_len);

        //! Import text from a Generation V+ game into a pkstring
        pkmn::pkstring PKMN_API import_modern_text(const uint16_t* buffer, uint8_t max_len);

        //! Export a pkstring into a Generation V+ string
        void PKMN_API export_modern_text(const pkmn::pkstring &text, uint16_t* buffer, uint8_t max_len);
    }
}
#endif /* INCLUDED_PKMN_CONVERSIONS_TEXT_HPP */
