/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <random>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/test/unit_test.hpp>

#include <pkmn/io.hpp>
#include <pkmn/paths.hpp>

namespace fs = boost::filesystem;

BOOST_AUTO_TEST_CASE(gen1_pksql_test)
{
    srand((unsigned int)time(NULL));

    pkmn::pokemon::sptr pkmn1 = pkmn::pokemon::make("Bulbasaur", "Red", 50,
                                                    "Tackle", "Vine Whip",
                                                    "Sleep Powder", "Razor Leaf");

    std::string filename = str(boost::format("pksql_gen1_%d.pksql") % rand());
    fs::path filepath = fs::path(fs::path(pkmn::get_tmp_dir()) / filename);

    pkmn::io::export_to_pksql(pkmn1, filepath.string());
    pkmn::pokemon::sptr pkmn2 = pkmn::pokemon::make(filepath.string());

    fs::remove(filepath);
}
