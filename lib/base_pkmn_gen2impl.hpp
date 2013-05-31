/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_BASE_PKMN_GEN2IMPL_HPP
#define INCLUDED_BASE_PKMN_GEN2IMPL_HPP

#include <map>
#include "sqlitecpp/SQLiteCPP.h"
#include <pkmnsim/base_pkmn.hpp>
#include <pkmnsim/dict.hpp>
#include <string>

namespace pkmnsim
{
    class PKMNSIM_API base_pkmn_gen2impl: public base_pkmn
    {
        public:
            base_pkmn_gen2impl(std::string identifier, SQLite::Database *db, bool query_moves);
            std::string get_info(void);
            std::string get_info_verbose(void);
            dict<std::string, int> get_base_stats(void);
            dict<std::string, int> get_ev_yields(void);
            double get_chance_male();
            double get_chance_female();

        private:
            int baseSATK, baseSDEF;
            double chance_male, chance_female;
    };
}

#endif /* INCLUDED_BASE_PKMN_GEN2IMPL_HPP */
