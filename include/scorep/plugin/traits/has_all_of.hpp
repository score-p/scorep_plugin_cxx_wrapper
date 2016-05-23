/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 *
 */
#ifndef INCLUDE_SCOREP_PLUGIN_TRAITS_HAS_ALL_OF_HPP
#define INCLUDE_SCOREP_PLUGIN_TRAITS_HAS_ALL_OF_HPP

#include <nitro/meta/variadic.hpp>

#include <scorep/plugin/traits/list.hpp>

namespace scorep
{
namespace plugin
{
    namespace traits
    {
        template <typename T, typename U>
        struct has_all_of;

        template <typename What, typename... Whats, typename... Where>
        struct has_all_of<list<What, Whats...>, list<Where...>>
        {
            static const bool value =
                nitro::meta::is_variadic_member<What, Where...>::value &&
                has_all_of<list<Whats...>, list<Where...>>::value;
        };

        template <typename... Where>
        struct has_all_of<list<>, list<Where...>>
        {
            static const bool value = true;
        };
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_TRAITS_HAS_ONE_OF_HPP
