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
        public class AbilityListComboBox: ComboBox
        {
            public AbilityListComboBox(uint gen): base()
            {
                string_vec ability_vec = new string_vec();
                Lists.getAbilityList(ability_vec, gen);
                for(int i = 0; i < ability_vec.Count; i++)
                {
                    Items.Add(ability_vec[i]);
                }
            }
        }
    }
}
