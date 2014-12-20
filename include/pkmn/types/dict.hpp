/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TYPES_DICT_HPP
#define INCLUDED_PKMN_TYPES_DICT_HPP

#include <list>
#include <vector>

#include <pkmn/config.hpp>

namespace pkmn
{
    /*!
     * This class is a custom dictionary implementation. This class's API
     * is designed to match the API of each language's closest class.
     *
     *  * C++: std::map
     *  * C#: System.Collections.Generic.Dictionary
     *  * Java: java.util.HashMap
     *  * Python: dict
     */
    template <typename Key, typename Val> class dict
    {
        public:
        
            dict();

            template <typename InputIterator>
            dict(InputIterator first, InputIterator last);

            //! Return true if the dictionary is empty. Return false otherwise.
            bool empty() const;

            //! Return the number of key-value pairs.
            size_t size() const;

            //! Return the value with the given key. Key-value pair is created if it does not exist.
            /*!
             * Creates the key-value pair if it does not exist.
             *
             * \param key key whose value to query
             * \return key's value
             */
            const Val& operator[](const Key& key) const;

            //! Set the value with the given key.
            /*!
             * \param key key whose value to set
             */
            Val& operator[](const Key& key);

            //! Return the value with the given key.
            /*!
             * Throws an exception if the given key does not exist.
             *
             * \param key key whose's value to query
             * \return key's value
             */
            const Val& at(const Key& key) const;

            //! Return the value with the given key, or the given value if the key does not exist.
            /*
             * \param key key whose's value to query
             * \param other default value to return
             * \return key's value, or default key if key does not exist
             */
            const Val& at(const Key& key, const Val& other) const;

            //! Set the value with the given key.
            /*!
             * \param key key whose value to set
             * \param val value to set key
             */
            void insert(const Key& key, const Val& val);

            //! Erase the key-value pair with the given key.
            /*!
             * Throws an exception if the given key does not exist.
             *
             * \param key whose value to erase
             */
            void erase(const Key& key);

            //! Swap the values associated with the given keys;
            /*!
             * Throws an exception if the either key does not exist.
             *
             * \param key1 first key whose value to swap
             * \param key2 second key whose value to swap
             */
            void swap(Key key1, Key key2);

            //! Remove all key-value pairs.
            void clear();

            //! Return true if the given key exists in the dictionary. Return false otherwise.
            bool has_key(const Key& key) const;

            //! Return a vectors of the dictionary's keys.
            /*!
             * The order of these keys is guaranteed to match that of the values returned from vals().
             */
            std::vector<Key> keys() const;

            //! Return a vectors of the dictionary's values.
            /*!
             * The order of these values is guaranteed to match that of the keys returned from keys().
             */
            std::vector<Val> vals() const;

        private:
        
            typedef std::pair<Key, Val> dict_pair;
            std::list<dict_pair> _map;
        };

}

#include <pkmn/types/dict.ipp>

#endif /* INCLUDED_PKMN_TYPES_DICT_HPP */
