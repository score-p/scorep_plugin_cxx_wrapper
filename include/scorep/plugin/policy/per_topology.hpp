/*
 * Copyright (c) 2015-2025, Technische Universität Dresden, Germany
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

#ifndef INCLUDE_SCOREP_PLUGIN_POLICY_PER_TOPOLOGY_HPP
#define INCLUDE_SCOREP_PLUGIN_POLICY_PER_TOPOLOGY_HPP

#include <scorep/plugin/interface.hpp>

namespace scorep
{
namespace plugin
{
    #if SCOREP_METRIC_PLUGIN_VERSION >= 2
    using measurement_point = std::pair<uint32_t, const SCOREP_MetricTopologyNode*>;
    #endif

    namespace policy
    {
        template <typename Plugin, typename Policies>
        class per_topology
        {
        public:
            static void build_info(SCOREP_Metric_Plugin_Info& info)
            {
                #if SCOREP_METRIC_PLUGIN_VERSION >= 2
                info.run_per = SCOREP_METRIC_PER_TOPOLOGY;

                // add_counter and add_topology_counters are mutual exclusive
                info.add_counter           = nullptr;
                info.add_topology_counters = add_topology_counters_handler;
                #endif
            }

            #if SCOREP_METRIC_PLUGIN_VERSION >= 2
            template <bool Test = false>
            std::vector<measurement_point> add_topology_metrics(const SCOREP_MetricTopologyNode*, const std::string&)
            {
                static_assert(
                    Test,
                    "The per_topology policy requires the definition of the method `void add_topology_metrics()`");

                return {};
            }

            static int32_t add_topology_counters_handler(const SCOREP_MetricTopologyNode* topologyRoot,
                                                         const char*                      metricName,
                                                         SCOREP_MetricMeasurementPoint**  pointsOut)
            {
                *pointsOut = nullptr;
                std::vector<measurement_point> points;
                try
                {
                    // this is an very complicated way to statically call the correct version of
                    // add_topology_metrics callback
                    points = static_cast<typename traits::static_polymorph_resolve<Plugin, Policies>::type*>(
                        &Plugin::instance())
                        ->add_topology_metrics(topologyRoot, std::string(metricName));
                }
                catch (std::exception& e)
                {
                    Plugin::instance().print_uncaught_exception(e);

                    return -1;
                }

                SCOREP_MetricMeasurementPoint* results;
                try
                {
                    results = memory::allocate_c_memory<SCOREP_MetricMeasurementPoint>(
                        points.size() + 1);
                }
                catch (std::exception& e)
                {
                    // something REALLY bad happend. While trying to allocate the return structure for
                    // scorep, there was thrown an exception. Well, what to do now ?:(

                    Plugin::instance().print_uncaught_exception(e);

                    // I have to fucking return something, which looks valid. So here we go. EAT THIS!
                    return -1;
                }

                // copy points from C++ to C struct
                std::size_t i = 0;
                for (auto& point : points)
                {
                    auto& result = results[i];

                    result.id = point.first;
                    result.scope = point.second;

                    i++;
                }

                *pointsOut = results;
                return points.size();
            }
            #endif
        };
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_POLICY_PER_TOPOLOGY_HPP
