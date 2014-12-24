/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

/*
 * Macro for pkmn::shared_ptr templates
 */
%define CS_PKMN_SPTR(class_name, sptr_name)
    /*
     * Factory functions need to be associated with newobject
     * since they create a new instance of a type.
     */
    %newobject pkmn :: class_name :: make;

    %template(sptr_name) pkmn::shared_ptr<pkmn :: ## class_name>;
%enddef

%newobject pkmn::team_pokemon::get_base_pokemon;
