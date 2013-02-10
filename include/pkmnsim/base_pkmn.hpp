#ifndef BASE_PKMN_HPP
#define BASE_PKMN_HPP

#include <boost/shared_ptr.hpp>
#include <ciso646>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace pkmnsim
{

    /*!
     * Base Pokemon Class
     *
     * This class generates an object with all of the information for
     * a Pokemon, common to all Pokemon of this species. This information
     * includes name, Pokedex number, height, weight, etc.
     *
     * The information generated for a base Pokemon class is dependent on
     * the "identifier" parameter, which tells the program what to look for
     * in the database, as well as the "gen" parameter, which specifies the
     * generation from which to grab stats. This is important because certain
     * stats were added between generations (Special Attack/Defense, gender,
     * etc.), and some mechanics were completely changed between generations
     * 2-3. Therefore, a Gen 1 Pikachu will be very different than a
     * Gen 3 Pikachu.
     */

    class base_pkmn
    {
        typedef boost::shared_ptr<base_pkmn> sptr;

        public:

            /*!
             * Make a new base Pokemon.
             * \identifier The Pokemon's identifier in the database
             * \param gen This generation's implementation of the Pokemon.
             * \return A new base Pokemon object
            */
            static sptr make(const std::string identifier, const int gen);

            /*!
             * Get a string with basic information on the Pokemon.
             * \return string with basic information on the Pokemon
             */
            virtual std::string get_info(void) = 0;

            /*!
             * Get a string with all information on the Pokemon.
             * \return string with all information on the Pokemon
             */
            virtual std::string get_info_verbose(void) = 0;

            /*!
             * Return Pokemon's display name (e.g. Bulbasaur).
             * \return string with display name
             */
            std::string get_display_name(void) {return display_name;}

            /*!
             * Return Pokemon's National Pokedex number (1-649, as of Gen 5).
             * \return int with National Pokedex number
             */
            int get_nat_pokedex_num(void) {return nat_pokedex_num;}

            /*!
             * Return Pokemon's types.
             * \return list of strings with types
             */
            virtual std::string * get_types(void)
            {
                std::string * types = new std::string[2];
                types[0] = type1;
                types[1] = type2;
                return types;
            }

            /*!
             * Return Pokemon's height (in meters).
             * \return int with Pokemon height
             */
            int get_height(void) {return height;}

            /*!
             * Return Pokemon's weight (in kilograms).
             * \return int with Pokemon weight
             */
            int get_weight(void) {return weight;}

            /*!
             * Return Pokemon's base stats.
             * \return list of ints with base stats
             */
            virtual std::map<std::string,int> get_base_stats(void) = 0;

            /*!
             * Return Pokemon's EV yields.
             * In Gen 1-2, simply calls get_base_stats().
             * \return string with display name
             */
            virtual std::map<std::string,int> get_ev_yields(void) = 0;

        protected:
            std::string display_name;
            int nat_pokedex_num;
            std::string species;
            std::string type1, type2;
            double height; //meters
            double weight; //kilograms
            int baseHP, baseATK, baseDEF, baseSPD; //Base stats
            int exp_yield;

    };
}
#endif
