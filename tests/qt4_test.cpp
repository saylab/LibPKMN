/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <vector>

#include <boost/test/unit_test.hpp>

#include <QApplication>

#include <pkmn/enums.hpp>
#include <pkmn/lists.hpp>
#include <pkmn/qt4/AbilityListComboBox.hpp>
#include <pkmn/qt4/ItemListComboBox.hpp>
#include <pkmn/qt4/NatureListComboBox.hpp>
#include <pkmn/qt4/PokemonListComboBox.hpp>
#include <pkmn/qt4/TypeListComboBox.hpp>
#include <pkmn/qt4/VersionListComboBox.hpp>
#include <pkmn/types/pkstring.hpp>

QApplication app(0,0);

/*
 * Make sure all abilities are accounted for and that and
 * that strings convert correctly.
 */
BOOST_AUTO_TEST_CASE(AbilityListComboBox_test)
{
    std::vector<pkmn::pkstring> abilities;
    pkmn::get_ability_list(abilities, 6);
    pkmn::qt4::AbilityListComboBox comboBox(6);

    for(size_t i = 0; i < abilities.size(); i++)
        BOOST_CHECK_EQUAL(comboBox.findText(QString::fromUtf16(abilities[i])), i);
}

/*
 * Make sure all items are accounted for and that and
 * that strings convert correctly.
 */
BOOST_AUTO_TEST_CASE(ItemListComboBox_test)
{
    std::vector<pkmn::pkstring> items;
    pkmn::get_item_list(items, pkmn::Versions::Y);
    pkmn::qt4::ItemListComboBox comboBox(pkmn::Versions::Y);

    for(size_t i = 0; i < items.size(); i++)
        BOOST_CHECK_EQUAL(comboBox.findText(QString::fromUtf16(items[i])), i);
}

/*
 * Make sure all natures are accounted for and that and
 * that strings convert correctly.
 */
BOOST_AUTO_TEST_CASE(NatureListComboBox_test)
{
    std::vector<pkmn::pkstring> natures;
    pkmn::get_nature_list(natures);
    pkmn::qt4::NatureListComboBox comboBox;

    for(size_t i = 0; i < natures.size(); i++)
        BOOST_CHECK_EQUAL(comboBox.findText(QString::fromUtf16(natures[i])), i);
}

/*
 * Make sure all Pokémon are accounted for and that and
 * that strings convert correctly.
 */
BOOST_AUTO_TEST_CASE(PokemonListComboBox_test)
{
    std::vector<pkmn::pkstring> pokemon;
    pkmn::get_pokemon_list(pokemon, pkmn::Versions::Y);
    pkmn::qt4::PokemonListComboBox comboBox(pkmn::Versions::Y);

    for(size_t i = 0; i < pokemon.size(); i++)
        BOOST_CHECK_EQUAL(comboBox.findText(QString::fromUtf16(pokemon[i])), i);
}

/*
 * Make sure all types are accounted for and that and
 * that strings convert correctly.
 */
BOOST_AUTO_TEST_CASE(TypeListComboBox_test)
{
    std::vector<pkmn::pkstring> types;
    pkmn::get_type_list(types, 6);
    pkmn::qt4::TypeListComboBox comboBox(6);

    for(size_t i = 0; i < types.size(); i++)
        BOOST_CHECK_EQUAL(comboBox.findText(QString::fromUtf16(types[i])), i);
}

/*
 * Make sure all versions are accounted for and that and
 * that strings convert correctly.
 */
BOOST_AUTO_TEST_CASE(VersionListComboBox_test)
{
    std::vector<pkmn::pkstring> versions;
    pkmn::get_version_list(versions);
    pkmn::qt4::VersionListComboBox comboBox;

    for(size_t i = 0; i < versions.size(); i++)
        BOOST_CHECK_EQUAL(comboBox.findText(QString::fromUtf16(versions[i])), i);
}
