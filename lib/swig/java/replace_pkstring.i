/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

/*
 * TODO: automate this
 */

%extend pkmn::bag {
    static sptr pkmn::bag::make(const std::wstring &input)
    {
        return pkmn::bag::make(pkmn::pkstring(input));
    }

    std::wstring getGame()
    {
        return self->get_game();
    }

    void addItem(const std::wstring &input, unsigned int amount)
    {
        self->add_item(input, amount);
    }

    void removeItem(const std::wstring &input, unsigned int amount)
    {
        self->remove_item(input, amount);
    }

    unsigned int getItemAmount(const std::wstring &input)
    {
        return self->get_item_amount(input);
    }

    pocket::sptr getPocket(const std::wstring &input)
    {
        return self->get_pocket(input);
    }

    pkmn::dict<std::string, pocket::sptr> getPockets()
    {
        pkmn::dict<pkmn::pkstring, pkmn::pocket::sptr> pkpockets = self->get_pockets();
        std::vector<pkmn::pkstring> keys = pkpockets.keys();

        pkmn::dict<std::string, pkmn::pocket::sptr> pockets;
        for(size_t i = 0; i < keys.size(); i++)
        {
            pockets[keys[i]] = pkpockets[keys[i]];
        }

        return pockets;
    }
};

%ignore pkmn::bag::make(const pkmn::pkstring&);
%ignore pkmn::bag::get_game() const;
%ignore pkmn::bag::add_item(const pkmn::pkstring&, unsigned int);
%ignore pkmn::bag::remove_item(const pkmn::pkstring&, unsigned int);
%ignore pkmn::bag::get_item_amount(const pkmn::pkstring&) const;
%ignore pkmn::bag::get_pocket(const pkmn::pkstring&) const;
%ignore pkmn::bag::get_pockets() const;

//////////////////////////////////////////////

%extend pkmn::base_pokemon {
    static sptr pkmn::base_pokemon::make(const std::wstring &species, const std::wstring &game)
    {
        return pkmn::base_pokemon::make(pkmn::pkstring(species), pkmn::pkstring(game));
    }

    std::wstring getGame()
    {
        return self->get_game();
    }

    std::wstring getName()
    {
        return self->get_name();
    }

    std::wstring getSpecies()
    {
        return self->get_species();
    }

    std::wstring getPokedexEntry()
    {
        return self->get_pokedex_entry();
    }

    std::pair<std::wstring, std::wstring> getTypes()
    {
        std::pair<pkmn::pkstring, pkmn::pkstring> pktypes = self->get_types();
        std::pair<std::wstring, std::wstring> types;

        types.first = pktypes.first;
        types.second = pktypes.second;

        return types;
    }

    std::pair<std::wstring, std::wstring> getAbilities()
    {
        std::pair<pkmn::pkstring, pkmn::pkstring> pkabilities = self->get_abilities();
        std::pair<std::wstring, std::wstring> abilities;

        abilities.first = pkabilities.first;
        abilities.second = pkabilities.second;

        return abilities;
    }

    void getEggGroups(std::vector<std::wstring> &vec)
    {
        vec.clear();
        std::vector<pkmn::pkstring> pkvec;
        self->get_egg_groups(pkvec);

        for(uint8_t i = 0; i < pkvec.size(); i++) vec.push_back(pkvec[i]);
    }

    std::wstring getHiddenAbility()
    {
        return self->get_hidden_ability();
    }

    pkmn::dict<std::string, int> getBaseStats()
    {
        pkmn::dict<pkmn::pkstring, unsigned int> pkstats = self->get_base_stats();
        std::vector<pkmn::pkstring> keys = pkstats.keys();
        std::vector<unsigned int> vals = pkstats.vals();

        pkmn::dict<std::string, int> stats;
        for(uint8_t i = 0; i < keys.size(); i++)
        {
            stats[keys[i]] = vals[i];
        }

        return stats;
    }

    pkmn::dict<std::string, int> getEVYields()
    {
        pkmn::dict<pkmn::pkstring, unsigned int> pkstats = self->get_ev_yields();
        std::vector<pkmn::pkstring> keys = pkstats.keys();
        std::vector<unsigned int> vals = pkstats.vals();

        pkmn::dict<std::string, int> stats;
        for(uint8_t i = 0; i < keys.size(); i++)
        {
            stats[keys[i]] = vals[i];
        }

        return stats;
    }

    std::wstring getForm()
    {
        return self->get_form();
    }

    void setForm(const std::wstring &form)
    {
        self->set_form(form);
    }
};

%ignore pkmn::base_pokemon::make(const pkmn::pkstring&, const pkmn::pkstring&);
%ignore pkmn::base_pokemon::get_game() const;
%ignore pkmn::base_pokemon::get_name() const;
%ignore pkmn::base_pokemon::get_species() const;
%ignore pkmn::base_pokemon::get_pokedex_entry() const;
%ignore pkmn::base_pokemon::get_types() const;
%ignore pkmn::base_pokemon::get_abilities() const;
%ignore pkmn::base_pokemon::get_egg_groups(pkmn::pkstring_vector_t&) const;
%ignore pkmn::base_pokemon::get_hidden_ability() const;
%ignore pkmn::base_pokemon::get_base_stats() const;
%ignore pkmn::base_pokemon::get_ev_yields() const;
%ignore pkmn::base_pokemon::get_form() const;
%ignore pkmn::base_pokemon::set_form(const pkmn::pkstring&);

//////////////////////////////////////////////

%extend pkmn::game_save {
    std::wstring getTrainerName()
    {
        return self->get_trainer_name().std_wstring();
    }

    void setTrainerName(const std::wstring &input)
    {
        pkmn::pkstring trainer_name(input);
        self->set_trainer_name(trainer_name);
    }
};

%ignore pkmn::game_save::get_trainer_name() const;
%ignore pkmn::game_save::set_trainer_name(const pkmn::pkstring&);

//////////////////////////////////////////////

%extend pkmn::item {
    static sptr pkmn::item::make(const std::wstring &input, const std::wstring &game)
    {
        return pkmn::item::make(pkmn::pkstring(input), pkmn::pkstring(game));
    }

    std::wstring getGame()
    {
        return self->get_game();
    }

    std::wstring getName()
    {
        return self->get_name();
    }

    std::wstring getDescription()
    {
        return self->get_description();
    }

    std::wstring getCategory()
    {
        return self->get_category();
    }
};

%ignore pkmn::item::make(const pkmn::pkstring&, const pkmn::pkstring&);
%ignore pkmn::item::get_game() const;
%ignore pkmn::item::get_name() const;
%ignore pkmn::item::get_description() const;
%ignore pkmn::item::get_category() const;

//////////////////////////////////////////////

%extend pkmn::move {
    static sptr pkmn::move::make(const std::wstring &input, const std::wstring &game)
    {
        return pkmn::move::make(pkmn::pkstring(input), pkmn::pkstring(game));
    }

    std::wstring getGame()
    {
        return self->get_game();
    }

    std::wstring getName()
    {
        return self->get_name();
    }

    std::wstring getDescription()
    {
        return self->get_description();
    }

    std::wstring getType()
    {
        return self->get_type();
    }

    std::wstring getMoveDamageClass()
    {
        return self->get_move_damage_class();
    }

    std::wstring getEffect()
    {
        return self->get_effect();
    }

    std::wstring getTarget()
    {
        return self->get_target();
    }
};

%ignore pkmn::move::make(const pkmn::pkstring&, const pkmn::pkstring&);
%ignore pkmn::move::get_game() const;
%ignore pkmn::move::get_name() const;
%ignore pkmn::move::get_description() const;
%ignore pkmn::move::get_type() const;
%ignore pkmn::move::get_move_damage_class() const;
%ignore pkmn::move::get_effect() const;
%ignore pkmn::move::get_target() const;

//////////////////////////////////////////////

%extend pkmn::pocket {
    static sptr pkmn::pocket::make(int &game, const std::wstring &name, int size)
    {
        return pkmn::pocket::make(game, pkmn::pkstring(name), size);
    }

    static sptr pkmn::pocket::make(const std::wstring &game, const std::wstring &name, int size)
    {
        return pkmn::pocket::make(pkmn::pkstring(game), pkmn::pkstring(name), size);
    }

    std::wstring getGame()
    {
        return self->get_game();
    }

    std::wstring getName()
    {
        return self->get_name();
    }

    void addItem(const std::wstring &input, unsigned int amount)
    {
        self->add_item(input, amount);
    }

    void removeItem(const std::wstring &input, unsigned int amount)
    {
        self->remove_item(input, amount);
    }

    unsigned int getItemAmount(const std::wstring &input)
    {
        return self->get_item_amount(input);
    }
};

%ignore pkmn::pocket::make(unsigned int, const pkmn::pkstring&, unsigned int);
%ignore pkmn::pocket::make(const pkmn::pkstring&, const pkmn::pkstring&, unsigned int);
%ignore pkmn::pocket::get_game() const;
%ignore pkmn::pocket::get_name() const;
%ignore pkmn::pocket::add_item(const pkmn::pkstring&, unsigned int);
%ignore pkmn::pocket::remove_item(const pkmn::pkstring&, unsigned int);
%ignore pkmn::pocket::get_item_amount(const pkmn::pkstring&) const;

//////////////////////////////////////////////

%extend pkmn::team_pokemon {
    static sptr pkmn::team_pokemon::make(const std::wstring &name, const std::wstring &game, int level,
                                         const std::wstring &move1, const std::wstring &move2,
                                         const std::wstring &move3, const std::wstring &move4)
    {
        return pkmn::team_pokemon::make(pkmn::pkstring(name), pkmn::pkstring(game), level,
                                        pkmn::pkstring(move1), pkmn::pkstring(move2),
                                        pkmn::pkstring(move3), pkmn::pkstring(move4));
    }

    std::wstring getGame()
    {
        return self->get_game();
    }

    std::wstring getSpeciesName() const
    {
        return self->get_species_name();
    }

    std::wstring getOriginalGame() const
    {
        return self->get_original_game();
    }

    std::wstring getNickname() const
    {
        return self->get_nickname();
    }

    std::pair<std::wstring, std::wstring> getTypes()
    {
        std::pair<pkmn::pkstring, pkmn::pkstring> pktypes = self->get_types();
        std::pair<std::wstring, std::wstring> types;

        types.first = pktypes.first;
        types.second = pktypes.second;

        return types;
    }

    pkmn::dict<std::string, int> getBaseStats()
    {
        pkmn::dict<pkmn::pkstring, unsigned int> pkstats = self->get_base_pokemon()->get_base_stats();

        std::vector<pkmn::pkstring> keys = pkstats.keys();
        std::vector<unsigned int> vals = pkstats.vals();

        pkmn::dict<std::string, int> stats;
        for(uint8_t i = 0; i < keys.size(); i++)
        {
            stats[keys[i]] = vals[i];
        }

        return stats;
    }

    std::wstring getTrainerName() const
    {
        return self->get_trainer_name();
    }

    std::wstring getTrainerGender() const
    {
        return self->get_trainer_gender();
    }

    std::wstring getBall() const
    {
        return self->get_ball();
    }

    void setOriginalGame(const std::wstring &input)
    {
        self->set_original_game(input);
    }

    void setNickname(const std::wstring &input)
    {
        self->set_nickname(input);
    }

    void setTrainerName(const std::wstring &input)
    {
        self->set_trainer_name(input);
    }

    void setTrainerGender(const std::wstring &input)
    {
        self->set_trainer_gender(input);
    }

    void setBall(const std::wstring &input)
    {
        self->set_ball(input);
    }

    std::wstring getGender()
    {
        return self->get_gender();
    }

    std::wstring getAbility()
    {
        return self->get_ability();
    }

    pkmn::dict<std::string, int> getStats()
    {
        pkmn::dict<pkmn::pkstring, unsigned int> pkstats = self->get_stats();
        std::vector<pkmn::pkstring> keys = pkstats.keys();
        std::vector<unsigned int> vals = pkstats.vals();

        pkmn::dict<std::string, int> stats;
        for(uint8_t i = 0; i < keys.size(); i++)
        {
            stats[keys[i]] = vals[i];
        }

        return stats;
    }

    pkmn::dict<std::string, int> getEVs()
    {
        pkmn::dict<pkmn::pkstring, unsigned int> pkstats = self->get_EVs();
        std::vector<pkmn::pkstring> keys = pkstats.keys();
        std::vector<unsigned int> vals = pkstats.vals();

        pkmn::dict<std::string, int> stats;
        for(uint8_t i = 0; i < keys.size(); i++)
        {
            stats[keys[i]] = vals[i];
        }

        return stats;
    }

    pkmn::dict<std::string, int> getIVs()
    {
        pkmn::dict<pkmn::pkstring, unsigned int> pkstats = self->get_IVs();
        std::vector<pkmn::pkstring> keys = pkstats.keys();
        std::vector<unsigned int> vals = pkstats.vals();

        pkmn::dict<std::string, int> stats;
        for(uint8_t i = 0; i < keys.size(); i++)
        {
            stats[keys[i]] = vals[i];
        }

        return stats;
    }

    void setNature(const std::wstring &input)
    {
        self->set_nature(input);
    }

    void setAbility(const std::wstring &input)
    {
        self->set_ability(input);
    }

    void setEV(const std::wstring &input, int value)
    {
        self->set_EV(input, value);
    }

    void setIV(const std::wstring &input, int value)
    {
        self->set_IV(input, value);
    }

    std::wstring getStatus() const
    {
        return self->get_status();
    }

    void setStatus(const std::wstring &input)
    {
        self->set_status(input);
    }

    void setHeldItem(const std::wstring &input)
    {
        self->set_held_item(input);
    }

    void setMove(const std::wstring &name, size_t pos)
    {
        self->set_move(name, pos);
    }

    int getAttribute(const std::wstring &input)
    {
        return self->get_attribute(input);
    }

    pkmn::dict<std::string, int> getAttributes()
    {
        pkmn::dict<pkmn::pkstring, int> pkattributes = self->get_attributes();
        std::vector<pkmn::pkstring> keys = pkattributes.keys();
        std::vector<int> vals = pkattributes.vals();

        pkmn::dict<std::string, int> attributes;
        for(size_t i = 0; i < keys.size(); i++)
        {
            attributes[keys[i]] = vals[i];
        }

        return attributes;
    }

    bool hasAttribute(const std::wstring &input)
    {
        return self->has_attribute(input);
    }

    void setAttribute(const std::wstring &key, int value)
    {
        self->set_attribute(key, value);
    }

    void setForm(const std::wstring &input)
    {
        self->set_form(input);
    }
};

%ignore pkmn::team_pokemon::make(const pkmn::pkstring&, const pkmn::pkstring&, unsigned int,
                                 const pkmn::pkstring&, const pkmn::pkstring&,
                                 const pkmn::pkstring&, const pkmn::pkstring&);
%ignore pkmn::team_pokemon::get_game() const;
%ignore pkmn::team_pokemon::get_species_name() const;
%ignore pkmn::team_pokemon::get_original_game() const;
%ignore pkmn::team_pokemon::get_nickname() const;
%ignore pkmn::team_pokemon::get_types() const;
%ignore pkmn::team_pokemon::get_base_stats() const;
%ignore pkmn::team_pokemon::get_trainer_name() const;
%ignore pkmn::team_pokemon::get_trainer_gender() const;
%ignore pkmn::team_pokemon::get_ball() const;
%ignore pkmn::team_pokemon::set_original_game(const pkmn::pkstring&);
%ignore pkmn::team_pokemon::set_nickname(const pkmn::pkstring&);
%ignore pkmn::team_pokemon::set_trainer_name(const pkmn::pkstring&);
%ignore pkmn::team_pokemon::set_trainer_gender(const pkmn::pkstring&);
%ignore pkmn::team_pokemon::set_ball(const pkmn::pkstring&);
%ignore pkmn::team_pokemon::get_gender() const;
%ignore pkmn::team_pokemon::get_ability() const;
%ignore pkmn::team_pokemon::get_stats() const;
%ignore pkmn::team_pokemon::get_EVs() const;
%ignore pkmn::team_pokemon::get_IVs() const;
%ignore pkmn::team_pokemon::set_nature(const pkmn::pkstring&);
%ignore pkmn::team_pokemon::set_ability(const pkmn::pkstring&);
%ignore pkmn::team_pokemon::set_EV(const pkmn::pkstring&, unsigned int);
%ignore pkmn::team_pokemon::set_IV(const pkmn::pkstring&, unsigned int);
%ignore pkmn::team_pokemon::get_status() const;
%ignore pkmn::team_pokemon::set_status(const pkmn::pkstring&);
%ignore pkmn::team_pokemon::set_held_item(const pkmn::pkstring&);
%ignore pkmn::team_pokemon::set_move(const pkmn::pkstring&, unsigned int);
%ignore pkmn::team_pokemon::get_attribute(const pkmn::pkstring&) const;
%ignore pkmn::team_pokemon::get_attributes() const;
%ignore pkmn::team_pokemon::has_attribute(const pkmn::pkstring&) const;
%ignore pkmn::team_pokemon::set_attribute(const pkmn::pkstring&, int);
%ignore pkmn::team_pokemon::set_form(const pkmn::pkstring&);

//////////////////////////////////////////////

%extend pkmn::trainer {
    static sptr pkmn::trainer::make(unsigned int game, std::wstring input, unsigned int gender)
    {
        pkmn::pkstring name(input);
        return pkmn::trainer::make(game, name, gender);
    }

    static sptr pkmn::trainer::make(const std::wstring &game, const std::wstring &input, const std::wstring &gender)
    {
        pkmn::pkstring name(input);
        return pkmn::trainer::make(game, name, gender);
    }

    std::wstring getGame()
    {
        return pkmn::pkstring(self->get_game());
    }

    std::wstring getName()
    {
        return pkmn::pkstring(self->get_name());
    }

    std::wstring getGender()
    {
        return pkmn::pkstring(self->get_gender());
    }

    void setName(const std::wstring &name)
    {
        self->set_name(pkmn::pkstring(name));
    }

    void setGender(const std::wstring &gender)
    {
        self->set_gender(pkmn::pkstring(gender));
    }
};

%ignore pkmn::trainer::make(unsigned int, const pkmn::pkstring&, unsigned int);
%ignore pkmn::trainer::make(const pkmn::pkstring&, const pkmn::pkstring&, const pkmn::pkstring&);
%ignore pkmn::trainer::get_game() const;
%ignore pkmn::trainer::get_name() const;
%ignore pkmn::trainer::get_gender() const;
%ignore pkmn::trainer::set_name(const pkmn::pkstring&);
%ignore pkmn::trainer::set_gender(const pkmn::pkstring&);
