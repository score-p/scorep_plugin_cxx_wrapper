/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 *
 */
#ifndef INCLUDE_SCOREP_PLUGIN_POLICY_SYNC_STRICT_HPP
#define INCLUDE_SCOREP_PLUGIN_POLICY_SYNC_STRICT_HPP

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
        class sync_strict
        {
        public:
            static void build_info(SCOREP_Metric_Plugin_Info& info)
            {
                info.sync = SCOREP_METRIC_STRICTLY_SYNC;

                info.get_current_value = get_current_value_handler;
            }

            template <typename Proxy, bool Test = false>
            uint64_t get_current_value(std::int32_t, Proxy&)
            {
                static_assert(
                    Test,
                    "The sync_strict policy requires the definition of the method"
                    "`template <typename Proxy> uint64_t get_current_value(std::int32_t, Proxy&)`");

                return 0;
            }

            static uint64_t get_current_value_handler(std::int32_t id)
            {
                std::uint64_t result;

                types::proxy p(&result);

                try
                {
                    static_cast<typename traits::static_polymorph_resolve<Plugin, Policies>::type*>(
                        &Plugin::instance())
                        ->get_current_value(id, p);

                    if (!p)
                    {
                        throw exception::semantic_error(
                            "Metric value not written, but plugin is SYNC_STRICT");
                    }
                }
                catch (std::exception& e)
                {
                    Plugin::instance().print_uncaught_exception(e);
                    return 0;
                }

                return result;
            }
        };
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_POLICY_SYNC_STRICT_HPP
