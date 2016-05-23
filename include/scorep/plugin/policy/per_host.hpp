/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 *
 */
#ifndef INCLUDE_SCOREP_PLUGIN_POLICY_PER_HOST_HPP
#define INCLUDE_SCOREP_PLUGIN_POLICY_PER_HOST_HPP

#include <scorep/plugin/interface.hpp>

namespace scorep
{
namespace plugin
{
    namespace policy
    {
        template <typename Plugin, typename Policies>
        class per_host
        {
        public:
            static void build_info(SCOREP_Metric_Plugin_Info& info)
            {
                info.run_per = SCOREP_METRIC_PER_HOST;
            }
        };
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_POLICY_PER_HOST_HPP
