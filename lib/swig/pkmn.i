/*
 * Copyright (c) 2013-2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include "exception.i"

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

    /*
     * This will remove the nested union from the struct
     * while preserving every field.
     */
    typedef struct
    {
        uint8_t coolness;
        uint8_t beauty;
        uint8_t cuteness;
        uint8_t smartness;
        uint8_t toughness;
        uint8_t feel;
        uint8_t sheen;
    } contest_stats_t;
}

/*
 * Ignore C++ operators and some duplicate constructors.
 * Their functionality will be replicated in the other languages.
 */
%ignore pkmn::pokemon::get_native;
%ignore pkmn::dict::operator[];
%ignore pkmn::markings_t::operator uint8_t;
%ignore pkmn::nature_t::operator[];
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
%ignore pkmn::hoenn_ribbons_t::operator uint32_t;
%ignore pkmn::sinnoh_ribbons1_t::operator uint16_t;
%ignore pkmn::sinnoh_ribbons2_t::operator uint16_t;
%ignore pkmn::sinnoh_ribbons3_t::operator uint32_t;
%ignore pkmn::unova_ribbons_t::operator uint16_t;
%ignore pkmn::kalos_ribbons_t::operator uint64_t;

%{
    #include "pkmn/config.hpp"
%}

%include "pkmn/config.hpp"
