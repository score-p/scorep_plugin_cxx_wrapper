/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_FILTER_NULL_FILTER_HPP
#define INCLUDE_NITRO_LOG_FILTER_NULL_FILTER_HPP

namespace nitro { namespace log { namespace filter {
    
    template <typename Record>
    class null_filter
    {
    public:
        typedef Record record_type;
        
        constexpr bool filter(Record&) const
        {
            return true;
        }
    };
    
}}} // namespace nitro::log::filter

#endif //INCLUDE_NITRO_LOG_FILTER_NULL_FILTER_HPP