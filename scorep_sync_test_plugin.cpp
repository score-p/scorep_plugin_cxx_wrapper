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

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

namespace spp = scorep::plugin::policy;
using scorep_clock = scorep::chrono::measurement_clock;
using scorep::plugin::logging;

// Our metric handle.
struct sine_metric
{
    sine_metric(double i) : interval(i)
    {
    }

    double value() const
    {
        return sin(scorep_clock::now().count() / interval);
    }

    double interval;
};

template <typename T, typename P>
using sine_object_id = spp::object_id<sine_metric, T, P>;

class sine_plugin : public scorep::plugin::base<sine_plugin, spp::per_thread, spp::sync,
                                                spp::scorep_clock, sine_object_id>
{
public:
    // You don't have to do anything in this method, but it tells the plugin
    // that this metric will indeed be used
    void add_metric(sine_metric& m)
    {
        logging::info() << "adding sine metric with interval " << m.interval;
    }

    // Will be called for every event in by the measurement environment
    // You may or may or may not give it a value here.
    template <typename P>
    void get_optional_value(sine_metric& m, P& proxy)
    {
        auto value = m.value();
        if (value >= 0)
        {
            proxy.store(value);
        }
    }

    // Convert a named metric (may contain wildcards or so) to a vector of
    // actual metrics (may have a different name)
    std::vector<scorep::plugin::metric_property> get_metric_properties(const std::string& name)
    {
        logging::info() << "get metric properties called with: " << name;

        if (name == "long")
        {
            // This will create an object of sine_metric with the respective name
            // and constructor arguments that are forwarded
            make_handle(name, 1000000000.0);
        }
        else if (name == "short")
        {
            make_handle(name, 30000000.0);
        }

        // Must use the same name here as for the handle you made earlier.
        return { scorep::plugin::metric_property(name, name + " sine curve", "Unit")
                     .absolute_point()
                     .value_double() };
    }
};

SCOREP_METRIC_PLUGIN_CLASS(sine_plugin, "sine")
