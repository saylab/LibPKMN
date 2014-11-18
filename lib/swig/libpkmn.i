/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "exception.i"
%include "stdint.i"
%include "std_string.i"
%include "std_pair.i"
%include "std_vector.i"
%include "std_wstring.i"
%include "stl.i"
%include "typemaps.i"

%exception {
  try {
    $action
  } catch (const std::exception& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
}

namespace pkmn
{
    template<class T> class shared_ptr
    {
        public:
            shared_ptr() {}
            shared_ptr(T * p) {}
            T * operator-> () {return px;}

        private:
            T * px;
            int pn;
    };
}

namespace std
{
    %template(uchar_pair) pair<uint8_t, uint8_t>;
    %template(uint_pair) pair<unsigned int, unsigned int>;
    %template(string_pair) pair<std::string, std::string>;

    %template(int_vec) vector<int>;
    %template(double_vec) vector<double>;
    %template(uint_vec) vector<unsigned int>;
};

/*
 * Ignore C++ operators and some duplicate constructors.
 * Their functionality will be replicated in the other languages.
 */
%ignore pkmn::dict::operator[];
%ignore pkmn::markings::operator uint8_t;
%ignore pkmn::pkstring::pkstring(const std::string&);
%ignore pkmn::pkstring::pkstring(const uint16_t*);
%ignore pkmn::pkstring::pkstring(const wchar_t*);
%ignore pkmn::pkstring::pkstring(const std::wstring&);
%ignore pkmn::pkstring::operator[];
%ignore pkmn::pkstring::operator std::string;
%ignore pkmn::pkstring::operator const uint16_t*;
%ignore pkmn::pkstring::operator std::wstring;
%ignore pkmn::pkstring::const_wchar_t() const;
%ignore pkmn::pkstring::std_string() const;
%ignore pkmn::pkstring::std_wstring() const;
%ignore pkmn::pkstring::set(const std::string&);
%ignore pkmn::pkstring::set(const wchar_t*);
%ignore pkmn::pkstring::set(const std::wstring&);
%ignore pkmn::nature::operator[];

%{
    #include "pkmn/config.hpp"
%}

%include "pkmn/config.hpp"
