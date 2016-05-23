/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 *
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

class async_oid_plugin
    : public scorep::plugin::base<async_oid_plugin, async, once, post_mortem,
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
            my_data.emplace_back(
                scorep::chrono::ticks((end - begin).count() / 100 * i) + begin,
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

    std::vector<scorep::plugin::metric_property>
    get_event_info(const std::string& event)
    {
        logging::info() << "get event info called with: " << event;

        auto handle = make_handle(event + "_hello");

        handle.name = "The hound of Baskerville";

        return { scorep::plugin::metric_property(event, "blablubfasel",
                                                 "GigaUltraFoo")
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
