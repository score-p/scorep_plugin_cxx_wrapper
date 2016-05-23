/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_SINK_NULL_HPP
#define INCLUDE_NITRO_LOG_SINK_NULL_HPP

#include <string>

namespace nitro { namespace log { namespace sink {

    class null
    {
    public:
        void sink(std::string){}
    };
    
}}} //namespace nitro::log::sink

#endif //INCLUDE_NITRO_LOG_SINK_STDOUT_HPP