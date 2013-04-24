/*
 * Copyright (c) 2013 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_VLA_IPP
#define INCLUDED_VLA_IPP

#include <boost/format.hpp>
#include <stdexcept>

namespace pkmnsim
{
    template<typename item_type>
    vla<item_type>::vla () {}

    template<typename item_type>
    vla<item_type>::vla(std::string name, int items)
    {
        _vec.clear();
        nice_name = name;
        max_items = items;

        for(int i = 0; i < items; i++) _vec.push_back(*(new item_type()));

        full_vec_err_msg = str(boost::format("List of %ss can only have %d members.")
                                             % nice_name % max_items);
        invalid_pos_err_msg = str(boost::format("Position must be 0-%d.")
                                                % (max_items-1));
        empty_vec_err_msg = str(boost::format("Cannot get %s from empty list.")
                                              % nice_name);
    }

    template<typename item_type>
    void vla<item_type>::put(item_type item)
    {
        if(_vec.size() < max_items) _vec.push_back(item);
        else throw std::runtime_error(full_vec_err_msg);
    }

    template<typename item_type>
    void vla<item_type>::put(item_type item, int pos)
    {
        if(pos > _vec.size()-1) throw std::runtime_error(invalid_pos_err_msg);
        else if(_vec.size() >= max_items) throw std::runtime_error(full_vec_err_msg);
        else _vec.insert(_vec.begin()+pos, item);
    }

    template<typename item_type>
    item_type vla<item_type>::get(int pos)
    {
        if(_vec.begin() == _vec.end()) throw std::runtime_error(empty_vec_err_msg);
        else if(pos > _vec.size()-1) throw std::runtime_error(invalid_pos_err_msg);
        else return _vec[pos];
    }

    template<typename item_type>
    item_type vla<item_type>::get_lead()
    {
        if(_vec.begin() == _vec.end()) throw std::runtime_error(empty_vec_err_msg);
        else return _vec[0];
    }

    template<typename item_type>
    void vla<item_type>::remove(int pos)
    {
        if(_vec.begin() == _vec.end()) throw std::runtime_error(empty_vec_err_msg);
        else if(pos > _vec.size()-1) throw std::runtime_error(invalid_pos_err_msg);
        else _vec.remove(_vec.begin()+pos);
    }
}

#endif /* INCLUDED_VLA_IPP */