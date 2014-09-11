/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System.Windows.Forms;

namespace LibPKMN
{
    public class NatureListComboBox: ComboBox
    {
        public NatureListComboBox(): base()
        {
            PKStringVector nature_vec = new PKStringVector();
            LibPKMN.getNatureList(nature_vec);
            for(int i = 0; i < nature_vec.Count; i++)
            {
                Items.Add(nature_vec[i]);
            }
        }
    }
}
