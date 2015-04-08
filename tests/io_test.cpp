/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <random>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/test/unit_test.hpp>

#include <pkmn/paths.hpp>
#include <pkmn/pokemon.hpp>

namespace fs = boost::filesystem;

BOOST_AUTO_TEST_CASE(_3gpkm_test)
{
    srand((unsigned int)time(NULL));

    pkmn::pokemon::sptr pkmn1 = pkmn::pokemon::make("Treecko", "Emerald", 50,
                                                    "Scratch", "Leech Seed",
                                                    "Razor Leaf", "None");

    std::string filename = str(boost::format("3gpkm_%d.3gpkm") % rand());
    fs::path filepath = fs::path(fs::path(pkmn::get_tmp_dir()) / filename);

    pkmn::pokemon::export_to(pkmn1, filepath.string());
    pkmn::pokemon::sptr pkmn2 = pkmn::pokemon::make(filepath.string());

    fs::remove(filepath);
}
