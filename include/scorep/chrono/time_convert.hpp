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

#ifndef INCLUDE_SCOREP_CHRONO_TIME_CONVERT_HPP
#define INCLUDE_SCOREP_CHRONO_TIME_CONVERT_HPP

#include <scorep/chrono/measurement_clock.hpp>
#include <scorep/chrono/ticks.hpp>

#include <scorep/exception.hpp>

#include <chrono>
#include <type_traits>

namespace scorep
{
namespace chrono
{

    template <typename Clock = std::chrono::system_clock>
    class time_convert
    {
        // Rationale: Plugins can assume, that the used clock here is a UNIX timestamp.
        //            That behaviour is fairly hard to check. However, at this time, every to me
        //            known clock representing a unix timestamp isn't steady. Thus, the following
        //            static_assert() asserts on that. If you poor bastard, ever trigger this
        //            assertion, I wish you all the luck and god (or whomever you are believing in)
        //            may bless you.
        static_assert(std::is_same<Clock, std::chrono::system_clock>::value && !Clock::is_steady,
                      "Fly you fools!");

        using local_duration_t = typename Clock::time_point::duration;
        using local_time_point_t = typename Clock::time_point;

        enum class state
        {
            init_called,
            synchronized_once,
            tick_rate_set
        };

        void set_tick_rate(local_time_point_t local_stop, scorep::chrono::ticks scorep_stop)
        {
            const auto local_duration = local_stop - local_start_;
            const auto scorep_duration = scorep_stop - scorep_start_;
            tick_rate_ = static_cast<double>(scorep_duration.count()) / local_duration.count();

            state_ = state::tick_rate_set;
        }

    public:
        time_convert() = default;

        time_convert(local_time_point_t local_start, local_time_point_t local_stop,
                     scorep::chrono::ticks scorep_start, scorep::chrono::ticks scorep_stop)
        : local_start_(local_start), scorep_start_(scorep_start)
        {
            set_tick_rate(local_stop, scorep_stop);
        }

        void synchronize_point(local_time_point_t local_tp = Clock::now(),
                               scorep::chrono::ticks scorep_tp = measurement_clock::now())
        {
            if (state_ == state::init_called)
            {
                local_start_ = local_tp;
                scorep_start_ = scorep_tp;

                state_ = state::synchronized_once;
            }
            else if (state_ == state::synchronized_once)
            {
                set_tick_rate(local_tp, scorep_tp);
            }
            else
            {
                exception::raise(
                    "Trying to add synchronize time point, when already fully synchronized.");
            }
        }

        bool is_synchronized() const noexcept
        {
            return state_ == state::tick_rate_set;
        }

    public:
        template <typename T>
        scorep::chrono::ticks to_ticks(const T duration) const
        {
            if (!is_synchronized())
            {
                exception::raise("Trying to convert, when NOT yet synchronized.");
            }

            return to_ticks(std::chrono::duration_cast<local_duration_t>(duration));
        }

        scorep::chrono::ticks to_ticks(const local_duration_t duration) const
        {
            if (!is_synchronized())
            {
                exception::raise("Trying to convert, when NOT yet synchronized.");
            }

            return scorep::chrono::ticks(duration.count() * tick_rate_);
        }

        scorep::chrono::ticks to_ticks(const local_time_point_t tp) const
        {
            if (!is_synchronized())
            {
                exception::raise("Trying to convert, when NOT yet synchronized.");
            }

            const auto tp_offset = tp - local_start_;
            return scorep_start_ + to_ticks(tp_offset);
        }

    private:
        local_time_point_t local_start_;
        scorep::chrono::ticks scorep_start_;
        double tick_rate_;
        state state_ = state::init_called;
    };
}
}

#endif // INCLUDE_SCOREP_CHRONO_TIME_CONVERT_HPP
