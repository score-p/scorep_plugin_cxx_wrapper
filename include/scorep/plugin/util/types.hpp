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

    template <typename T>
    inline std::uint64_t convert(T v) noexcept
    {
        // Note: reinterpret_cast breaks strict aliasing rules
        // A compiler should be smart enough to optimize that away...

        union {
            std::uint64_t uint64;
            T type;
        } union_value;
        union_value.type = v;
        return union_value.uint64;
    }

    class proxy
    {
    public:
        proxy() = delete;
        proxy(const proxy&) = delete;

        proxy(std::uint64_t* value) : value(value)
        {
        }

        template <typename T>
        void write(T t)
        {
            store(t);
        }

        void store(std::int64_t v)
        {
            store(convert(v));
        }

        void store(double v)
        {
            store(convert(v));
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
}
}

#endif // INCLUDE_SCOREP_PLUGIN_UTIL_TYPES_HPP
