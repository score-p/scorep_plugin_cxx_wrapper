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
#include <scorep/util.hpp>

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
                info.sync = SCOREP_METRIC_SYNC;

                info.get_optional_value = get_optional_value_handler;
            }

            static bool get_optional_value_handler(std::int32_t id,
                                                   uint64_t* value)
            {
                proxy p(value);

                static_cast<typename traits::static_polymorph_resolve<
                    Plugin, Policies>::type*>(&Plugin::instance())
                    ->get_optional_value(id, p);

                return static_cast<bool>(p);
            }
        };
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_POLICY_SYNC_HPP
