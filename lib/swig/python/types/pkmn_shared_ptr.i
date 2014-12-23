/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

/*
 * Macro for pkmn::shared_ptr templates
 */
%define PYTHON_PKMN_SPTR(class_name)
    %template(class_name ## _sptr) pkmn::shared_ptr<pkmn :: ## class_name>;
    %pythoncode %{
        class_name = class_name.make;
    %}
%enddef
