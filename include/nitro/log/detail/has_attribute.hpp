/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_DETAIL_HAS_ATTRIBUTE_HPP
#define INCLUDE_NITRO_LOG_DETAIL_HAS_ATTRIBUTE_HPP

#include <nitro/meta/variadic.hpp>

namespace nitro { namespace log { namespace detail {
    
    template <typename ...Attributes> 
    struct has_attribute;
    
    
    template <typename Attribute, typename ...Attributes, template <typename ...Attributes2> class Record> 
    struct has_attribute<Attribute, Record<Attributes...>>
    {
        static const bool value = nitro::meta::is_variadic_member<Attribute, Attributes...>::value;
    };

    
    
}}} //namespace nitro::log::detail

#endif //INCLUDE_NITRO_LOG_DETAIL_HAS_ATTRIBUTE_HPP
