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

#ifndef INCLUDE_SCOREP_PLUGIN_UTIL_ENVIRONMENT_HPP
#define INCLUDE_SCOREP_PLUGIN_UTIL_ENVIRONMENT_HPP

#include <mutex>
#include <string>

#include <scorep/plugin/log.hpp>

namespace scorep
{
namespace environment
{

    inline std::string get_global_variable(const std::string& name,
                                           const std::string& default_ = std::string(""))
    {
        // Note: This lock may not be sufficient, if application / plugin uses an exotic thread
        //       paradigm. However, in that case, all you can do is sacrifice a goat, prey and hope
        //       for the best.
        static std::mutex getenv_mutex;
        std::lock_guard<std::mutex> my_lock(getenv_mutex);

        char* tmp;
        tmp = getenv(name.c_str());

        scorep::plugin::logging::debug() << "Access to environment variable '" << name << "'";

        if (tmp == nullptr)
        {
            return default_;
        }

        return std::string(tmp);
    }

    inline std::string& plugin_name()
    {
        static std::string name_;
        return name_;
    }

    inline std::string plugin_prefix()
    {
        std::string upper_name = plugin_name();

        for (auto& c : upper_name)
            c = toupper(c);

        return std::string("SCOREP_METRIC_") + upper_name + "_";
    }

    inline std::string get_variable_name(const std::string& name)
    {
        return plugin_prefix() + name;
    }

    inline std::string get_variable(const std::string& name,
                                    const std::string& default_ = std::string(""))
    {
        return get_global_variable(get_variable_name(name), default_);
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_UTIL_ENVIRONMENT_HPP
