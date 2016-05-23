/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_SEVERITY_HPP
#define INCLUDE_NITRO_LOG_SEVERITY_HPP

#include <string>
#include <algorithm>

namespace nitro { namespace log {

    enum class severity_level : char
    {
        trace,
        debug,
        info,
        warn,
        error,
        fatal
    };

    inline severity_level severity_from_string(std::string text, severity_level default_)
    {
        std::transform(text.begin(), text.end(), text.begin(), ::toupper);
        if (text == "FATAL")
        {
            return severity_level::fatal;
        }
        if (text == "ERROR")
        {
            return severity_level::error;
        }
        if (text == "WARN")
        {
            return severity_level::warn;
        }
        if (text == "INFO")
        {
            return severity_level::info;
        }
        if (text == "DEBUG")
        {
            return severity_level::debug;
        }
        if (text == "TRACE")
        {
            return severity_level::trace;
        }
        return default_;
    }
    
    template<typename S>
    S& operator<<(S& s, severity_level sev)
    {
        switch(sev)
        {
            case severity_level::debug:
                s << "DEBUG";
                break;
                
            case severity_level::trace:
                s << "TRACE";
                break;
                
            case severity_level::info:
                s << " INFO";
                break;
                
            case severity_level::warn:
                s << " WARN";
                break;
                
            case severity_level::error:
                s << "ERROR";
                break;
                
            case severity_level::fatal:
                s << "FATAL";
                break;
                
        }
        
        return s;
    }
    
}} //namespace nitro::log


#endif //INCLUDE_NITRO_LOG_SEVERITY_HPP