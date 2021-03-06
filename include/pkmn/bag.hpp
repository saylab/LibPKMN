/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_BAG_HPP
#define INCLUDED_PKMN_BAG_HPP

#include <pkmn/config.hpp>
#include <pkmn/pocket.hpp>
#include <pkmn/types/dict.hpp>
#include <pkmn/types/shared_ptr.hpp>

namespace pkmn
{
    /*!
     * This class represents a trainer's <a href="http://bulbapedia.bulbagarden.net/wiki/Bag">bag</a>,
     * which holds various items in its different pockets. This class can return pointers to each of
     * its pockets, so users can add items directly there. The user also has the choice to add items
     * from this class, and it will automatically choose the correct pocket in which to place the
     * item.
     *
     * To instantiate a bag shared_ptr, you can use one of two methods:
     * <pre>
     * pkmn::bag::sptr pkmn_bag = pkmn::bag::make(pkmn::Games::DIAMOND);
     * pkmn::bag::sptr pkmn_bag = pkmn::bag::make("Diamond");
     * </pre>
     *
     * In specifying a game, specific pockets are generated. These pockets can be accessed with the
     * following command:
     * <pre>
     * pkmn::pocket::sptr pkmn_pocket = pkmn_bag->get_pocket("<Pocket Name>");
     * </pre>
     *
     * The list of pockets for each game group, as well as their sizes, are listed below:
     * <ul>
     *     <li>Red/Blue/Yellow
     *         <ul>
     *             <li>Items (20)</li>
     *         </ul>
     *     </li>
     *     <li>Gold/Silver/Crystal
     *         <ul>
     *             <li>Items (20)</li>
     *             <li>Balls (12)</li>
     *             <li>KeyItems (26)</li>
     *             <li>TM/HM (99)</li>
     *         </ul>
     *     </li>
     *     <li>Ruby/Sapphire
     *         <ul>
     *             <li>Items (20)</li>
     *             <li>Poke Balls (16)</li>
     *             <li>TMs and HMs (64)</li>
     *             <li>Berries (46)</li>
     *             <li>Key Items (20)</li>
     *         </ul>
     *     </li>
     *     <li>Emerald
     *         <ul>
     *             <li>Items (20)</li>
     *             <li>Poke Balls (16)</li>
     *             <li>TMs and HMs (64)</li>
     *             <li>Berries (46)</li>
     *             <li>Key Items (30)</li>
     *         </ul>
     *     </li>
     *     <li>Fire Red/Leaf Green
     *         <ul>
     *             <li>Items (42)</li>
     *             <li>Key Items (30)</li>
     *             <li>Poke Balls (13)</li>
     *             <li>TM Case (58)</li>
     *             <li>Berry Pouch (43)</li>
     *         </ul>
     *     </li>
     *     <li>Diamond/Pearl/Platinum
     *         <ul>
     *             <li>Items (165)</li>
     *             <li>Medicine (40)</li>
     *             <li>Poke Balls (15)</li>
     *             <li>TMs and HMs (100)</li>
     *             <li>Berries (64)</li>
     *             <li>Mail (12)</li>
     *             <li>Battle Items (30)</li>
     *             <li>Key Items (50)</li>
     *         </ul>
     *     </li>
     *     <li>Heart Gold/Soul Silver
     *         <ul>
     *             <li>Items (165)</li>
     *             <li>Medicine (40)</li>
     *             <li>Poke Balls (24)</li>
     *             <li>TMs and HMs (100)</li>
     *             <li>Berries (64)</li>
     *             <li>Mail (12)</li>
     *             <li>Battle Items (30)</li>
     *             <li>Key Items (43)</li>
     *         </ul>
     *     </li>
     *     <li>Black/White/Black 2/White 2/X/Y
     *         <ul>
     *             <li>Items (310)</li>
     *             <li>Medicine (48)</li>
     *             <li>TMs and HMs (109)</li>
     *             <li>Berries (64)</li>
     *             <li>Key Items (83)</li>
     *         </ul>
     *     </li>
     * </ul>
     */
    class PKMN_API bag
    {
        public:

            typedef pkmn::shared_ptr<bag> sptr;

            /*!
             * This is the class's factory function. It returns a shared_ptr to a bag
             * based on the given game.
             *
             * \param game SQLite ID of the desired game, as defined in <pkmn/enums.hpp>
             * \return shared pointer to instance of pkmn::bag from the given game
             */
            static sptr make(int game);

            /*!
             * This is the class's factory function. It returns a shared_ptr to a bag
             * based on the given game.
             *
             * \param game name of the desired game
             * \return shared pointer to instance of pkmn::trainer with described parameters
             */
            static sptr make(const pkmn::pkstring &game);

            bag() {};
            virtual ~bag() {};

            /*!
             * Return the name of the game from which this bag was created.
             *
             * Examples: "Red", "Diamond"
             *
             * \return game's name
             */
            virtual pkmn::pkstring get_game() const = 0;

            /*!
             * Return the generation (1-6) of the game from which this bag
             * was created.
             *
             * \return game's generation
             */
            virtual int get_generation() const = 0;

            /*!
             * Return the amount of the given item.
             *
             * \param item_id SQLite ID of the given item, as defined in <pkmn/enums.hpp>
             * \return amount of given item
             */
            virtual int get_item_amount(int item_id) const = 0;

            /*!
             * Return the amount of the given item.
             *
             * \param item_name name of given item
             * \return amount of given item
             */
            virtual int get_item_amount(const pkmn::pkstring &item_name) const = 0;

            /*!
             * Set the amount of the given item.
             *
             * \param item_id SQLite ID of given item
             * \param amount amount of given item
             */
            virtual void set_item_amount(int item_id, int amount) = 0;            

            /*!
             * Set the amount of the given item.
             *
             * \param item_name name of given item
             * \param amount amount of given item
             */
            virtual void set_item_amount(const pkmn::pkstring &item_name, int amount) = 0;            
            
            /*!
             * Return the pocket with the given name. Pocket names for each
             * game group are listed above.
             *
             * \param name name of pocket
             * \return shared_ptr to given pocket
             */
            virtual pocket::sptr get_pocket(const pkmn::pkstring &name) const = 0;

            /*!
             * Return all of the bag's pockets in a dictionary. Each pocket
             * can be accessed by using its name as the dictionary key.
             *
             * \return dictionary of pockets
             */
            virtual pkmn::dict<pkmn::pkstring, pocket::sptr> get_pockets() const = 0;

            //! Return SQLite ID of the game used to create this bag
            virtual int get_game_id() const = 0;
    };
}

#endif /* INCLUDED_PKMN_BAG_HPP */
