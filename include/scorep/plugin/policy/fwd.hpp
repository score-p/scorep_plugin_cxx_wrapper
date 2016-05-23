/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 *
 */
#ifndef INCLUDE_SCOREP_PLUGIN_POLICY_FWD_HPP
#define INCLUDE_SCOREP_PLUGIN_POLICY_FWD_HPP

namespace scorep
{
namespace plugin
{
    namespace policy
    {
        template <typename T, typename Policies>
        class post_mortem;

        template <typename T, typename Policies>
        class async;
        template <typename T, typename Policies>
        class sync;
        template <typename T, typename Policies>
        class sync_strict;

        template <typename T, typename Policies>
        class with_initialize;
        template <typename T, typename Policies>
        class scorep_clock;
        template <typename Handle, typename T, typename Policies>
        class object_id;

        template <typename T, typename Policies>
        class once;
        template <typename T, typename Policies>
        class per_host;
        template <typename T, typename Policies>
        class per_process;
        template <typename T, typename Policies>
        class per_thread;

        template <typename T, typename Policies>
        class synchronize;
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_POLICY_FWD_HPP
