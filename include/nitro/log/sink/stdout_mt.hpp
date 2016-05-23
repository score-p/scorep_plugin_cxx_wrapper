/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_SINK_STDOUT_MT_HPP
#define INCLUDE_NITRO_LOG_SINK_STDOUT_MT_HPP

#include <string>
#include <iostream>
#include <mutex>

namespace nitro { namespace log { namespace sink {
    
    static std::mutex stdout_mutex;
    
    class stdout_mt
    {
    public:
        
        void sink(std::string formatted_record)
        {
            // lock Mutex
            std::lock_guard<std::mutex> my_lock(stdout_mutex);
            
            std::cout << formatted_record << std::endl;
        }
    };
    
    
}}} //namespace nitro::log::sink

#endif //INCLUDE_NITRO_LOG_SINK_STDOUT_HPP
