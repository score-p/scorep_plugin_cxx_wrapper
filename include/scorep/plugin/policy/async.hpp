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

#ifndef INCLUDE_SCOREP_PLUGIN_POLICY_ASYNC_HPP
#define INCLUDE_SCOREP_PLUGIN_POLICY_ASYNC_HPP

#include <scorep/chrono/chrono.hpp>
#include <scorep/plugin/interface.hpp>
#include <scorep/plugin/traits/static_polymorph_resolve.hpp>
#include <scorep/plugin/util/memory.hpp>
#include <scorep/plugin/util/types.hpp>

#include <cstdint>

namespace scorep
{
namespace plugin
{
    namespace policy
    {

        template <typename Plugin, typename Policies>
        class async
        {
        public:
            class cursor
            {
                using self = cursor;

            public:
                cursor() = delete;
                cursor(const cursor&) = delete;

                cursor(SCOREP_MetricTimeValuePair*& tvlist, scorep::chrono::ticks begin,
                       scorep::chrono::ticks end)
                : tvlist(tvlist), size_(0), capacity_(0), begin(begin), end(end)
                {
                }

                void resize(std::size_t new_size)
                {
                    memory::resize_c_memory(tvlist, capacity_);
                    capacity_ = new_size;
                }

                template <typename T>
                cursor& operator<<(const T& t)
                {
                    write(t);
                    return *this;
                }

                template <typename T>
                void write(const std::pair<chrono::ticks, T>& p)
                {
                    write(p.first, p.second);
                }

                template <typename T>
                void write(chrono::ticks t, T v)
                {
                    if (!in_range(t))
                        return;

                    if (capacity_ == 0)
                    {
                        resize(1024);
                    }

                    if (size_ == capacity_)
                    {
                        resize(capacity_ * 2);
                    }

                    store(t, v);
                }

                void shrink_to_fit()
                {
                    if (capacity_ > size_)
                    {
                        resize(size_);
                    }
                }

                void store(chrono::ticks t, std::uint64_t v) noexcept
                {
                    tvlist[size_].timestamp = t.count();
                    tvlist[size_].value = v;

                    size_++;
                }

                void store(chrono::ticks t, std::int64_t v) noexcept
                {
                    store(t, scorep::types::convert(v));
                }

                void store(chrono::ticks t, double v) noexcept
                {
                    store(t, scorep::types::convert(v));
                }

                void store(chrono::ticks t, float v) noexcept
                {
                    store(t, static_cast<double>(v));
                }

                std::size_t capacity() const noexcept
                {
                    return capacity_;
                }

                std::size_t size() const noexcept
                {
                    return size_;
                }

                bool in_range(scorep::chrono::ticks t) const
                {
                    return begin <= t && t <= end;
                }

            private:
                SCOREP_MetricTimeValuePair*& tvlist;
                std::size_t size_;
                std::size_t capacity_;
                scorep::chrono::ticks begin;
                scorep::chrono::ticks end;
            };

            static void build_info(SCOREP_Metric_Plugin_Info& info)
            {
                info.sync = SCOREP_METRIC_ASYNC;

                info.get_all_values = get_all_values_handler;
                info.synchronize = synchronize_handler;
            }

            template <bool Test = false>
            void start()
            {
                static_assert(Test, "The async policy requires the definition of the method"
                                    "`void start()`");
            }

            template <bool Test = false>
            void stop()
            {
                static_assert(Test, "The async policy requires the definition of the method"
                                    "`void stop()`");
            }

            template <bool Test = false>
            void synchronize(bool, SCOREP_MetricSynchronizationMode)
            {
                static_assert(Test, "The async policy requires the definition of the method"
                                    "`void synchronize(bool, SCOREP_MetricSynchronizationMode)`");
            }

            template <typename Cursor, bool Test = false>
            void get_all_values(std::int32_t, Cursor&)
            {
                static_assert(
                    Test,
                    "The async policy requires the definition of the method"
                    "`template <typename Cursor> void get_all_values(std::int32_t, Cursor&)`");
            }

            static void synchronize_handler(bool is_responsible,
                                            SCOREP_MetricSynchronizationMode sync_mode)
            {
                try
                {
                    if (sync_mode == SCOREP_METRIC_SYNCHRONIZATION_MODE_BEGIN ||
                        sync_mode == SCOREP_METRIC_SYNCHRONIZATION_MODE_BEGIN_MPP)
                    {
                        if (is_responsible)
                        {
                            Plugin::instance()._async_time_begin_ =
                                scorep::chrono::measurement_clock::now();
                            Plugin::instance().start();
                        }
                    }

                    Plugin::instance().synchronize(is_responsible, sync_mode);

                    if (sync_mode == SCOREP_METRIC_SYNCHRONIZATION_MODE_END)
                    {
                        if (is_responsible)
                        {
                            Plugin::instance().stop();
                            Plugin::instance()._async_time_end_ =
                                scorep::chrono::measurement_clock::now();
                        }
                    }
                }
                catch (std::exception& e)
                {
                    Plugin::instance().print_uncaught_exception(e);
                }
            }

            static std::uint64_t get_all_values_handler(std::int32_t id,
                                                        SCOREP_MetricTimeValuePair** tvlist)
            {
                try
                {
                    // Note: tvlist MUST NOT be freed from the plugin. Scorep takes the ownership
                    //       of this chunk of memory. However, if something bad happens, i.e., an
                    //       exception is thrown, we have to free the memory. Also the memory MUST
                    //       NOT be allocated with new (or freed with delete).
                    *tvlist = nullptr;

                    cursor c(tvlist, Plugin::instance()._async_time_begin_,
                             Plugin::instance()._async_time_end_);

                    static_cast<typename traits::static_polymorph_resolve<Plugin, Policies>::type*>(
                        &Plugin::instance())
                        ->get_all_values(id, c);

                    c.shrink_to_fit();

                    return c.size();
                }
                catch (std::exception& e)
                {
                    if (*tvlist)
                    {
                        free(*tvlist);
                        *tvlist = nullptr;
                    }
                    Plugin::instance().print_uncaught_exception(e);
                    return 0;
                }
            }

            scorep::chrono::ticks _async_time_begin_;
            scorep::chrono::ticks _async_time_end_;
        };
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_POLICY_ASYNC_HPP
