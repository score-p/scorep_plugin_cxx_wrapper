/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#include <scorep/plugin/plugin.hpp>

#include <string>
#include <vector>
#include <iostream>

using namespace scorep::plugin::policy;

using scorep::plugin::logging;

class async_plugin : public scorep::plugin::base<async_plugin, async, once,
                                                 post_mortem, scorep_clock>
{
public:
    using TVPair = std::pair<scorep::chrono::ticks, double>;

    template <typename C>
    void get_all_values(int32_t id, C& cursor)
    {

        logging::info() << "get all values called with: " << id;

        end = scorep::chrono::measurement_clock::now();

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
            cursor.write(tvpair.first, tvpair.second);
        }
#endif
    }

    int32_t add_counter(const std::string& event)
    {
        logging::info() << "add counter called with: " << event;

        begin = scorep::chrono::measurement_clock::now();

        return 42;
    }

    std::vector<scorep::plugin::metric_property>
    get_event_info(const std::string& event)
    {
        logging::info() << "get event info called with: " << event;

        return { scorep::plugin::metric_property(event, "blablubfasel",
                                                 "GigaUltraFoo")
                     .absolute_point()
                     .value_double() };
    }

    void start()
    {
    }

    void stop()
    {
    }

    void synchronize(bool, SCOREP_MetricSynchronizationMode)
    {
    }

private:
    scorep::chrono::ticks begin, end;
};

SCOREP_METRIC_PLUGIN_CLASS(async_plugin, "async")
