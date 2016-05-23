/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 *
 */
#ifndef INCLUDE_SCOREP_PLUGIN_UTIL_TYPES_HPP
#define INCLUDE_SCOREP_PLUGIN_UTIL_TYPES_HPP

#include <cstdint>

namespace scorep
{
namespace types
{

    // Note: reinterpret_cast breaks strict aliasing rules
    // A compiler should be smart enough to optimize that away...
    template <typename T>
    static std::uint64_t convert(T v)
    {
        union {
            std::uint64_t uint64;
            T type;
        } union_value;
        union_value.type = v;
        return union_value.uint64;
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_UTIL_TYPES_HPP
