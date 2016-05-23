/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_DETAIL_SET_ATTRIBUTE_HPP
#define INCLUDE_NITRO_LOG_DETAIL_SET_ATTRIBUTE_HPP

#include <nitro/log/detail/has_attribute.hpp>
#include <nitro/log/severity.hpp>
#include <nitro/log/attribute/severity.hpp>
#include <nitro/log/record.hpp>

namespace nitro { namespace log { namespace detail {
    
    template <bool, template <typename ...Attributes> class Record, typename ...Attributes> 
    struct assign_severity
    {
        void operator()(Record<Attributes...>& r, const severity_level& v)
        {
            r.severity() = v;
        }
    };
    
    template <typename ...Attributes, template <typename ...Attributes2> class Record> 
    struct assign_severity<false, Record, Attributes...>
    {
        void operator()(Record<Attributes...>&, const severity_level&) {}
    };
    
    
    template <typename ...Attributes>
    struct set_severity;
    
    template <typename ...Attributes>
    struct set_severity<record<Attributes...>>
    {
        void operator()(record<Attributes...>& r, const severity_level& v)
        {
            assign_severity<has_attribute<severity_attribute, record<Attributes...>>::value, record, Attributes...>()(r, v);
        }
    };
    
    
    
}}} //namespace nitro::log::detail

#endif //INCLUDE_NITRO_LOG_DETAIL_SET_ATTRIBUTE_HPP
