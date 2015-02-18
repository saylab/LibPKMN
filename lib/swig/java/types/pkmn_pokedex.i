/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "stdint.i"
%include "std_pair.i"
%include "std_vector.i"
%include "std_wstring.i"

%{
    #include <boost/foreach.hpp>

    #include "entry_wrappers.hpp"
%}

%template(BagSlot)            std::pair<pkmn::java::ItemEntry, uint16_t>;
%template(ItemList)           std::vector<std::pair<pkmn::java::ItemEntry, uint16_t> >;
%template(Moveset)            std::vector<pkmn::java::MoveEntry>;
%template(PokemonEntryVector) std::vector<pkmn::java::PokemonEntry>;

%extend pkmn::pocket{
    void getItemList(std::vector<std::pair<pkmn::java::ItemEntry, uint16_t> > &itemList){
        itemList.clear();
        pkmn::item_list_t item_list;
        self->get_item_list(item_list);

        BOOST_FOREACH(const pkmn::bag_slot_t &slot, item_list){
            itemList.push_back(std::make_pair(pkmn::java::ItemEntry(self->get_game(),slot.first.name),
                                              slot.second));
        }
    }
}

%extend pkmn::pokedex{
    pkmn::java::PokemonEntry getPokemonEntry(const uint16_t speciesID, const uint16_t formID){
        return pkmn::java::PokemonEntry(self->get_game_id(), speciesID, formID);
    }

    pkmn::java::PokemonEntry getPokemonEntry(const pkmn::pkstring &speciesName, const pkmn::pkstring &formName){
        return pkmn::java::PokemonEntry(self->get_game(), speciesName, formName);
    }

    pkmn::java::MoveEntry getMoveEntry(const uint16_t moveID){
        return pkmn::java::MoveEntry(self->get_game_id(), moveID);
    }

    pkmn::java::MoveEntry getMoveEntry(const pkmn::pkstring &moveName){
        return pkmn::java::MoveEntry(self->get_game(), moveName);
    }

    pkmn::java::ItemEntry getItemEntry(const uint16_t itemID){
        return pkmn::java::ItemEntry(self->get_game_id(), itemID);
    }

    pkmn::java::ItemEntry getItemEntry(const pkmn::pkstring &itemName){
        return pkmn::java::ItemEntry(self->get_game(), itemName);
    }
}

%extend pkmn::pokemon{
    pkmn::java::PokemonEntry getPokedexEntry(){
        return pkmn::java::PokemonEntry(self->get_game_id(), self->get_species_id(), self->get_form_id());
    }

    pkmn::java::ItemEntry getHeldItem(){
        return pkmn::java::ItemEntry(self->get_game_id(), self->get_item_id());
    }

    pkmn::java::MoveEntry getMove(uint8_t pos){
        return pkmn::java::MoveEntry(self->get_game(), self->get_move(pos).name);
    }

    void getMoves(std::vector<pkmn::java::MoveEntry> &moves){
        moves.clear();
        pkmn::moveset_t native_moves;
        self->get_moves(native_moves);
        for(size_t i = 0; i < 4; i++){
            moves.push_back(pkmn::java::MoveEntry(self->get_game(), self->get_move(i).name));
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
