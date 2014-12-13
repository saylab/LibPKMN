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
        public class VersionListComboBox: ComboBox
        {
            public VersionListComboBox(): base()
            {
                string_vec version_vec = new string_vec();
                Lists.getVersionList(version_vec);
                for(int i = 0; i < version_vec.Count; i++)
                {
                    Items.Add(version_vec[i]);
                }
            }
        }
    }
}
