/*
 * Copyright (c) 2014-2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_TYPES_PKSTRING_HPP
#define INCLUDED_PKMN_TYPES_PKSTRING_HPP

#include <cstdint>
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
     */
    class PKMN_API pkstring
    {
        public:

            pkstring() {};
            pkstring(const char* input);
            pkstring(const uint16_t* input);
            pkstring(const wchar_t* input);
            pkstring(const std::string &input);
            pkstring(const std::basic_string<uint16_t> &input);
            pkstring(const std::wstring &input);

            //! Return the string as a multi-byte C-string.
            PKMN_INLINE const char* const_char() const {
                return stdstring.c_str();
            }

            //! Return the string as a 2-byte C-string.
            PKMN_INLINE const uint16_t* const_uint16_t() const {
                return stdstring16.c_str();
            }

            //! Return the string as a wide C-string.
            PKMN_INLINE const wchar_t* const_wchar_t() const {
                return stdwstring.c_str();
            }

            //! Return the string as a std::string.
            PKMN_INLINE const std::string& std_string() const {
                return stdstring;
            }

            //! Return the string as a 2-byte STL string.
            PKMN_INLINE const std::basic_string<uint16_t>& std_string16() const {
                return stdstring16;
            }

            //! Return the string as a std::wstring.
            PKMN_INLINE const std::wstring& std_wstring() const {
                return stdwstring;
            }

            //! Set the string to the given multi-byte C-string.
            void set(const char* input);

            //! Set the string to the given 2-byte C-string.
            void set(const uint16_t* input);

            //! Set the string to the given wide C-string (size is platform-dependent).
            void set(const wchar_t* input);

            //! Set the string to the given std::string.
            void set(const std::string &input);

            //! Set the string to the given 2-byte STL string.
            void set(const std::basic_string<uint16_t> &input);

            //! Set the string to the given std::wstring (character size is platform-dependent).
            void set(const std::wstring &input);

            //! Return the number of characters in the string.
            PKMN_INLINE size_t length() const {
                return stdwstring.length();
            }

            //! Implicit typecast between pkmn::pkstring and const char*.
            PKMN_INLINE operator const char*() const {
                return stdstring.c_str();
            }

            //! Implicit typecast between pkmn::pkstring and const uint16_t*.
            PKMN_INLINE operator const uint16_t*() const {
                return stdstring16.c_str();
            };

#ifndef PKMN_PLATFORM_WIN32
            //! Implicit typecast between pkmn::pkstring and const wchar_t*.
            PKMN_INLINE operator const wchar_t*() const {
                return stdwstring.c_str();
            }
#endif

            PKMN_INLINE operator std::string() const {
                return stdstring;
            }

            //! Implicit typecast between pkmn::pkstring and std::basic_string<uint16_t>.
            PKMN_INLINE operator std::basic_string<uint16_t>() const {
                return stdstring16;
            }

            //! Implicit typecast between pkmn::pkstring and std::wstring.
            PKMN_INLINE operator std::wstring() const {
                return stdwstring;
            }

            //! Equality check between two pkmn::pkstrings.
            PKMN_INLINE bool operator==(const pkmn::pkstring &r) const {
                return (this->std_wstring() == r.std_wstring());
            }

            //! Equality check between a pkmn::pkstring and a const char*.
            PKMN_INLINE bool operator==(const char* r) const {
                return (this->std_string() == std::string(r));
            }

            //! Equality check between a pkmn::pkstring and a const uint16_t*.
            PKMN_INLINE bool operator==(const uint16_t* r) const {
                return (this->std_string16() == std::basic_string<uint16_t>(r));
            }

            //! Equality check between a pkmn::pkstring and a const wchar_t*.
            PKMN_INLINE bool operator==(const wchar_t* r) const {
                return (this->std_wstring() == std::wstring(r));
            }

            //! Equality check between a pkmn::pkstring and a std::string.
            PKMN_INLINE bool operator==(const std::string &r) const {
                return (this->std_string() == r);
            }

            //! Equality check between a pkmn::pkstring and a std::basic_string<uint16_t>.
            PKMN_INLINE bool operator==(const std::basic_string<uint16_t> &r) const {
                return (this->std_string16() == r);
            }

            //! Equality check between a pkmn::pkstring and a std::wstring.
            PKMN_INLINE bool operator==(const std::wstring &r) const {
                return (this->std_wstring() == r);
            }

        private:

            std::string stdstring;
            std::basic_string<uint16_t> stdstring16;
            std::wstring stdwstring;
    };

    //Related typedefs
    typedef std::pair<pkmn::pkstring, pkmn::pkstring> pkstring_pair_t;
    typedef std::vector<pkmn::pkstring> pkstring_vector_t;

#ifndef SWIG
    //! Use pkmn::pkstring in std::cin or std::istringstream
    PKMN_API std::istream& operator>>(std::istream &stream, pkstring &text);

    //! Use pkmn::pkstring in std::wcin or std::wsistringstream
    PKMN_API std::wistream& operator>>(std::wistream &wstream, pkstring &text);

    //! Use pkmn::pkstring in std::cout or std::ostringstream
    PKMN_API std::ostream& operator<<(std::ostream &stream, const pkstring &text);

    //! Use pkmn::pkstring in std::wcout or std::wostringstream
    PKMN_API std::wostream& operator<<(std::wostream &wstream, const pkstring &text);
#endif
}

#endif /* INCLUDED_PKMN_TYPES_PKSTRING_HPP */
