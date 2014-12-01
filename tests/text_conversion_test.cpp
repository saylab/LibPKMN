/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cstring>
#include <iostream>

#include <boost/test/unit_test.hpp>

#include "conversions/text.hpp"

BOOST_AUTO_TEST_CASE(gen1_test)
{
    // Make sure import/export functions match
    uint8_t gen1_input[15] = {0x8B,0xA8,0x81,0xAF,0x8A,0xAC,0x8D,
                              0xF7,0xF8,0xF9,0xFA,0xFB,
                              0x50,0x50,0x50};
    uint8_t gen1_output[15];

    pkmn::pkstring gen1_pkstring = pkmn::conversions::import_gen1_text(gen1_input, 15);
    BOOST_CHECK_EQUAL(gen1_pkstring.length(), 12);
    BOOST_CHECK(gen1_pkstring.std_string() == "LiBpKmN12345");
    pkmn::conversions::export_gen1_text(gen1_pkstring, gen1_output, 15);
    BOOST_CHECK(!memcmp(gen1_input, gen1_output, 15));

    // Confirm gender symbols work
    uint8_t gen1_genders[2] = {0xEF, 0xF5};

    std::wstring genders_str = pkmn::conversions::import_gen1_text(gen1_genders, 2);
    BOOST_CHECK_EQUAL(genders_str[0], 0x2642);
    BOOST_CHECK_EQUAL(genders_str[1], 0x2640);
}

BOOST_AUTO_TEST_CASE(gen2_test)
{
    // Make sure import/export functions match
    uint8_t gen2_input[15] = {0x8B,0xA8,0x81,0xAF,0x8A,0xAC,0x8D,
                              0xF7,0xF8,0xF9,0xFA,0xFB,
                              0x50,0x50,0x50};
    uint8_t gen2_output[15];

    pkmn::pkstring gen2_pkstring = pkmn::conversions::import_gen2_text(gen2_input, 15);
    BOOST_CHECK_EQUAL(gen2_pkstring.length(), 12);
    BOOST_CHECK(gen2_pkstring.std_string() == "LiBpKmN12345");
    pkmn::conversions::export_gen2_text(gen2_pkstring, gen2_output, 15);
    BOOST_CHECK(!memcmp(gen2_input, gen2_output, 15));
}

BOOST_AUTO_TEST_CASE(gen3_test)
{
    // Make sure import/export functions match
    uint8_t gen3_input[15] = {0xC6,0xDD,0xBC,0xE4,0xC5,0xE1,0xC8,
                              0xA2,0xA3,0xA4,0xA5,0xA6,
                              0xFF,0xFF,0xFF};
    uint8_t gen3_output[15];

    pkmn::pkstring gen3_pkstring = pkmn::conversions::import_gen3_text(gen3_input, 15);
    BOOST_CHECK_EQUAL(gen3_pkstring.length(), 12);
    BOOST_CHECK(gen3_pkstring.std_string() == "LiBpKmN12345");
    pkmn::conversions::export_gen3_text(gen3_pkstring, gen3_output, 15);
    BOOST_CHECK(!memcmp(gen3_input, gen3_output, 15));

    // Confirm gender characters work
    uint8_t gen3_genders[2] = {0xB5, 0xB6};

    std::wstring genders_str = pkmn::conversions::import_gen3_text(gen3_genders, 2);
    BOOST_CHECK_EQUAL(genders_str[0], 0x2642);
    BOOST_CHECK_EQUAL(genders_str[1], 0x2640);
}