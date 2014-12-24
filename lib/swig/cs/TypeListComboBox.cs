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
        public class TypeListComboBox: ComboBox
        {
            public TypeListComboBox(uint gen): base()
            {
                StringVector type_vec = new StringVector();
                Lists.getTypeList(type_vec, gen);
                for(int i = 0; i < type_vec.Count; i++)
                {
                    Items.Add(type_vec[i]);
                }
            }
        }
    }
}
