/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "stdint.i"
%include "std_vector.i"

/*
 * Horribly hacky method, but SWIG's C# array module didn't work.
 */
%extend pkmn::shared_ptr<pkmn::pokemon> {
    std::vector<uint8_t> getNativeBase(){
        static int native_sizes[7] = {0,44,48,100,380,380,500};
        int size = native_sizes[self->get()->get_generation()];

        std::vector<uint8_t> ret(size);
        memcpy(&ret[0], self->get()->get_native(), size);

        return ret;
    }
}

%typemap(cscode) pkmn::shared_ptr<pkmn::pokemon> %{
    public byte[] getNative() {
        ByteVector fromCpp;

        switch(getGeneration()){
            case 1:
                byte[] ret1 = new byte[44]; // sizeof(pkmn::native::gen1_party_pokemon_t)
                fromCpp = getNativeBase();
                fromCpp.CopyTo(ret1);
                return ret1;

            case 2:
                byte[] ret2 = new byte[48]; // sizeof(pkmn::native::gen2_party_pokemon_t)
                fromCpp = getNativeBase();
                fromCpp.CopyTo(ret2);
                return ret2;

            case 3:
                byte[] ret3 = new byte[100]; // sizeof(pkmn::native::gen3_party_pokemon_t)
                fromCpp = getNativeBase();
                fromCpp.CopyTo(ret3);
                return ret3;

            case 6:
                byte[] ret6 = new byte[500]; // sizeof(pkmn::native::gen6_party_pokemon_t)
                fromCpp = getNativeBase();
                fromCpp.CopyTo(ret6);
                return ret6;

            default:
                byte[] retn = new byte[380]; // sizeof(pkmn::native::nds_party_pokemon_t)
                fromCpp = getNativeBase();
                fromCpp.CopyTo(retn);
                return retn;
        }
    }
%}
