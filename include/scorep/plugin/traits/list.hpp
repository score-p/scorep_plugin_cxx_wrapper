/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 *
 */
#ifndef INCLUDE_SCOREP_PLUGIN_TRAITS_LIST_HPP
#define INCLUDE_SCOREP_PLUGIN_TRAITS_LIST_HPP

namespace scorep
{
namespace plugin
{
    namespace traits
    {
        template <typename... Elems>
        class list;

        template <template <typename... Args> class... Elems>
        class meta_list;
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_TRAITS_LIST_HPP
