/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_MESSAGE_HOSTNAME_HPP
#define INCLUDE_NITRO_LOG_MESSAGE_HOSTNAME_HPP

#include <string>

extern "C" {
#include <unistd.h>
#include <limits.h>

// For our beloved MAC
#ifndef HOST_NAME_MAX
#ifdef _POSIX_HOST_NAME_MAX
#define HOST_NAME_MAX _POSIX_HOST_NAME_MAX
#else
#define HOST_NAME_MAX 64
#endif
#endif
}

namespace nitro
{
namespace log
{

    class hostname_attribute
    {
        std::string m_hostname;

        static std::string read_hostname()
        {
            char c_hostname[HOST_NAME_MAX + 1];
            if (gethostname(c_hostname, HOST_NAME_MAX + 1))
            {
                throw std::runtime_error("Failed to get local hostname");
            }

            return c_hostname;
        }

    public:
        hostname_attribute() : m_hostname(read_hostname())
        {
        }

        std::string hostname() const
        {
            return m_hostname;
        }
    };
}
} // namespace nitro::log

#endif // INCLUDE_NITRO_LOG_MESSAGE_HOSTNAME_HPP
