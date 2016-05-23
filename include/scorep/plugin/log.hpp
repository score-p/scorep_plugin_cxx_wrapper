/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 *
 */
#ifndef INCLUDE_SCOREP_PLUGiN_LOG_HPP
#define INCLUDE_SCOREP_PLUGiN_LOG_HPP

#include <nitro/log/log.hpp>

#include <nitro/log/sink/stderr.hpp>

#include <nitro/log/attribute/hostname.hpp>
#include <nitro/log/attribute/message.hpp>
#include <nitro/log/attribute/severity.hpp>
#include <nitro/log/attribute/timestamp.hpp>

#include <nitro/log/filter/and_filter.hpp>
#include <nitro/log/filter/severity_filter.hpp>

namespace scorep
{
namespace plugin
{
    namespace log
    {

        class wtime_attribute
        {
            std::uint64_t my_timestamp;
            static uint64_t (*wtime_)(void);

        public:
            wtime_attribute()
            {
                if (wtime_ == nullptr)
                {
                    my_timestamp = 0;
                }
                else
                {
                    my_timestamp = wtime_();
                }
            }

            uint64_t wtime() const
            {
                return my_timestamp;
            }

            static void set_wtime(uint64_t (*clock)(void))
            {
                wtime_ = clock;
            }
        };

        uint64_t (*wtime_attribute::wtime_)(void);

        inline std::string& plugin_name()
        {
            static std::string name_;

            return name_;
        }

        namespace detail
        {

            typedef nitro::log::record<nitro::log::message_attribute, wtime_attribute,
                                       nitro::log::hostname_attribute,
                                       nitro::log::severity_attribute>
                record;

            template <typename Record>
            class log_formater
            {
            public:
                std::string format(Record& r)
                {
                    std::stringstream s;

                    s << "Score-P " << plugin_name() << " plugin: [" << r.hostname() << "]["
                      << r.wtime() << "][" << r.severity() << "]: " << r.message();

                    return s.str();
                }
            };

            template <typename Record>
            using log_filter = nitro::log::filter::severity_filter<Record>;
        }

        typedef nitro::log::logger<detail::record, detail::log_formater, nitro::log::sink::stderr,
                                   detail::log_filter>
            logging;

        inline void set_min_severity_level(nitro::log::severity_level sev)
        {
            detail::log_filter<detail::record>::set_severity(sev);
        }

    } // namespace log

    using log::logging;
}
}

#endif // INCLUDE_LOG_HPP
