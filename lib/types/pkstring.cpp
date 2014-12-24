/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <sstream>
#include <stdexcept>
#include <string>

#include <boost/locale/encoding_utf.hpp>

#include <pkmn/types/pkstring.hpp>

#define CONVERT_STRING(str,type) boost::locale::conv::utf_to_utf<type>(str)

namespace pkmn
{
    pkstring::pkstring(const char* input) {set(input);}
    pkstring::pkstring(const uint16_t* input) {set(input);}
    pkstring::pkstring(const wchar_t* input) {set(input);}
    pkstring::pkstring(const std::string& input) {set(input);}
    pkstring::pkstring(const std::basic_string<uint16_t>& input) {set(input);}
    pkstring::pkstring(const std::wstring& input) {set(input);}

    const char* pkstring::const_char() const {return stdstring.c_str();}
    const wchar_t* pkstring::const_wchar_t() const {return stdwstring.c_str();}
    std::string pkstring::std_string() const {return stdstring;}
    std::wstring pkstring::std_wstring() const {return stdwstring;}

    void pkstring::set(const char* input)
    {
        stdstring = std::string(input);
        stdstring16 = CONVERT_STRING(input,uint16_t);
        stdwstring = CONVERT_STRING(input,wchar_t);
    }

    void pkstring::set(const uint16_t* input)
    {
        stdstring = CONVERT_STRING(input,char);
        stdstring16 = std::basic_string<uint16_t>(input);
        stdwstring = CONVERT_STRING(input,wchar_t);
    }

    void pkstring::set(const wchar_t* input)
    {
        stdstring = CONVERT_STRING(input,char);
        stdstring16 = CONVERT_STRING(input,uint16_t);
        stdwstring = std::wstring(input);
    }

    void pkstring::set(const std::string& input)
    {
        stdstring = input;
        stdstring16 = CONVERT_STRING(input,uint16_t);
        stdwstring = CONVERT_STRING(input.c_str(),wchar_t);
    }

    void pkstring::set(const std::basic_string<uint16_t>& input)
    {
        stdstring = CONVERT_STRING(input,char);
        stdstring16 = input;
        stdwstring = CONVERT_STRING(input,wchar_t);
    }

    void pkstring::set(const std::wstring& input)
    {
        stdstring = CONVERT_STRING(input.c_str(),char);
        stdstring16 = CONVERT_STRING(input,uint16_t);
        stdwstring = input;
    }

    size_t pkstring::length() const {return stdstring.length();}

    bool pkstring::operator==(const pkstring &r) const
    {
        return this->std_wstring() == r.std_wstring();
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
