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

#ifndef INCLUDE_SCOREP_CHRONO_TICKS_HPP
#define INCLUDE_SCOREP_CHRONO_TICKS_HPP

#include <cstdint>

namespace scorep
{
namespace chrono
{
    class ticks
    {
    public:
        ticks() : count_(0)
        {
        }

        explicit ticks(std::uint64_t count) : count_(count)
        {
        }

        std::uint64_t count() const noexcept
        {
            return count_;
        }

    private:
        std::uint64_t count_;
    };

    inline ticks operator-(const ticks& a, const ticks& b)
    {
        return ticks(a.count() - b.count());
    }

    inline ticks operator+(const ticks& a, const ticks& b)
    {
        return ticks(a.count() + b.count());
    }

    template <typename T>
    inline ticks operator*(const T& a, const ticks& b)
    {
        return ticks(static_cast<std::uint64_t>(a * b.count()));
    }

    inline bool operator<=(const ticks& a, const ticks& b)
    {
        return a.count() <= b.count();
    }

    inline bool operator<(const ticks& a, const ticks& b)
    {
        return a.count() < b.count();
    }

    inline bool operator>=(const ticks& a, const ticks& b)
    {
        return a.count() >= b.count();
    }

    inline bool operator>(const ticks& a, const ticks& b)
    {
        return a.count() > b.count();
    }

    inline bool operator==(const ticks& a, const ticks& b)
    {
        return a.count() == b.count();
    }

    inline bool operator!=(const ticks& a, const ticks& b)
    {
        return a.count() != b.count();
    }
}
}

#endif // INCLUDE_SCOREP_CHRONO_TICKS_HPP
