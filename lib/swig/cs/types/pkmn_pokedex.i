/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "stdint.i"
%include "std_pair.i"
%include "std_vector.i"
%include "std_wstring.i"

/*
 * This workaround is necessary because of a bug in SWIG in which typemaps
 * are not properly propogated into structs that use the given class.
 * This bug is theoretically only present in versions 2.0.5-3.0.0, but I
 * have encountered it in 3.0.1, so this workaround is necessary.
 *
 * Unfortunately, this method means most entries won't be cached, but it's
 * the only way to get this to work the way we want.
 */

%{
    #include <boost/foreach.hpp>

    #include "entry_wrappers.hpp"
%}

%template(BagSlot)            std::pair<ItemEntry, uint16_t>;
%template(ItemList)           std::vector<std::pair<ItemEntry, uint16_t> >;
%template(Moveset)            std::vector<MoveEntry>;
%template(PokemonEntryVector) std::vector<PokemonEntry>;

%extend pkmn::pocket{
    void getItemList(std::vector<std::pair<ItemEntry, uint16_t> >& itemList){
        itemList.clear();
        pkmn::item_list_t item_list;
        self->get_item_list(item_list);

        BOOST_FOREACH(const pkmn::bag_slot_t& slot, item_list){
            itemList.push_back(std::make_pair(ItemEntry(self->get_game(),slot.first.name),
                                              slot.second));
        }
    }
}

%extend pkmn::pokedex{
    PokemonEntry getPokemonEntry(const uint16_t speciesID, const uint16_t formID){
        return PokemonEntry(self->get_game_id(), speciesID, formID);
    }

    PokemonEntry getPokemonEntry(const std::wstring& speciesName, const std::wstring& formName){
        return PokemonEntry(self->get_game(), speciesName, formName);
    }

    MoveEntry getMoveEntry(const uint16_t moveID){
        return MoveEntry(self->get_game_id(), moveID);
    }

    MoveEntry getMoveEntry(const std::wstring& moveName){
        return MoveEntry(self->get_game(), moveName);
    }

    ItemEntry getItemEntry(const uint16_t itemID){
        return ItemEntry(self->get_game_id(), itemID);
    }

    ItemEntry getItemEntry(const std::wstring& itemName){
        return ItemEntry(self->get_game(), itemName);
    }
}

%extend pkmn::pokemon{
    PokemonEntry getPokedexEntry(){
        return PokemonEntry(self->get_game_id(), self->get_species_id(), self->get_form_id());
    }

    ItemEntry getHeldItem(){
        return ItemEntry(self->get_game_id(), self->get_item_id());
    }

    MoveEntry getMove(uint8_t pos){
        return MoveEntry(self->get_game(), self->get_move(pos).name);
    }

    void getMoves(std::vector<MoveEntry>& moves){
        moves.clear();
        pkmn::moveset_t native_moves;
        self->get_moves(native_moves);
        for(size_t i = 0; i < 4; i++){
            moves.push_back(MoveEntry(self->get_game(), self->get_move(i).name));
        }
    }
}

%ignore pkmn::pokemon_entry_t;
%ignore pkmn::move_entry_t;
%ignore pkmn::item_entry_t;
%ignore pkmn::pocket::get_item_list;
%ignore pkmn::pokedex::get_pokemon_entry;
%ignore pkmn::pokedex::get_move_entry;
%ignore pkmn::pokedex::get_item_entry;
%ignore pkmn::pokemon::get_pokedex_entry;
%ignore pkmn::pokemon::get_held_item;
%ignore pkmn::pokemon::get_move;
%ignore pkmn::pokemon::get_moves;

%include "pkmn/pokedex.hpp"
%include "entry_wrappers.hpp"

%{
    #include "pkmn/pokedex.hpp"
    #include "pkmn/pokemon.hpp"
%}
