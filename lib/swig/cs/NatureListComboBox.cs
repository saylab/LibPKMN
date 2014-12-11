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
        public class NatureListComboBox: ComboBox
        {
            public NatureListComboBox(): base()
            {
                string_vec nature_vec = new string_vec();
                Lists.getNatureList(nature_vec);
                for(int i = 0; i < nature_vec.Count; i++)
                {
                    Items.Add(nature_vec[i]);
                }
            }
        }
    }
}
