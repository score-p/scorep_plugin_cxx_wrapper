/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_STD_THREAD_ID_ATTRIBUTE_HPP
#define INCLUDE_NITRO_LOG_STD_THREAD_ID_ATTRIBUTE_HPP

#include <thread>

namespace nitro { namespace log {
    
    class std_thread_id_attribute
    {
        std::thread::id my_std_thread_id;
        
    public:
        
        std_thread_id_attribute() : my_std_thread_id(std::this_thread::get_id()) {}
        
        std::thread::id std_thread_id() const
        {
            return my_std_thread_id;
        }
    };
    
}} //namespace nitro::log

#endif //INCLUDE_NITRO_LOG_STD_THREAD_ID_ATTRIBUTE_HPP
