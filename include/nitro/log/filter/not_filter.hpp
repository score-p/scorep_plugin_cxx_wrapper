/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_FILTER_NOT_FILTER_HPP
#define INCLUDE_NITRO_LOG_FILTER_NOT_FILTER_HPP

namespace nitro { namespace log { namespace filter {
    
    template <typename F1>
    class not_filter : F1
    {
    public:
        typedef typename F1::record_type record_type;
        
        bool filter(record_type& r) const
        {
            return !F1::filter(r);
        }
    };
    
    template <typename F1>
    class not_filter<not_filter<F1>> : public F1 {};
    
}}} // namespace nitro::log::filter

#endif //INCLUDE_NITRO_LOG_FILTER_NOT_FILTER_HPP