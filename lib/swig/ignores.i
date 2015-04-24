/*
 * Copyright (c) 2015 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%ignore pkmn::pokemon::get_native;
%ignore pkmn::dict::operator[];
%ignore pkmn::markings_t::operator uint8_t;
%ignore pkmn::nature_t::operator[];

%ignore pkmn::pkstring::pkstring(const uint16_t*);
%ignore pkmn::pkstring::pkstring(const wchar_t*);
%ignore pkmn::pkstring::pkstring(const std::string&);
%ignore pkmn::pkstring::pkstring(const std::basic_string<uint16_t>&);
%ignore pkmn::pkstring::pkstring(const std::wstring&);
%ignore pkmn::pkstring::operator==;
%ignore pkmn::pkstring::operator[];
%ignore pkmn::pkstring::operator const char*;
%ignore pkmn::pkstring::operator const uint16_t*;
%ignore pkmn::pkstring::operator const wchar_t*;
%ignore pkmn::pkstring::operator std::string;
%ignore pkmn::pkstring::operator std::basic_string<uint16_t>;
%ignore pkmn::pkstring::operator std::wstring;
%ignore pkmn::pkstring::const_uint16_t() const;
%ignore pkmn::pkstring::const_wchar_t() const;
%ignore pkmn::pkstring::std_string() const;
%ignore pkmn::pkstring::std_string16() const;
%ignore pkmn::pkstring::std_wstring() const;
%ignore pkmn::pkstring::set(const uint16_t*);
%ignore pkmn::pkstring::set(const wchar_t*);
%ignore pkmn::pkstring::set(const std::string&);
%ignore pkmn::pkstring::set(const std::basic_string<uint16_t>&);
%ignore pkmn::pkstring::set(const std::wstring&);

%ignore pkmn::pokerus_t::operator uint8_t;
%ignore pkmn::hoenn_ribbons_t::operator uint32_t;
%ignore pkmn::sinnoh_ribbons1_t::operator uint16_t;
%ignore pkmn::sinnoh_ribbons2_t::operator uint16_t;
%ignore pkmn::sinnoh_ribbons3_t::operator uint32_t;
%ignore pkmn::unova_ribbons_t::operator uint16_t;
%ignore pkmn::kalos_ribbons_t::operator uint64_t;
%ignore pkmn::super_training_medals_t::operator uint32_t;
