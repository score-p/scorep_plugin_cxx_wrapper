/*
 *
 *  c_abstraction_test,
 *  a c++ plugin interface for Score-P plugins.
 *  Copyright (C) 2016 TU Dresden, ZIH
 *
 */
#ifndef INCLUDE_SCOREP_PLUGIN_POLICY_SYNCHRONIZE_HPP
#define INCLUDE_SCOREP_PLUGIN_POLICY_SYNCHRONIZE_HPP

#include <scorep/plugin/interface.hpp>

#include <cstdint>
#include <limits>

namespace scorep
{
namespace plugin
{
    namespace policy
    {
        template <typename Plugin, typename Policies>
        class synchronize
        {
        public:
            static void build_info(SCOREP_Metric_Plugin_Info& info)
            {
                info.synchronize = synchronize_handler;
            }

            static void
            synchronize_handler(bool is_responsible,
                                SCOREP_MetricSynchronizationMode sync_mode)
            {
                try
                {
                    Plugin::instance().synchronize(is_responsible, sync_mode);
                }
                catch (std::exception& e)
                {
                    Plugin::instance().print_uncaught_exception(e);
                }
            }
        };
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_POLICY_SYNCHRONIZE_HPP
