/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_STREAM_HPP
#define INCLUDE_NITRO_LOG_STREAM_HPP

#include <sstream>

#include <memory>

#include <nitro/log/severity.hpp>
#include <nitro/log/detail/set_attribute.hpp>

namespace nitro { namespace log {

    template <typename Record, template <typename> class Formatter, typename Sink, template <typename> class Filter>
    class logger;

    namespace detail {

        template <typename Record, template <typename> class Formatter, typename Sink, template <typename> class Filter, severity_level Severity>
        class smart_stream
        {
            typedef nitro::log::logger<Record, Formatter, Sink, Filter> logger;
        public:

            smart_stream() : r(new Record), s(new std::stringstream())
            {
                detail::set_severity<Record>()(*r, Severity);
            }
            
            smart_stream(smart_stream&& ss) : r(std::move(ss.r)), s(std::move(ss.s))
            {
            }
            
            ~smart_stream()
            {
                if(r)
                {
                    r->message() = s->str();
                    logger::log(*r);
                }
            }
            
            Record& record()
            {
                return *r;
            }
            
            std::stringstream& sstr()
            {
                return *s;
            }
            
        private:
            std::unique_ptr<Record> r;
            std::unique_ptr<std::stringstream> s;
        };


        template <typename Record, template <typename> class Formatter, typename Sink, template <typename> class Filter, typename T, severity_level Severity>
        smart_stream<Record, Formatter, Sink, Filter, Severity> operator<<( smart_stream<Record, Formatter, Sink, Filter, Severity>&& s, const T& t)
        {
            s.sstr() << t;
            
            return std::move(s);
        }

        template <typename Record, template <typename> class Formatter, typename Sink, template <typename> class Filter, typename T, severity_level Severity>
        smart_stream<Record, Formatter, Sink, Filter, Severity>& operator<<( smart_stream<Record, Formatter, Sink, Filter, Severity>& s, const T& t)
        {
            s.sstr() << t;
            
            return s;
        }
        
        class null_stream {};
        
        template <typename T>
        null_stream operator<<(null_stream&& s, const T&) {
            return s;
        }

        template  <bool, typename Record, template <typename> class Formatter, typename Sink, template <typename> class Filter, severity_level Severity>
        struct actual_stream
        {
            typedef smart_stream<Record, Formatter, Sink, Filter, Severity> type;
        };

        template  <typename Record, template <typename> class Formatter, typename Sink, template <typename> class Filter, severity_level Severity>
        struct actual_stream<false, Record, Formatter, Sink, Filter, Severity>
        {
            typedef null_stream type;
        };


    } // namespace detail

    template <severity_level Severity, typename Record, template <typename> class Formatter, typename Sink, template <typename> class Filter>
    struct actual_stream
    {
        typedef typename detail::actual_stream<Severity >= severity_level::NITRO_LOG_MIN_SEVERITY, Record, Formatter, Sink, Filter, Severity>::type type;
    };

}} //namespace nitro::log


#endif //INCLUDE_NITRO_LOG_STREAM_HPP