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

%template(ItemList)           std::vector<pkmn::cs::BagSlot>;
%template(Moveset)            std::vector<pkmn::cs::MoveEntry>;
%template(PokemonEntryVector) std::vector<pkmn::cs::PokemonEntry>;

%extend pkmn::pocket{
    std::vector<pkmn::cs::BagSlot> getItemList(){
        pkmn::item_list_t item_list = self->get_item_list();
        std::vector<pkmn::cs::BagSlot> itemList;

        BOOST_FOREACH(const pkmn::bag_slot_t &slot, item_list){
            pkmn::cs::BagSlot bagSlot;
            bagSlot.item   = pkmn::cs::ItemEntry(self->get_game(), slot.item.name);
            bagSlot.amount = slot.amount;

            itemList.push_back(bagSlot);
        }

        return itemList;
    }
}

%extend pkmn::pokedex{
    pkmn::cs::PokemonEntry getPokemonEntry(const uint16_t speciesID, const uint16_t formID){
        return pkmn::cs::PokemonEntry(self->get_game_id(), speciesID, formID);
    }

    pkmn::cs::PokemonEntry getPokemonEntry(const std::wstring &speciesName, const std::wstring &formName){
        return pkmn::cs::PokemonEntry(self->get_game(), speciesName, formName);
    }

    pkmn::cs::MoveEntry getMoveEntry(const uint16_t moveID){
        return pkmn::cs::MoveEntry(self->get_game_id(), moveID);
    }

    pkmn::cs::MoveEntry getMoveEntry(const std::wstring &moveName){
        return pkmn::cs::MoveEntry(self->get_game(), moveName);
    }

    pkmn::cs::ItemEntry getItemEntry(const uint16_t itemID){
        return pkmn::cs::ItemEntry(self->get_game_id(), itemID);
    }

    pkmn::cs::ItemEntry getItemEntry(const std::wstring &itemName){
        return pkmn::cs::ItemEntry(self->get_game(), itemName);
    }
}

%extend pkmn::pokemon{
    pkmn::cs::PokemonEntry getPokedexEntry(){
        return pkmn::cs::PokemonEntry(self->get_game_id(), self->get_species_id(), self->get_form_id());
    }

    pkmn::cs::ItemEntry getHeldItem(){
        return pkmn::cs::ItemEntry(self->get_game_id(), self->get_item_id());
    }

    pkmn::cs::MoveEntry getMove(uint8_t pos){
        return pkmn::cs::MoveEntry(self->get_game(), self->get_move(pos).name);
    }

    void getMoves(std::vector<pkmn::cs::MoveEntry> &moves){
        moves.clear();
        pkmn::moveset_t native_moves;
        self->get_moves(native_moves);
        for(size_t i = 0; i < 4; i++){
            moves.push_back(pkmn::cs::MoveEntry(self->get_game(), self->get_move(i).name));
        }
    }
}

%ignore pkmn::pokemon_entry_t;
%ignore pkmn::move_entry_t;
%ignore pkmn::item_entry_t;
%ignore pkmn::bag_slot_t;
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
