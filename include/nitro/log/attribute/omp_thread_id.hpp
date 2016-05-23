/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_OMP_THREAD_ID_ATTRIBUTE_HPP
#define INCLUDE_NITRO_LOG_OMP_THREAD_ID_ATTRIBUTE_HPP

#include <omp.h>


namespace nitro { namespace log {
    
    class omp_thread_id_attribute
    {
        int my_omp_thread_id;
        
    public:
        
        omp_thread_id_attribute() : my_omp_thread_id(omp_get_thread_num()) {}

        int omp_thread_id() const
        {
            return my_omp_thread_id;
        }
    };
    
}} //namespace nitro::log

#endif //INCLUDE_NITRO_LOG_OMP_THREAD_ID_ATTRIBUTE_HPP
