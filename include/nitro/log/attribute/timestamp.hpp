/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_TIMESTAMP_ATTRIBUTE_HPP
#define INCLUDE_NITRO_LOG_TIMESTAMP_ATTRIBUTE_HPP

#include <chrono>

namespace nitro { namespace log {
    
    class timestamp_attribute
    {
        std::chrono::nanoseconds my_timestamp;
        
    public:
        
        timestamp_attribute() : my_timestamp(std::chrono::high_resolution_clock::now().time_since_epoch()) {}
        
        std::chrono::nanoseconds timestamp() const
        {
            return my_timestamp;
        }
    };

}} //namespace nitro::log
    
    #endif //INCLUDE_NITRO_LOG_TIMESTAMP_ATTRIBUTE_HPP
    