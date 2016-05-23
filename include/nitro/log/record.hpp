/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_RECORD_HPP
#define INCLUDE_NITRO_LOG_RECORD_HPP

#include <nitro/log/detail/has_attribute.hpp>
#include <nitro/log/attribute/message.hpp>

namespace nitro { namespace log {

    template <typename ...Attributes>
    class record : public Attributes...
    {
        static_assert(detail::has_attribute<message_attribute, record>::value,"Record must have a message attribute!");
    };
        
}} // namespace nitro::log
    
#endif //INCLUDE_NITRO_LOG_RECORD_HPP
