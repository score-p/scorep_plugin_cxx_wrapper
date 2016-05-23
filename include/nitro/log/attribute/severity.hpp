/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_SEVERITY_ATTRIBUTE_HPP
#define INCLUDE_NITRO_LOG_SEVERITY_ATTRIBUTE_HPP

#include <string>
#include <nitro/log/severity.hpp>

namespace nitro { namespace log {
    
    class severity_attribute
    {
        severity_level m_severity;
    public:
        typedef severity_level severity_type;
        
        severity_attribute() = default;
        
        severity_type severity() const
        {
            return m_severity;
        }
        
        severity_type& severity()
        {
            return m_severity;
        }
    };
    
}} //namespace nitro::log

#endif //INCLUDE_NITRO_LOG_SEVERITY_ATTRIBUTE_HPP
