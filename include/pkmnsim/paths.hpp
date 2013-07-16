/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMNSIM_PATHS_HPP
#define INCLUDED_PKMNSIM_PATHS_HPP

#include <string>
#include <vector>

#include <pkmnsim/config.hpp>

namespace pkmnsim
{
    /*
     * Returns the path to the PKMNsim SQLite database.
     */
    std::string PKMNSIM_API get_database_path(void);

    /*
     * Returns the path to the directory where all images are stored.
     */
    std::string PKMNSIM_API get_images_dir(void);

    /*
     * Returns the system's temporary directory.
     */
    std::string PKMNSIM_API get_tmp_dir(void);
}

#endif /* INCLUDED_PKMNSIM_PATHS_HPP */