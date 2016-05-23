/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 *
 */
#ifndef INCLUDE_SCOREP_PLUGIN_UTIL_HPP
#define INCLUDE_SCOREP_PLUGIN_UTIL_HPP

#include <cstdlib>
#include <memory>
#include <string>

#include <scorep/exception.hpp>

namespace scorep
{
namespace memory
{
    template <typename T>
    T* allocate_c_memory(std::size_t count)
    {
        if (count == 0)
        {
            // TODO Add warning here
            return nullptr;
        }

        T* p = static_cast<T*>(calloc(count, sizeof(T)));

        if (p == nullptr)
        {
            // FIXME: what about string literals? -> "..."s
            throw exception::memory("Couldn't allocate memory of size " +
                                    std::to_string(count * sizeof(T)));
        }

        return p;
    }

    template <typename T>
    void resize_c_memory(T*& p, std::size_t count)
    {
        p = static_cast<T*>(realloc(p, sizeof(T) * count));

        if (p == nullptr && count > 0)
        {
            // FIXME: what about string literals? -> "..."s
            throw scorep::exception::memory(
                "Couldn't allocate memory of size " +
                std::to_string(count * sizeof(T)));
        }
    }
}

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

namespace environment
{

    static std::string
    get_global_variable(const std::string& name,
                        const std::string& default_ = std::string(""))
    {
        char* tmp;
        tmp = getenv(name.c_str());

        if (tmp == nullptr)
        {
            return default_;
        }

        return std::string(tmp);
    }

    static std::string
    get_variable(const std::string& name,
                 const std::string& default_ = std::string(""))
    {
        return get_global_variable(std::string(ENV_CONFIG_PREFIX) + name,
                                   default_);
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_UTIL_HPP
