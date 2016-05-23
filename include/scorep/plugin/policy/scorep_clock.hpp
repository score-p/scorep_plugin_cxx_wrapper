/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 *
 */
#ifndef INCLUDE_SCOREP_PLUGIN_POLICY_CLOCK_HPP
#define INCLUDE_SCOREP_PLUGIN_POLICY_CLOCK_HPP

#include <scorep/chrono/chrono.hpp>
#include <scorep/plugin/interface.hpp>

#include <scorep/chrono/measurement_clock.hpp>
#include <scorep/plugin/log.hpp>

#include <cstdint>

namespace scorep
{
namespace plugin
{
    namespace policy
    {

        template <typename Plugin, typename Policies>
        class scorep_clock
        {
        public:
            static void build_info(SCOREP_Metric_Plugin_Info& info)
            {
                info.set_clock_function = set_clock_handler;
            }

            static void set_clock_handler(std::uint64_t (*clock)())
            {
                scorep::plugin::log::wtime_attribute::set_wtime(clock);
                scorep::chrono::measurement_clock::clock_function() = clock;
            }
        };
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_POLICY_CLOCK_HPP
