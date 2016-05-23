/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 *
 */
#ifndef INCLUDE_SCOREP_PLUGIN_POLICY_POST_MORTEM_HPP
#define INCLUDE_SCOREP_PLUGIN_POLICY_POST_MORTEM_HPP

#include <scorep/plugin/interface.hpp>

#include <cstdint>
#include <limits>

namespace scorep
{
namespace plugin
{
    namespace policy
    {
        template <typename T, typename Policies>
        class post_mortem
        {
        public:
            static void build_info(SCOREP_Metric_Plugin_Info& info)
            {
                info.delta_t = std::numeric_limits<std::uint64_t>::max();
            }
        };
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_POLICY_POST_MORTEM_HPP
