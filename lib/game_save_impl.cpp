/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <fstream>
#include <iostream>
#include <string>

#include <boost/filesystem.hpp>

#include <pkmn/enums.hpp>
#include <pkmn/game_save.hpp>
#include <pkmn/paths.hpp>

#include "game_save_impl.hpp"
#include "game_save_gen1impl.hpp"
#include "game_save_gen2impl.hpp"
#include "game_save_gen3impl.hpp"
#include "game_save_gen4impl.hpp"
#include "game_save_gen5impl.hpp"
#include "library_bridge.hpp"
#include "conversions/trainer.hpp"

namespace fs = boost::filesystem;

namespace pkmn
{
    game_save::sptr game_save::make(const pkmn::pkstring &filename)
    {
        std::vector<uint8_t> data(fs::file_size(fs::path(filename.std_string())));
        std::ifstream ifile(filename.const_char(), std::ios::binary);
        ifile.read((char*)&data[0], data.size());
        ifile.close();

        uint32_t size = data.size();
        if(size >= 0x80000)
        {
            gen4_games_t game_type = get_gen4_save_type(data);
            if(game_type) return sptr(new game_save_gen4impl(filename, game_type, false));
            else
            {
                //Check to see if PKMDS accepts this as a proper Gen V save
                pkmds_g5_sptr sav = pkmds_g5_sptr(new pkmds::bw2sav_obj);
                pkmds::read(filename.const_char(), sav.get());
                if(pkmds::savisbw2(sav.get())) return sptr(new game_save_gen5impl(sav, filename));
            }
        }
        else if(size >= 0x40000)
        {
            gen4_games_t game_type = get_gen4_save_type(data);
            if(game_type) return sptr(new game_save_gen4impl(filename, game_type, true));
        }
        else if(size >= 0x20000)
        {
            if(frlg_check(data))
            {
                return sptr(new game_save_gen3impl(filename, Versions::FIRERED));
            }
            else if(emerald_check(data))
            {
                return sptr(new game_save_gen3impl(filename, Versions::EMERALD));
            }
            else if(rs_check(data))
            {
                return sptr(new game_save_gen3impl(filename, Versions::RUBY));
            }
        }
        else if(size >= (2 << 14))
        {
            if(gen1_check(data))
            {
                return sptr(new game_save_gen1impl(filename));
            }
            else if(gs_check(data))
            {
                return sptr(new game_save_gen2impl(filename, false));
            }
            else if(crystal_check(data))
            {
                return sptr(new game_save_gen2impl(filename, true));
            }
        }

        throw std::runtime_error("This is not a valid save file.");
    }

    game_save_impl::game_save_impl(const pkmn::pkstring &filename)
    {
        _filepath = fs::path(filename);
        uint32_t file_size = fs::file_size(filename.std_string());

        _data = std::vector<uint8_t>(file_size);
        std::ifstream ifile(filename.const_char(), std::ios::binary);
        ifile.read((char*)&_data[0], file_size);
        ifile.close();
    }

    void game_save_impl::save()
    {
        save_as(_filepath.string());
    }

    unsigned int game_save_impl::get_game_id() const {return _game_id;}

    trainer::sptr game_save_impl::get_trainer() const {return _trainer;}

    bag::sptr game_save_impl::get_trainer_bag() const {return _trainer->get_bag();}

    void game_save_impl::get_trainer_party(pokemon_team_t2& party) {_trainer->get_party(party);}

    void game_save_impl::set_trainer_party(pokemon_team_t2& party) {_trainer->set_party(party);}

    pkmn::pkstring game_save_impl::get_trainer_name() const {return _trainer->get_name();}

    void game_save_impl::set_trainer_name(const pkmn::pkstring &trainer_name) {_trainer->set_name(trainer_name);}

} /* namespace pkmn */
