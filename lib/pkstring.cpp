/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <stdexcept>
#include <string>

#include <boost/locale/encoding_utf.hpp>

#include <pkmn/types/pkstring.hpp>

#define BOOST_NARROW(str) boost::locale::conv::utf_to_utf<char>(str)
#define BOOST_WIDEN(str)  boost::locale::conv::utf_to_utf<wchar_t>(str)

namespace pkmn
{
    pkstring::pkstring(const char* input) {set(input);}
    pkstring::pkstring(const wchar_t* input) {set(input);}
    pkstring::pkstring(const std::string& input) {set(input);}
    pkstring::pkstring(const std::wstring& input) {set(input);}

    const char* pkstring::const_char() const {return stdstring.c_str();}
    const wchar_t* pkstring::const_wchar_t() const {return stdwstring.c_str();}
    std::string pkstring::std_string() const {return stdstring;}
    std::wstring pkstring::std_wstring() const {return stdwstring;}

    void pkstring::set(const char* input)
    {
        stdstring = std::string(input);
        stdwstring = BOOST_WIDEN(input);
    }

    void pkstring::set(const wchar_t* input)
    {
        stdstring = BOOST_NARROW(input);
        stdwstring = std::wstring(input);
    }

    void pkstring::set(const std::string& input)
    {
        stdstring = input;
        stdwstring = BOOST_WIDEN(input.c_str());
    }

    void pkstring::set(const std::wstring& input)
    {
        stdstring = BOOST_NARROW(input.c_str());
        stdwstring = input;
    }

    size_t pkstring::length() const {return stdstring.length();}

    const char& pkstring::operator[](size_t pos) const
    {
        size_t size = stdstring.size();
        if(pos >= size)
        {
            std::stringstream err_stream;
            err_stream << "Position must be 0'" << stdstring.size() << ".";
            throw std::runtime_error(err_stream.str().c_str());
        }
        else return stdstring[pos];
    }

    bool operator==(const pkstring& l, const pkstring& r)
    {
        return (l.std_string() == r.std_string()
               and (l.std_wstring() == r.std_wstring()));
    }

    std::istream& operator>>(std::istream& stream, pkstring& text)
    {
        std::string to_set;
        stream >> to_set;
        text.set(to_set);
        return stream;
    }

    std::wistream& operator>>(std::wistream& wstream, pkstring& text)
    {
        std::wstring to_set;
        wstream >> to_set;
        text.set(to_set);
        return wstream;
    }

    std::ostream& operator<<(std::ostream& stream, const pkstring& text)
    {
        stream << text.std_string();
        return stream;
    }

    std::wostream& operator<<(std::wostream& wstream, const pkstring& text)
    {
        wstream << text.std_wstring();
        return wstream;
    }
} /* namespace pkmn */