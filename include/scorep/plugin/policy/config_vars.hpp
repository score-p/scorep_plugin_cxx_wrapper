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

#ifndef INCLUDE_SCOREP_PLUGIN_POLICY_CONFIG_VARS_HPP
#define INCLUDE_SCOREP_PLUGIN_POLICY_CONFIG_VARS_HPP

#include <scorep/plugin/interface.hpp>

#include <cstdint>
#include <map>

namespace scorep
{
namespace plugin
{
    namespace policy
    {
        template <typename Plugin, typename Policies>
        class config_vars
        {
            template <bool Test = false>
            static std::map<std::string, std::string> declare_config_vars()
            {
                static_assert(Test, "The config_vars policy requires the definition of the method"
                                    "`static std::uint64_t declare_config_vars()`");

                return {};
            }

        public:
            static void build_info(SCOREP_Metric_Plugin_Info& info)
            {
                const auto vars = Plugin::declare_config_vars();
                if (vars.empty())
                {
                    return;
                }

                try
                {
                    _config_vars_ = memory::allocate_c_memory<SCOREP_MetricConfigVariable>(
                        vars.size() + 1);
                }
                catch (std::exception& e)
                {
                    // something REALLY bad happend. While trying to allocate the return structure for
                    // scorep, there was thrown an exception. Well, what to do now ?:(

                    Plugin::instance().print_uncaught_exception(e);

                    // I have to fucking return something, which looks valid. So here we go. EAT THIS!
                    return;
                }

                std::size_t i = 0;
                for (auto& [name, default_] : vars)
                {
                    auto& var = _config_vars_[i];

                    var.name = strdup(name.c_str());
                    var.value = strdup(default_.c_str());

                    i++;
                }
 
                info.config_variables = _config_vars_;
            }

            static std::map<std::string, std::string> get_config_vars()
            {
                std::map<std::string, std::string> result;
                const SCOREP_MetricConfigVariable* var = _config_vars_;
                while (var && var->name)
                {
                    result.emplace(std::string(var->name), std::string(var->value));
                    var++;
                }
                return result;
            }

        private:
            static SCOREP_MetricConfigVariable* _config_vars_;
        };

        template <typename Plugin, typename Policies>
        SCOREP_MetricConfigVariable* config_vars<Plugin, Policies>::_config_vars_{nullptr};
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_POLICY_CONFIG_VARS_HPP
