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

namespace pkmn
{
    namespace conversions
    {
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
