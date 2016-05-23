/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_FILTER_SEVERITY_FILTER_HPP
#define INCLUDE_NITRO_LOG_FILTER_SEVERITY_FILTER_HPP

#include <nitro/log/severity.hpp>

namespace nitro { namespace log { namespace filter {
    
    template <typename Record, unsigned N = 0>
    class severity_filter
    {
    public:
        typedef Record record_type;
        
        static void set_severity(severity_level new_sev)
        {
            sev = new_sev;
        }
        
        static severity_level min_severity()
        {
            return sev;
        }
        
        bool filter(Record& r) const
        {
            return r.severity() >= min_severity();
        }
        
    private:
        static severity_level sev;
    };
    
    template <typename Record, unsigned N>
    severity_level severity_filter<Record, N>::sev = severity_level::trace;
}}} // namespace nitro::log::filter

#endif //INCLUDE_NITRO_LOG_FILTER_SEVERITY_FILTER_HPP