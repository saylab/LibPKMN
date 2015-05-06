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
            game_save_impl(const pkmn::pkstring &filename);

            pkmn::pkstring get_game() const;
            int get_generation() const;

            PKMN_INLINE void save()
            {
                save_as(_filepath.string());
            }

            void save_as(const pkmn::pkstring &filename);

            trainer::sptr get_trainer() const;

            bag::sptr get_trainer_bag() const;

            const pokemon_team_t &get_trainer_party() const;
            void set_trainer_party(const pokemon_team_t &party);

            pkmn::pkstring get_trainer_name() const;
            void set_trainer_name(const pkmn::pkstring &trainer_name);

            PKMN_INLINE const std::vector<uint8_t>& get_raw()
            {
                _write_data();
                return _data;
            }

            PKMN_INLINE int get_game_id() const
            {
                return _version_id;
            }

        protected:

            virtual void _write_data() = 0;

            boost::filesystem::path _filepath;
            std::vector<uint8_t> _data;

            int _version_id;
            trainer::sptr _trainer;
    };
}
#endif /* INCLUDED_GAME_SAVE_IMPL_HPP */
