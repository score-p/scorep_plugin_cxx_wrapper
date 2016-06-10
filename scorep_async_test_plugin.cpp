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

/*
    This file contains an example plugin written with the Score-P metric plugin C++ wrapper.

    This plugin is an asynchronous metric plugin. It will record values once on a certain thread
    within a certain process.
*/

#include <scorep/plugin/plugin.hpp>

#include <iostream>
#include <string>
#include <vector>

using namespace scorep::plugin::policy;

using scorep::plugin::logging;

class async_plugin : public scorep::plugin::base<async_plugin, async, once, scorep_clock>
{
public:
    using TVPair = std::pair<scorep::chrono::ticks, double>;

    // Convert a named metric (may contain wildcards or so) to a vector of
    // actual metrics (may have a different name)
    std::vector<scorep::plugin::metric_property>
    get_metric_properties(const std::string& metric_name)
    {
        logging::info() << "get metric properties called with: " << metric_name;

        // Must use the same name here as for the handle you made earlier.
        return { scorep::plugin::metric_property(metric_name, "A description of the metric",
                                                 "The unit of the metric")
                     .absolute_point()
                     .value_double() };
    }

    int32_t add_metric(const std::string& event)
    {
        logging::info() << "add metric called with: " << event;

        return 42;
    }

    // start your measurement in this method
    void start()
    {
        begin = scorep::chrono::measurement_clock::now();
    }

    // stop your measurement in this method
    void stop()
    {
        end = scorep::chrono::measurement_clock::now();
    }

    // Will be called post mortem by the measurement environment
    // You return all values measured.
    template <typename C>
    void get_all_values(int32_t id, C& cursor)
    {

        // write the collected data to the cursor.
        for (auto& tvpair : generate_metric_data())
        {
            cursor.write(tvpair.first, tvpair.second);
        }
    }

private:
    std::vector<TVPair> generate_metric_data()
    {
        // gather the data by any means
        // In this case, we just create some meaningless values

        auto my_data = std::vector<TVPair>{};

        // gather the data by any means
        // In this case, we just create some meaningless values
        for (int i = 0; i < 100; i++)
        {
            my_data.emplace_back(scorep::chrono::ticks((end - begin).count() / 100 * i) + begin,
                                 double(i));
        }

        return my_data;
    }

private:
    scorep::chrono::ticks begin, end;
};

SCOREP_METRIC_PLUGIN_CLASS(async_plugin, "async")
