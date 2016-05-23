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
            class proxy
            {

            public:
                proxy() = delete;
                proxy(const proxy&) = delete;

                proxy(std::uint64_t* value) : value(value)
                {
                }

                void store(std::int64_t v)
                {
                    *value = scorep::types::convert(v);
                    written = true;
                }

                void store(double v)
                {
                    *value = scorep::types::convert(v);
                    written = true;
                }

                void store(std::uint64_t v)
                {
                    *value = v;
                    written = true;
                }

                explicit operator bool() const
                {
                    return written;
                }

            private:
                bool written = false;
                std::uint64_t* value;
            };

        public:
            static void build_info(SCOREP_Metric_Plugin_Info& info)
            {
                info.sync = SCOREP_METRIC_STRICTLY_SYNC;

                info.get_current_value = get_current_value_handler;
            }

            static uint64_t get_current_value_handler(std::int32_t id)
            {
                std::uint64_t result;

                proxy p(&result);

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
