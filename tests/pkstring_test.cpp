/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/test/unit_test.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/database.hpp>
#include <pkmn/types/pkstring.hpp>

/*
 * Test pkstring's implicit conversions with std::string, std::wstring, and
 * std::basic_string<uint16_t>.
 */
BOOST_AUTO_TEST_CASE(pkstring_conversions)
{
    std::string test_stdstring1 = "Test string";
    std::wstring test_stdwstring1 = L"Test string";
    pkmn::pkstring test_pkstring1 = test_stdstring1;
    pkmn::pkstring test_pkstring2 = test_stdwstring1;
    std::basic_string<uint16_t> test_string16_1 = (const uint16_t*)test_pkstring1;
    std::basic_string<uint16_t> test_string16_2 = (const uint16_t*)test_pkstring2;

    BOOST_CHECK(test_pkstring1 == test_stdstring1);
    BOOST_CHECK(test_pkstring1 == test_stdwstring1);
    BOOST_CHECK(test_pkstring1 == test_string16_2);
    BOOST_CHECK(test_pkstring2 == test_stdstring1);
    BOOST_CHECK(test_pkstring2 == test_stdwstring1);
    BOOST_CHECK(test_pkstring2 == test_string16_1);
    BOOST_CHECK(test_pkstring1 == test_pkstring2);
    BOOST_CHECK(test_string16_1 == test_string16_2);
}

/*
 * Test pkstring's usage in LibPKMN's database functions. Specifically, make sure
 * get_*_id and get_*_name functions work with names that involve wide characters.
 */
BOOST_AUTO_TEST_CASE(pkstring_database_usage)
{
    pkmn::pkstring test_pkstring1 = "Torrent";
    BOOST_CHECK(test_pkstring1 == pkmn::database::get_ability_name(pkmn::database::get_ability_id(test_pkstring1)));
    BOOST_CHECK_EQUAL(pkmn::Items::POKE_BALL, pkmn::database::get_item_id(pkmn::database::get_item_name(pkmn::Items::POKE_BALL)));
    BOOST_CHECK_EQUAL(pkmn::Species::NIDORAN_M, pkmn::database::get_species_id(pkmn::database::get_species_name(pkmn::Species::NIDORAN_M)));
}
