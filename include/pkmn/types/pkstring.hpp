/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TYPES_PKSTRING_HPP
#define INCLUDED_PKMN_TYPES_PKSTRING_HPP

#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include <pkmn/config.hpp>

namespace pkmn
{
    /*!
     * This is the custom string class used throughout LibPKMN. This class ignores C/C++'s distinction
     * between multi-byte and wide strings, with implicit assignment operators and equality checks back
     * and forth this class and all C/C++ strings, using this class as an intermediary. As such, users
     * can use std::string and std::wstring as desired, without having to worry about using the class
     * itself.
     *
     * When put through SWIG for the Python and C# wrappers, it serves a similar function, with implicit
     * operators between this class and those languages' native strings.
     */
    class PKMN_API pkstring
    {
        public:

            pkstring() {};
            pkstring(const char* input);
            pkstring(const wchar_t* input);
            pkstring(const std::string& input);
            pkstring(const std::wstring& input);

            //! Return the string as a multi-byte C-string.
            const char* const_char() const;

            //! Return the string as a wide C-string.
            const wchar_t* const_wchar_t() const;

            //! Return the string as a std::string.
            std::string std_string() const;

            //! Return the string as a std::wstring.
            std::wstring std_wstring() const;

            //! Set the string to the given multi-byte C-string.
            void set(const char* input);

            //! Set the string to the given wide C-string.
            void set(const wchar_t* input);

            //! Set the string to the given std::string.
            void set(const std::string& input);

            //! Set the string to the given std::wstring.
            void set(const std::wstring& input);

            //! Return the number of characters in the string.
            size_t length() const;

            //! Implicit typecast between pkmn::pkstring and std::string.
            PKMN_INLINE operator std::string() const {return stdstring;}

            //! Implicit typecast between pkmn::pkstring and std::wstring.
            PKMN_INLINE operator std::wstring() const {return stdwstring;}

            //! Return the character at the given value (returned as a char).
            const char& operator[](size_t pos) const;

        private:

            std::string stdstring;
            std::wstring stdwstring;
    };

    //Related typedefs
    typedef std::pair<pkmn::pkstring, pkmn::pkstring> pkstring_pair_t;
    typedef std::vector<pkmn::pkstring> pkstring_vector_t;

    //! Equality check between two pkmn::pkstrings
    PKMN_API bool operator==(const pkmn::pkstring& l, const pkmn::pkstring& r);
#ifndef SWIG
    //! Use pkmn::pkstring in std::cin or std::istringstream
    PKMN_API std::istream& operator>>(std::istream& stream, pkstring& text);

    //! Use pkmn::pkstring in std::wcin or std::wsistringstream
    PKMN_API std::wistream& operator>>(std::wistream& wstream, pkstring& text);

    //! Use pkmn::pkstring in std::cout or std::ostringstream
    PKMN_API std::ostream& operator<<(std::ostream& stream, const pkstring& text);

    //! Use pkmn::pkstring in std::wcout or std::wostringstream
    PKMN_API std::wostream& operator<<(std::wostream& wstream, const pkstring& text);
#endif
}

#endif /* INCLUDED_PKMN_TYPES_PKSTRING_HPP */
