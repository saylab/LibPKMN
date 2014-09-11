/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System.Windows.Forms;

namespace LibPKMN
{
    public class TypeListComboBox: ComboBox
    {
        public TypeListComboBox(uint gen): base()
        {
            PKStringVector type_vec = new PKStringVector();
            LibPKMN.getTypeList(type_vec, gen);
            for(int i = 0; i < type_vec.Count; i++)
            {
                Items.Add(type_vec[i]);
            }
        }
    }
}
