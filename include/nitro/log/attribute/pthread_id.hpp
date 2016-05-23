/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_PTHREAD_ID_ATTRIBUTE_HPP
#define INCLUDE_NITRO_LOG_PTHREAD_ID_ATTRIBUTE_HPP

#include <pthread.h>


namespace nitro { namespace log {
    
    class pthread_id_attribute
    {
        pthread_t my_pthread_id;
        
    public:
        
        pthread_id_attribute() : my_pthread_id(pthread_self())  //pthread_getthreadid_np())
        {}
        
        std::uint64_t pthread_id() const
        {
            return reinterpret_cast<std::uint64_t>(my_pthread_id);
        }
    };
    
}} //namespace nitro::log

#endif //INCLUDE_NITRO_LOG_PTHREAD_ID_ATTRIBUTE_HPP
