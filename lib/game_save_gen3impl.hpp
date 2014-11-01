/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_GEN3IMPL_HPP
#define INCLUDED_GAME_SAVE_GEN3IMPL_HPP

#include <boost/format.hpp>

#include <pkmn/config.hpp>

#include "game_save_impl.hpp"

#include "conversions/structs/gen3_save.hpp"
#include "conversions/structs/items.hpp"
#include "conversions/structs/pokemon.hpp"

namespace pkmn
{
    enum saves
    {
        SAVE_A = 0x0000,
        SAVE_B = 0xE000
    };

    enum games
    {
        RS,
        EMERALD,
        FRLG
    };

    enum section0
    {
        GAME_CODE,
        SECURITY_KEY1,
        SECURITY_KEY2
    };

    /*
     * Source: http://bulbapedia.bulbagarden.net/wiki/Save_data_structure_in_Generation_III#Section_0_-_trainer_info
     */
    static const uint16_t section0_offsets[3][3] =
    {
        {0xAC,0xAC,0x1F4}, //RS has no security key
        {0xAC,0xAC,0x1F4}, //Emerald has no game code
        {0xAC,0xAF8,0xF20}
    };

    enum section1
    {
        POKEMON_PARTY,
        MONEY,
        ITEM_STORAGE
    };

    /*
     * Source: http://bulbapedia.bulbagarden.net/wiki/Save_data_structure_in_Generation_III#Section_1_-_team_.2F_items
     */
    static const uint16_t section1_offsets[3][3] =
    {
        {0x234,0x490,0x498},
        {0x234,0x490,0x498},
        {0x34,0x290,0x298},
    };

    static const uint16_t RIVAL_NAME = 0x0BCC; //Only in FR/LG

    //How many bytes in each section are read for the checksum
    static const uint16_t gen3_section_sizes[14] =
    {
        3884,3968,3968,3968,3848,3968,3968,
        3968,3968,3968,3968,3968,3968,2000
    };

    #define GEN3_SAVE_INDEX(save) save->section0.footer.save_index
    #define GEN3_SECURITY_KEY(save,game,num) *reinterpret_cast<const uint32_t*>(&save->section0.data8[section0_offsets[game][num]])

    PKMN_INLINE gen3_save_t* gen3_get_main_save(uint8_t* data)
    {
        gen3_save_t* saveA = reinterpret_cast<gen3_save_t*>(&data[SAVE_A]);
        gen3_save_t* saveB = reinterpret_cast<gen3_save_t*>(&data[SAVE_B]);

        /*
         * Each file stores two saves. When the game saves, it alternates which of the two save is overwritten,
         * and that save's index value is incremented. Whichever save has the highest value is the most recent.
         *
         * NOTE: This is how it works in theory. In development, I've noticed this is not always the case.
         *       There was a file where one save was used ten times, while the other was never used.
         */
        return (GEN3_SAVE_INDEX(saveA) > GEN3_SAVE_INDEX(saveB)) ? saveA : saveB;
    }

    //The save sections aren't necessarily in order
    void PKMN_INLINE gen3_crypt_save_sections(const gen3_save_t* src, gen3_save_t* dest)
    {
        for(size_t i = 0; i < 14; i++)
        {
            uint8_t section_id = src->sections[i].footer.section_id;
            dest->sections[section_id] = src->sections[i];
        }
    }

    bool PKMN_INLINE gen3_section_check(const gen3_save_t* save)
    {
        for(size_t i = 0; i < 14; i++)
        {
            if(save->sections[i].footer.validation != 0x08012025) return false;
        }

        return true;
    }

    //Check R/S game code+security key
    bool PKMN_INLINE rs_check(std::vector<uint8_t> &data)
    {
        const gen3_save_t* raw_saveA = reinterpret_cast<const gen3_save_t*>(&data[SAVE_A]);
        const gen3_save_t* raw_saveB = reinterpret_cast<const gen3_save_t*>(&data[SAVE_B]);

        gen3_save_t* saveA = new gen3_save_t;
        gen3_save_t* saveB = new gen3_save_t;

        gen3_crypt_save_sections(raw_saveA, saveA);
        gen3_crypt_save_sections(raw_saveB, saveB);

        uint32_t saveA_key1 = GEN3_SECURITY_KEY(saveA,RS,SECURITY_KEY1);
        uint32_t saveB_key1 = GEN3_SECURITY_KEY(saveB,RS,SECURITY_KEY1);

        delete saveA;
        delete saveB;

        return gen3_section_check(saveA) and gen3_section_check(saveB)
               and ((saveA_key1 == 0) and (saveB_key1 == 0));
    }

    //Check Emerald security key
    bool PKMN_INLINE emerald_check(std::vector<uint8_t> &data)
    {
        const gen3_save_t* raw_save = gen3_get_main_save((uint8_t*)&data[0]);
        gen3_save_t* save = new gen3_save_t;
        gen3_crypt_save_sections(raw_save, save);

        uint32_t security_key1 = GEN3_SECURITY_KEY(save,EMERALD,SECURITY_KEY1);
        uint32_t security_key2 = GEN3_SECURITY_KEY(save,EMERALD,SECURITY_KEY2);

        delete save;

        return gen3_section_check(save) and ((security_key1 == security_key2) and
                                             (security_key1 != 0) and (security_key2 != 0));
    }

    //Check FR/LG security key
    bool PKMN_INLINE frlg_check(std::vector<uint8_t> &data)
    {
        const gen3_save_t* raw_save = gen3_get_main_save((uint8_t*)&data[0]);
        gen3_save_t* save = new gen3_save_t;
        gen3_crypt_save_sections(raw_save, save);

        uint32_t security_key1 = GEN3_SECURITY_KEY(save,FRLG,SECURITY_KEY1);
        uint32_t security_key2 = GEN3_SECURITY_KEY(save,FRLG,SECURITY_KEY2);

        delete save;

        return gen3_section_check(save) and ((security_key1 == security_key2) and
                                             (security_key1 != 0) and (security_key2 != 0));
    }

    class game_save_gen3impl: public game_save_impl
    {
        public:

            game_save_gen3impl(const std::string &filename, unsigned int game_id);
            ~game_save_gen3impl() {free(_save);}
            
            void load();
            void save_as(const std::string &filename);
            bool check();
            
        private:

            gen3_save_t* _raw_save; //Main save
            gen3_save_t* _save; //Unscrambled, what we'll actually be using
            games _game;

            gen3_pokemon_party_t* _pokemon_party;
            void* _item_storage;

            uint32_t _security_key;
    };
}

#endif /* INCLUDED_GAME_SAVE_GEN3IMPL_HPP */
