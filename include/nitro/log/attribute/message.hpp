/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_MESSAGE_ATTRIBUTE_HPP
#define INCLUDE_NITRO_LOG_MESSAGE_ATTRIBUTE_HPP
 
#include <string>

namespace nitro { namespace log {

    class message_attribute
    {
        std::string m_message;
    public:
        
        
        message_attribute() = default;
        
        std::string message() const
        {
            return m_message;
        }
        
        std::string& message()
        {
            return m_message;
        }
    };

}} //namespace nitro::log

#endif //INCLUDE_NITRO_LOG_MESSAGE_ATTRIBUTE_HPP
