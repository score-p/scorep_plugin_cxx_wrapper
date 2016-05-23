/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_LOGGER_HPP
#define INCLUDE_NITRO_LOG_LOGGER_HPP

#include <nitro/log/severity.hpp>
#include <nitro/log/stream.hpp>

namespace nitro { namespace log {

    template <typename Record, template <typename> class Formater, typename Sink, template <typename> class Filter>
    class logger : Sink, Formater<Record>, Filter<Record>
    {
        typedef logger self;
        
        logger() = default;
    public:
        static void log(Record& r)
        {
            static self instance;
            
            if(instance.Filter<Record>::filter(r))
            {            
                instance.Sink::sink(instance.Formater<Record>::format(r));
            }
        }

        static typename actual_stream<severity_level::trace, Record, Formater, Sink, Filter>::type trace()
        {
            return typename actual_stream<severity_level::trace, Record, Formater, Sink, Filter>::type();
        }

        static typename actual_stream<severity_level::debug, Record, Formater, Sink, Filter>::type debug()
        {
            return typename actual_stream<severity_level::debug, Record, Formater, Sink, Filter>::type();
        }

        static typename actual_stream<severity_level::info, Record, Formater, Sink, Filter>::type info()
        {
            return typename actual_stream<severity_level::info, Record, Formater, Sink, Filter>::type();
        }

        static typename actual_stream<severity_level::warn, Record, Formater, Sink, Filter>::type warn()
        {
            return typename actual_stream<severity_level::warn, Record, Formater, Sink, Filter>::type();
        }

        static typename actual_stream<severity_level::error, Record, Formater, Sink, Filter>::type error()
        {
            return typename actual_stream<severity_level::error, Record, Formater, Sink, Filter>::type();
        }

        static typename actual_stream<severity_level::fatal, Record, Formater, Sink, Filter>::type fatal()
        {
            return typename actual_stream<severity_level::fatal, Record, Formater, Sink, Filter>::type();
        }
    };
}} //namespace nitro::log

#endif //INCLUDE_NITRO_LOG_LOGGER_HPP