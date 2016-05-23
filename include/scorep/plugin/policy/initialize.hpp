/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 *
 */
#ifndef INCLUDE_SCOREP_PLUGIN_POLICY_INITIALIZE_HPP
#define INCLUDE_SCOREP_PLUGIN_POLICY_INITIALIZE_HPP

#include <scorep/plugin/interface.hpp>

#include <cstdint>

namespace scorep
{
namespace plugin
{
    namespace policy
    {
        template <typename Plugin, typename Policies>
        class with_initialize
        {
        public:
            static void build_info(SCOREP_Metric_Plugin_Info& info)
            {
                info.initialize = initialize_handler;
            }

            static std::int32_t initialize_handler()
            {
                return Plugin::instance().initialize();
            }
        };
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_POLICY_INITIALIZE_HPP
