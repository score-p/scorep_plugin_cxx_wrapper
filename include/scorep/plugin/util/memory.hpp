/*
 * Copyright (c) 2015-2016, Technische Universität Dresden, Germany
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions
 *    and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to
 *    endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
