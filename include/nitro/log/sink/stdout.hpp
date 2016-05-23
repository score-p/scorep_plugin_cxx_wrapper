/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_SINK_STDOUT_HPP
#define INCLUDE_NITRO_LOG_SINK_STDOUT_HPP

#include <string>
#include <iostream>

namespace nitro { namespace log { namespace sink {
    
    class stdout
    {
    public:
        
        void sink(std::string formatted_record)
        {
            std::cout << formatted_record << std::endl;
        }
    };
    
    
}}} //namespace nitro::log::sink

#endif //INCLUDE_NITRO_LOG_SINK_STDOUT_HPP
