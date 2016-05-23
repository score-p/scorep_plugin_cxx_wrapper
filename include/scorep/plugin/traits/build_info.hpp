/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_SCOREP_PLUGIN_TRAITS_BUILD_INFO_HPP
#define INCLUDE_SCOREP_PLUGIN_TRAITS_BUILD_INFO_HPP

#include <scorep/plugin/interface.hpp>

namespace scorep
{
namespace plugin
{
    namespace traits
    {
        template <typename Arg, typename... Args> class build_info
        {
        public:
            void operator()(SCOREP_Metric_Plugin_Info& info)
            {
                Arg::build_info(info);
                build_info<Args...>()(info);
            }
        };

        template <typename Arg> class build_info<Arg>
        {
        public:
            void operator()(SCOREP_Metric_Plugin_Info& info)
            {
                Arg::build_info(info);
            }
        };
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_TRAITS_BUILD_INFO_HPP