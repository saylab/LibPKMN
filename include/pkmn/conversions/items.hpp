/*
 * Copyright (c) 2013-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_CONVERSIONS_ITEMS_HPP
#define INCLUDED_PKMN_CONVERSIONS_ITEMS_HPP

#include <cstdint>

#include <pkmn/bag.hpp>
#include <pkmn/config.hpp>
#include <pkmn/native/items.hpp>

namespace pkmn
{
    namespace conversions
    {
        //! Import a Generation I item bag into a LibPKMN bag
        /*!
         * Converts the Generation I in-game representation of a trainer's items into a LibPKMN
         * bag::sptr.
         *
         * \param libpkmn_bag LibPKMN bag representation
         * \param raw_bag pointer to Generation I bag representation
         */
        void PKMN_API import_gen1_bag(bag::sptr libpkmn_bag, const native::gen1_item_bag_t* raw_bag);

        //! Export a LibPKMN bag into a Generation I in-game representation
        /*!
         * Converts a LibPKMN bag::sptr into the Generation I in-game representation of a trainer's
         * items.
         *
         * \param libpkmn_bag LibPKMN bag representation
         * \param raw_bag pointer to Generation I bag representation
         */
        void PKMN_API export_gen1_bag(bag::sptr libpkmn_bag, native::gen1_item_bag_t* raw_bag);

        //! Import a Generation II item bag into a LibPKMN bag
        /*!
         * Converts the Generation II in-game representation of a trainer's items into a LibPKMN
         * bag::sptr.
         *
         * \param libpkmn_bag LibPKMN bag representation
         * \param raw_bag pointer to Generation II bag representation
         */
        void PKMN_API import_gen2_bag(bag::sptr libpkmn_bag, const native::gen2_item_bag_t* raw_bag);

        //! Export a LibPKMN bag into a Generation II in-game representation
        /*!
         * Converts a LibPKMN bag::sptr into the Generation II in-game representation of a trainer's
         * items.
         *
         * \param libpkmn_bag LibPKMN bag representation
         * \param raw_bag pointer to Generation II bag representation
         */
        void PKMN_API export_gen2_bag(bag::sptr libpkmn_bag, native::gen2_item_bag_t* raw_bag);

        //! Import a Generation III item bag into a LibPKMN bag
        /*!
         * Converts a Generation III in-game representation of a trainer's item bag into a LibPKMN
         * bag::sptr. Interally converts the given pointer into the correct in-game representation
         * based on which game the LibPKMN bag comes from.
         *
         * \param libpkmn_bag LibPKMN bag representation
         * \param raw_bag pointer to Generation III in-game representation
         * \param security_key key used to decrypt item amounts
         */
        void PKMN_API import_gen3_bag(bag::sptr libpkmn_bag, const void* raw_bag, const uint16_t security_key);

        //! Export a LibPKMN bag into a Generation III in-game representation
        /*!
         * Converts a LibPKMN bag::sptr into a Generation III in-game representation of a trainer's
         * item bag. Internally converts the given pointer into the correct in-game representation
         * based on which game the LibPKMN bag comes from.
         *
         * \param libpkmn_bag LibPKMN bag representation
         * \param raw_bag pointer to Generation III in-game representation
         * \param security_key key used to decrypt item amounts
         */
        void PKMN_API export_gen3_bag(bag::sptr libpkmn_bag, void* raw_bag, const uint16_t security_key);

        //! Import a Generation IV item bag into a LibPKMN bag
        /*!
         * Converts a Generation IV in-game representation of a trainer's item bag into a LibPKMN
         * bag::sptr. Interally converts the given pointer into the correct in-game representation
         * based on which game the LibPKMN bag comes from.
         *
         * \param libpkmn_bag LibPKMN bag representation
         * \param raw_bag pointer to Generation IV in-game representation
         */
        void PKMN_API import_gen4_bag(bag::sptr libpkmn_bag, const void* raw_bag);

        //! Export a LibPKMN bag into a Generation IV in-game representation
        /*!
         * Converts a LibPKMN bag::sptr into a Generation IV in-game representation of a trainer's
         * item bag. Internally converts the given pointer into the correct in-game representation
         * based on which game the LibPKMN bag comes from.
         *
         * \param libpkmn_bag LibPKMN bag representation
         * \param raw_bag pointer to Generation IV in-game representation
         */
        void PKMN_API export_gen4_bag(bag::sptr libpkmn_bag, void* raw_bag);
    }
}

#endif /* INCLUDED_PKMN_CONVERSIONS_ITEMS_HPP */
