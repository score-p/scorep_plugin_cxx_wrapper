/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 *
 */
#ifndef INCLUDE_SCOREP_PLUGIN_UTIL_MEMORY_HPP
#define INCLUDE_SCOREP_PLUGIN_UTIL_MEMORY_HPP

#include <cstdlib>
#include <memory>
#include <string>

#include <scorep/exception.hpp>
#include <scorep/plugin/log.hpp>

using namespace std::literals::string_literals;

namespace scorep
{
namespace memory
{
    template <typename T>
    T* allocate_c_memory(std::size_t count)
    {
        if (count == 0)
        {
            plugin::logging::warn()
                << "Trying to allocate a chunk of memory for zero items. That sounds fishy to me!";
            return nullptr;
        }

        T* p = static_cast<T*>(calloc(count, sizeof(T)));

        if (p == nullptr)
        {
            // Note: It might be not as smart as it seems, to work with non-static strings here, as
            //       if the "malloc" fails, constructing a string may also fail. However, there
            //       isn't much to do anyways in that case.
            throw exception::memory("Couldn't allocate memory of size "s +
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
            // Note: It might be not as smart as it seems, to work with non-static strings here, as
            //       if the "malloc" fails, constructing a string may also fail. However, there
            //       isn't much to do anyways in that case.
            throw exception::memory("Couldn't allocate memory of size "s +
                                    std::to_string(count * sizeof(T)));
        }
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_UTIL_MEMORY_HPP
