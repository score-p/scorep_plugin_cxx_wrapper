/*
 *   cplus_abstraction_test,
 *      a c++ wrapper for scorep plugins
 *          Copyright (C) 2015 TU Dresden, ZIH
 *
 */

// Must be defined before the include!
#define ENV_CONFIG_PREFIX "SCOREP_METRIC_SINE_PLUGIN_"
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

class sine_plugin
    : public scorep::plugin::base<sine_plugin, spp::per_thread, spp::sync,
                                  spp::scorep_clock, sine_object_id>
{
public:
    // You don't have to do anything in this method, but it tells the plugin
    // that this metric will indeed be used
    void add_counter(sine_metric& m)
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
    std::vector<scorep::plugin::metric_property>
    get_event_info(const std::string& name)
    {
        logging::info() << "get event info called with: " << name;

        if (name == "long")
        {
            // This will create an object of sine_metric with the respective
            // name
            // and constructor arguments that are forwarded
            make_handle(name, 1000000000.0);
        }
        else if (name == "short")
        {
            make_handle(name, 30000000.0);
        }

        // Must use the same name here as for the handle you made earlier... I
        // think.
        return { scorep::plugin::metric_property(name, name + " sine curve",
                                                 "Unit")
                     .absolute_point()
                     .value_double() };
    }
};

SCOREP_METRIC_PLUGIN_CLASS(sine_plugin, "sine")
