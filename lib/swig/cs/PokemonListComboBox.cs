/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System.Windows.Forms;

namespace PKMN
{
    namespace Forms
    {
        public class PokemonListComboBox: ComboBox
        {
            public PokemonListComboBox(uint version_id): base()
            {
                StringVector pokemon_vec = new StringVector();
                Lists.getPokemonList(pokemon_vec, version_id);
                for(int i = 0; i < pokemon_vec.Count; i++)
                {
                    Items.Add(pokemon_vec[i]);
                }
            }
        }
    }
}
