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

#include <scorep/plugin/plugin.hpp>

#include <iostream>
#include <string>
#include <vector>

using namespace scorep::plugin::policy;

using scorep::plugin::logging;

struct foo
{
    std::string name;
};

template <typename T, typename Policies>
using my_object_id = object_id<foo, T, Policies>;

class async_oid_plugin : public scorep::plugin::base<async_oid_plugin, async, once, post_mortem,
                                                     scorep_clock, my_object_id>
{
public:
    using TVPair = std::pair<scorep::chrono::ticks, double>;

    template <typename C>
    void get_all_values(foo& f, C& cursor)
    {
        logging::info() << "get all values called with: " << f.name;

        auto my_data = std::vector<TVPair>{};

        for (int i = 0; i < 100; i++)
        {
            my_data.emplace_back(scorep::chrono::ticks((end - begin).count() / 100 * i) + begin,
                                 double(i));
        }

// if you know what you do,
// then this is fine:
#ifdef DEVELOPER_HAS_A_GLUE_WHATS_GOING_ON
        cursor.resize(my_data.size());

        for (auto& tvpair : my_data)
        {
            cursor.store(tvpair.first, tvpair.second);
        }

// else take this:
#else
        for (auto& tvpair : my_data)
        {
            cursor << tvpair;
        }
#endif
    }

    void add_counter(foo& f)
    {
        logging::info() << "add counter called with: " << f.name;
    }

    std::vector<scorep::plugin::metric_property> get_event_info(const std::string& event)
    {
        logging::info() << "get event info called with: " << event;

        auto handle = make_handle(event + "_hello");

        handle.name = "The hound of Baskerville";

        return { scorep::plugin::metric_property(event, "blablubfasel", "GigaUltraFoo")
                     .absolute_point()
                     .value_double() };
    }

    void start()
    {
        begin = scorep::chrono::measurement_clock::now();
    }

    void stop()
    {
        end = scorep::chrono::measurement_clock::now();
    }

    void synchronize(bool, SCOREP_MetricSynchronizationMode)
    {
    }

private:
    scorep::chrono::ticks begin, end;
};

SCOREP_METRIC_PLUGIN_CLASS(async_oid_plugin, "async")
