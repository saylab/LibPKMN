/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TRAINER_HPP
#define INCLUDED_PKMN_TRAINER_HPP

#include <string>

#include <pkmn/bag.hpp>
#include <pkmn/config.hpp>
#include <pkmn/pokemon.hpp>
#include <pkmn/types/pkstring.hpp>
#include <pkmn/types/shared_ptr.hpp>

namespace pkmn
{
    /*!
     * This class represents a <a href="http://bulbapedia.bulbagarden.net/wiki/Trainer">Pokémon trainer</a>. From
     * this class, you can access a trainer's party and bag, as well as information about them, such as
     * their name and ID.
     *
     * To instantiate a trainer shared_ptr, you can use one of two methods:
     * <pre>
     * pkmn::trainer::sptr pkmn_trainer = pkmn::trainer::make(pkmn::Games::HEART_GOLD, "Gold", pkmn::Genders::MALE);
     * pkmn::trainer::sptr pkmn_trainer = pkmn::trainer::make("Heart Gold", "Gold", "Male");
     * </pre>
     */
    class PKMN_API trainer
    {
        public:

            typedef pkmn::shared_ptr<trainer> sptr;

            /*!
             * This is the class's factory function. It returns a shared_ptr to a trainer
             * based on the given parameters.
             *
             * \param game SQLite ID of the desired game
             * \param name trainer's name
             * \param gender SQLite ID of the trainer's gender
             * \return shared pointer to instance of pkmn::trainer with described parameters
             */
            static sptr make(int game, const pkmn::pkstring &name, int gender);

            /*!
             * This is the class's factory function. It takes in the name of the game this trainer
             * comes from, the trainer's name, and the trainer's gender.
             *
             * \param game of the desired game
             * \param name trainer's name
             * \param gender trainer's gender
             * \return shared pointer to instance of pkmn::trainer with described parameters
             */
            static sptr make(const pkmn::pkstring &game, const pkmn::pkstring &name, const pkmn::pkstring &gender);

            //! Class constructor (use factory function instead)
            trainer() {};
            virtual ~trainer() {};

            /*!
             * Return the name of the game from which this trainer was created.
             *
             * Examples: "Red", "Diamond"
             *
             * \return game's name
             */
            virtual pkmn::pkstring get_game() const = 0;

            /*!
             * Return the generation (1-6) of the game from which this trainer
             * was created.
             *
             * \return game's generation
             */
            virtual int get_generation() const = 0;

            /*!
             * Return the trainer's name.
             *
             * \return name of trainer
             */
            virtual pkmn::pkstring get_name() const = 0;

            /*!
             * Return the amount of money the trainer currently has (0-999999).
             *
             * \return trainer's money amount
             */
            virtual int get_money() const = 0;

            /*!
             * Return the trainer's gender ("Male" or "Female").
             *
             * \return trainer's gender
             */
            virtual pkmn::pkstring get_gender() const = 0;

            /*!
             * Return the trainer's full ID. In Generations I-II, this value can span the entire
             * range of an unsigned 16-bit number (0-65535). In Generations III-VI, this range is
             * extended to the range of an unsigned 32-bit number (0-4294967295); however, the in-game
             * value is always seen as the second 16-bits of the number.
             *
             * \return trainer's full ID (both secret and public)
             */
            virtual uint32_t get_id() const = 0;
            
            /*!
             * In Generations I-II, return the trainer's full ID. In Generations III-VI, return the ID
             * displayed in-game, which is the second half of the full ID. This number can be anywhere
             * in the range 0-65535.
             *
             * \return trainer's ID as seen in-game
             */
            virtual uint16_t get_public_id() const = 0;

            /*!
             * In Generations III-VI, there is a second half of the trainer ID that is never seen in-game.
             * It is the first half of the full trainer ID. In Generations I-II, this value is always 0.
             *
             * \return trainer's secret ID, never seen in-game
             */
            virtual uint16_t get_secret_id() const = 0;

            /*!
             * Set the trainer's name. This value must be of length 1-7, or the function will do nothing.
             *
             * \param name trainer's new name
             */
            virtual void set_name(const pkmn::pkstring &name) = 0;

            /*!
             * Set the amount of money the trainer has. This value must be 0-999999, or the function will do nothing.
             *
             * \param money new amount of money
             */
            virtual void set_money(int money) = 0;

            /*!
             * Set the trainer's new gender. This value must be "Male" or "Female", or the function will do nothing.
             *
             * \param gender trainer's new gender
             */
            virtual void set_gender(const pkmn::pkstring &gender) = 0;

            /*!
             * Set the trainer's new full ID. This value must be in range 0-4294967295 (the range of an unsigned 32-bit
             * number). If the trainer comes from Generation I-II and the given ID is greater than 65535, this effectively
             * sets the value to 65535.
             *
             * \param id new trainer ID
             */
            virtual void set_id(uint32_t id) = 0;

            /*!
             * Sets the trainer's new public ID, as seen in-game. This value must be in range 0-65536. For trainers from
             * Generations I-II, this is effectively the same as set_id.
             *
             * \param id trainer's new public ID
             */
            virtual void set_public_id(uint16_t id) = 0;

            /*!
             * Sets the trainer's new secret ID, which is never seen in-game. This value must be in range 0-65536.
             * For trainers from Generations I-II, this function does nothing.
             *
             * \param id trainer's new secret ID
             */
            virtual void set_secret_id(uint16_t id) = 0;

            /*!
             * Return the Pokémon at the given position in the trainer's party. The given position must be in the range
             * 1-6, or this function will return an Invalid Pokémon.
             *
             * \param pos Position of Pokémon in party to return
             * \return Pokémon in given position
             */
            virtual pokemon::sptr get_pokemon(int pos) = 0;

            /*!
             * Replaces the Pokémon at the given position (1-6). Currently, this will fail if the new Pokémon is not from the
             * same version group.
             *
             * \param pos position to place new Pokémon
             * \param pkmn new Pokémon
             */
            virtual void set_pokemon(int pos, pokemon::sptr pkmn) = 0;

            /*!
             * Removes the Pokémon at the given position. Shift any subsequent Pokémon backward.
             *
             * \param pos position of Pokémon to remove.
             */
            virtual void remove_pokemon(int pos) = 0;

            /*!
             * Places the party in the given pokemon_team vector.
             *
             * \return party
             */
            virtual const pokemon_team_t& get_party() const = 0;

            /*!
             * Sets the party to the given pokemon_vector. Currently only takes Pokémon
             * from the same game.
             *
             * \param party reference to a pokemon_team vector
             */
            virtual void set_party(const pokemon_team_t &party) = 0;

            //! Returns a pointer to the trainer's bag.
            virtual bag::sptr get_bag() const = 0;

            //! Return SQLite Database ID of game used to create this item instance
            virtual int get_game_id() const = 0;

            //! Public trainer ID compatible with a modern shiny Pokémon
            static const uint16_t LIBPKMN_PUBLIC_ID = 46479;

            //! Secret trainer ID compatible with a modern shiny Pokémon
            static const uint16_t LIBPKMN_SECRET_ID = 2858;

            //! Full trainer ID compatible with a modern shiny Pokémon
            static const uint32_t LIBPKMN_TRAINER_ID = 3046050602;
    };
}

#endif /* INCLUDED_PKMN_TRAINER_HPP */
