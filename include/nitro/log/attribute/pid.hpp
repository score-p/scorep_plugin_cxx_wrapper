/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_PID_ATTRIBUTE_HPP
#define INCLUDE_NITRO_LOG_PID_ATTRIBUTE_HPP

extern "C" {
#ifdef __APPLE__
    #include <sys/types.h>
    #include <ssp/unistd.h>
#else
    #include <sys/types.h>
    #include <unistd.h>
#endif
}

namespace nitro { namespace log {
    
    class pid_attribute
    {
        int my_pid;
        
    public:
        
        pid_attribute() : my_pid(getpid()) {}

        int pid() const
        {
            return my_pid;
        }
    };
    
}} //namespace nitro::log

#endif //INCLUDE_NITRO_LOG_PID_ATTRIBUTE_HPP
