/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%extend pkmn::shared_ptr<pkmn::pokemon> {
    %pythoncode %{

        def get_native(self):
            import conversions

            gen = self.get_generation()

            if gen == 1:
                return conversions.export_gen1_pokemon(self)
            elif gen == 2:
                return conversions.export_gen2_pokemon(self)
            elif gen == 3:
                return conversions.export_gen3_pokemon(self, False)
            else:
                return conversions.export_nds_pokemon(self, False)
    %}
};
