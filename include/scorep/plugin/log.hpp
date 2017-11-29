/*
 * Copyright (c) 2015-2016, Technische Universität Dresden, Germany
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions
 *    and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to
 *    endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

#include <scorep/plugin/util/plugin.hpp>

namespace scorep
{
namespace plugin
{
    namespace log
    {

        class wtime_attribute
        {
            std::uint64_t my_timestamp;
            using clock_pointer_t = std::uint64_t (*)();

        public:
            wtime_attribute()
            {
                if (wtime_ptr() == nullptr)
                {
                    my_timestamp = 0;
                }
                else
                {
                    my_timestamp = wtime_ptr()();
                }
            }

            uint64_t wtime() const
            {
                return my_timestamp;
            }

            static clock_pointer_t& wtime_ptr()
            {
                static uint64_t (*clock)(void);
                return clock;
            }
        };

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

                    s << "Score-P " << scorep::plugin::name() << " plugin: [" << r.hostname()
                      << "][" << r.wtime() << "][" << r.severity() << "]: " << r.message();

                    return s.str();
                }
            };

            template <typename Record>
            using log_filter = nitro::log::filter::severity_filter<Record>;
        }

        typedef nitro::log::logger<detail::record, detail::log_formater, nitro::log::sink::StdErr,
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
