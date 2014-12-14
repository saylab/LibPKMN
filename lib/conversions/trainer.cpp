/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <string>

#include <boost/locale/encoding_utf.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/database.hpp>
#include <pkmn/types/pkstring.hpp>

#include "../library_bridge.hpp"
#include "items.hpp"
#include "pokemon.hpp"
#include "trainer.hpp"

//Copied from PokeLib/lib/Trainer.cpp

uint8_t itemNum[3][8] = {
    {165,50,100,12,40,64,15,30},
    {165,50,100,12,40,64,15,30},
    {165,43,100,12,40,64,24,30},
};
uint16_t itemOffset[3][8] = {
    {0x624,0x8B8,0x980,0xB10,0xB40,0xBE0,0xCE0,0xD1C},
    {0x630,0x8C4,0x98C,0xB1C,0xB4C,0xBEC,0xCEC,0xD28},
    {0x644,0x8D8,0x9A4,0xB34,0xB64,0xC04,0xD04,0xD64},
};

enum {
    offsetName = 0,
    offsetTID, offsetSID, offsetMoney,
    offsetGender, offsetCountry,
    offsetBadges, offsetBadges2,
    offsetAvatar, offsetRival,
};

uint16_t tOffset[][3] = {
    {0x0064, 0x0068, 0x0064}, //TNAME (x8 16 bit)
    {0x0074, 0x0078, 0x0074}, //TID
    {0x0076, 0x007A, 0x0076}, //SID
    {0x0078, 0x007C, 0x0078}, //MONEY
    {0x007C, 0x0080, 0x007C}, //GENDER
    {0x007D, 0x0081, 0x007D}, //COUNTRY
    {0x007E, 0x0082, 0x007E}, //BADGES (HGSS Johto)
    {0x007E, 0x0082, 0x0083}, //BADGES 2 (HGSS Kanto, others mirror)
    {0x007F, 0x0083, 0x007F}, //Multiplayer Avatar
    {0x25A8, 0x27E8, 0x22D4}, //Rival Name (x8 16 bit)
};

namespace pkmn
{
    namespace conversions
    {
        trainer::sptr import_gen4_trainer(pokelib_sptr pokelib_save)
        {
            PokeLib::Trainer* pokelib_trainer = pokelib_save->getTrainer();

            unsigned int game_id;
            unsigned int save_type = pokelib_save->getSaveType();

            /*
             * The save type doesn't distinguish between D/P or HG/SS, but
             * Pokemon have a field that does. Parse the party to try and
             * find a Pokemon with the same ID as the trainer and get
             * the correct game ID from that. If no such Pokemon exists,
             * use the save type.
             */
            PokeLib::Party* pokelib_party = pokelib_save->getParty();
            uint16_t pokelib_public_id = pokelib_trainer->getPID();
            uint16_t pokelib_secret_id = pokelib_trainer->getSID();
            bool found = false;

            for(size_t i = 1; i <= (pokelib_party->count()); i++)
            {
                PokeLib::Pokemon pokelib_pokemon = pokelib_party->getPokemon(i);

                if((pokelib_pokemon.pkm->pkm.ot_id == pokelib_public_id)
                    and (pokelib_pokemon.pkm->pkm.ot_sid == pokelib_secret_id))
                {
                    game_id = hometown_to_libpkmn_game(pokelib_pokemon.pkm->pkm.hometown);
                    found = true;
                    break;
                }
            }
            if(not found)
            {
                switch(save_type)
                {
                    case PokeLib::DP:
                        game_id = Versions::DIAMOND;
                        break;

                    case PokeLib::PLAT:
                        game_id = Versions::PLATINUM;
                        break;

                    default:
                        game_id = Versions::HEARTGOLD;
                        break;
                }
            }
            pkstring trainer_name = pokelib_trainer->getName();
            unsigned int gender = (pokelib_trainer->getFemale()) ? Genders::FEMALE : Genders::MALE;

            trainer::sptr libpkmn_trainer = trainer::make(game_id, trainer_name, gender);

            libpkmn_trainer->set_public_id(pokelib_public_id);
            libpkmn_trainer->set_secret_id(pokelib_secret_id);
            libpkmn_trainer->set_money(pokelib_trainer->getMoney());

            import_gen4_items(libpkmn_trainer->get_bag(), *pokelib_trainer);

            for(size_t i = 1; i <= (unsigned int)(pokelib_party->count()); i++)
            {
                PokeLib::Pokemon pokelib_pokemon = pokelib_party->getPokemon(i);

                if(pokelib_pokemon.pkm->pkm.species == 0) break;
                else libpkmn_trainer->set_pokemon(i, import_gen4_pokemon(pokelib_pokemon));
            }

            return libpkmn_trainer;
        }

        void export_gen4_trainer(trainer::sptr libpkmn_trainer, pokelib_sptr pokelib_save)
        {
            PokeLib::Trainer* pokelib_trainer = pokelib_save->getTrainer();

            pokelib_trainer->setName(libpkmn_trainer->get_name());
            pokelib_trainer->setFemale(libpkmn_trainer->get_gender().std_string() == "Female");

            export_gen4_items(libpkmn_trainer->get_bag(), pokelib_trainer);

            //PokeLib::Save::getTrainer returns new trainer
            pokelib_save->setTrainer(pokelib_trainer);

            PokeLib::Party* pokelib_party = pokelib_save->getParty();

            for(int i = 1; i <= 6; i++)
            {
                team_pokemon::sptr t_pkmn = libpkmn_trainer->get_pokemon(i);
                if(t_pkmn->get_species_id() == Species::NONE) break;
                else pokelib_party->setPokemon(i, export_gen4_pokemon(t_pkmn));
            }
        }

        trainer::sptr import_gen5_trainer(pkmds_g5_sptr pkmds_save)
        {
            /*
             * PKMDS has no way of distinguishing between the different Gen 5
             * games, and trainer gender hasn't been reverse engineered. To
             * find each, we will parse the party to determine the relevant
             * information from Pokemon with matching ID's. If none are found,
             * use defaults.
             */
            std::array<pkmds::party_pkm, 6> pkmds_party = pkmds_save->cur.party.pokemon;
            uint16_t pkmds_public_id = pkmds_save->cur.tid;
            uint16_t pkmds_secret_id = pkmds_save->cur.sid;
            unsigned int game_id, gender;
            bool found = false;

            for(size_t i = 0; i < pkmds_party.size(); i++)
            {
                pkmds::pokemon_obj* pkmds_pokemon = &(pkmds_party[i]);

                if((pkmds_pokemon->tid == pkmds_public_id) and (pkmds_pokemon->sid == pkmds_secret_id))
                {
                    game_id = hometown_to_libpkmn_game(int(pkmds_pokemon->hometown));
                    gender = (get_gen_456_otgender((reinterpret_cast<uint8_t*>(&(pkmds_pokemon->ball)+1)))) ?
                             Genders::FEMALE : Genders::MALE;
                    found = true;
                    break;
                }
            }
            if(not found)
            {
                game_id = Versions::BLACK_2;
                gender = Genders::MALE;
            }

            pkmn::pkstring pkmds_name = import_modern_text(pkmds_save->cur.trainername, 8);
            trainer::sptr libpkmn_trainer = trainer::make(game_id, pkmds_name, gender);

            libpkmn_trainer->set_public_id(pkmds_public_id);
            libpkmn_trainer->set_secret_id(pkmds_secret_id);
            libpkmn_trainer->set_money(0);

            import_gen5_items(libpkmn_trainer->get_bag(), &(pkmds_save->cur.bag));

            for(size_t i = 0; i < pkmds_party.size(); i++)
            {
                pkmds::decryptpkm(&pkmds_party[i]);
                libpkmn_trainer->set_pokemon(i+1, import_gen5_pokemon(&(pkmds_party[i])));
                pkmds::encryptpkm(&pkmds_party[i]);
            }

            return libpkmn_trainer;
        }

        void export_gen5_trainer(trainer::sptr libpkmn_trainer, pkmds_g5_sptr pkmds_save)
        {
            std::wstring trainer_name = libpkmn_trainer->get_name();

            for(size_t i = 0; i < trainer_name.size(); i++)
            {
                pkmds_save->cur.trainername[i] = trainer_name[i];
            }

            export_gen5_items(libpkmn_trainer->get_bag(), &(pkmds_save->cur.bag));

            for(size_t i = 1; i <= 6; i++)
            {
                team_pokemon::sptr t_pkmn = libpkmn_trainer->get_pokemon(i);

                if(t_pkmn->get_species_id() == Species::NONE) break;
                else export_gen5_pokemon(t_pkmn, &(pkmds_save->cur.party.pokemon[i-1]));
            }
        }
    } /* namespace conversions */
} /* namespace pkmn */
