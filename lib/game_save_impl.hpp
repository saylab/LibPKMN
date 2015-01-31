/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_GAME_SAVE_IMPL_HPP
#define INCLUDED_GAME_SAVE_IMPL_HPP

#include <vector>

#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>

#include <pkmn/game_save.hpp>

namespace pkmn
{
    class game_save_impl: public game_save, boost::noncopyable
    {
        public:

            game_save_impl() {};
            game_save_impl(const pkmn::pkstring& filename);

            pkmn::pkstring get_game() const;
            uint16_t get_generation() const;

            void save();

            trainer::sptr get_trainer() const;

            bag::sptr get_trainer_bag() const;

            void get_trainer_party(pokemon_team_t& party);
            void set_trainer_party(pokemon_team_t& party);

            pkmn::pkstring get_trainer_name() const;
            void set_trainer_name(const pkmn::pkstring& trainer_name);

            uint16_t get_game_id() const;

        protected:

            boost::filesystem::path _filepath;
            std::vector<uint8_t> _data;

            uint16_t _version_id;
            trainer::sptr _trainer;
    };
}
#endif /* INCLUDED_GAME_SAVE_IMPL_HPP */
