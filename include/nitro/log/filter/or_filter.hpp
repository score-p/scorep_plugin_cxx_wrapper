/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_FILTER_OR_FILTER_HPP
#define INCLUDE_NITRO_LOG_FILTER_OR_FILTER_HPP

namespace nitro { namespace log { namespace filter {
    
    template <typename F1, typename F2>
    class or_filter : F1, F2
    {
    public:
        typedef typename F1::record_type record_type;
        
        static_assert(std::is_same<typename F1::record_type, typename F2::record_type>::value, "record_type must match for both filters");
        
        bool filter(record_type& r) const
        {
            return F1::filter(r) || F2::filter(r);
        }
    };
    
}}} // namespace nitro::log::filter

#endif //INCLUDE_NITRO_LOG_FILTER_OR_FILTER_HPP