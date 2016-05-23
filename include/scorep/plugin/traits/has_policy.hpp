/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 *
 */
#ifndef INCLUDE_SCOREP_PLUGIN_TRAITS_HAS_POLICY_HPP
#define INCLUDE_SCOREP_PLUGIN_TRAITS_HAS_POLICY_HPP

#include <scorep/plugin/traits/list.hpp>

#include <nitro/meta/variadic.hpp>

namespace scorep
{
namespace plugin
{
    namespace traits
    {
        template <typename T, typename U>
        struct has_policy;

        template <typename What, typename... Where>
        struct has_policy<What, list<Where...>>
        {
            static const bool value =
                nitro::meta::is_variadic_member<What, Where...>::value;
        };
    }
}
}
#endif // INCLUDE_SCOREP_PLUGIN_TRAITS_HAS_POLICY_HPP
