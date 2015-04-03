/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_CONVERSIONS_MISC_HPP
#define INCLUDED_PKMN_CONVERSIONS_MISC_HPP

#include <cstdint>

#include <pkmn/config.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/pkstring.hpp>

namespace pkmn
{
    namespace conversions
    {
        pkmn::dict<pkmn::pkstring, int> PKMN_API import_gb_IVs(uint16_t iv_data);

        void PKMN_API export_gb_IV(const pkmn::pkstring &stat, int value, uint16_t &iv_data);

        uint32_t PKMN_API import_gb_money(uint8_t* money_buf);

        void PKMN_API export_gb_money(uint32_t money, uint8_t* money_buf);
    }
}
#endif /* INCLUDED_PKMN_CONVERSIONS_MISC_HPP */
