/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 *
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

        return std::string("SCOREP_METRIC_") + upper_name + "_PLUGIN_";
    }

    inline std::string get_variable(const std::string& name,
                                    const std::string& default_ = std::string(""))
    {
        return get_global_variable(plugin_prefix() + name, default_);
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_UTIL_ENVIRONMENT_HPP
