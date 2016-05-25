/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 *
 */
#ifndef INCLUDE_SCOREP_PLUGIN_POLICY_SYNC_HPP
#define INCLUDE_SCOREP_PLUGIN_POLICY_SYNC_HPP

#include <scorep/plugin/interface.hpp>
#include <scorep/plugin/traits/static_polymorph_resolve.hpp>
#include <scorep/plugin/util/memory.hpp>
#include <scorep/plugin/util/types.hpp>

#include <cstdint>

namespace scorep
{
namespace plugin
{
    namespace policy
    {
        template <typename Plugin, typename Policies>
        class sync
        {
        public:
            static void build_info(SCOREP_Metric_Plugin_Info& info)
            {
                info.sync = SCOREP_METRIC_SYNC;

                info.get_optional_value = get_optional_value_handler;
            }

            template <typename Proxy, bool Test = false>
            bool get_optional_value(std::int32_t, Proxy&)
            {
                static_assert(
                    Test,
                    "The sync policy requires the definition of the method"
                    "`template <typename Proxy> bool get_optional_value(std::int32_t, Proxy&)`");
            }

            static bool get_optional_value_handler(std::int32_t id, uint64_t* value)
            {
                types::proxy p(value);

                static_cast<typename traits::static_polymorph_resolve<Plugin, Policies>::type*>(
                    &Plugin::instance())
                    ->get_optional_value(id, p);

                return static_cast<bool>(p);
            }
        };
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_POLICY_SYNC_HPP
