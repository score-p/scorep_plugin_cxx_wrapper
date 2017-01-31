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

#ifndef INCLUDE_SCOREP_PLUGIN_BASE_HPP
#define INCLUDE_SCOREP_PLUGIN_BASE_HPP

#ifndef ENV_CONFIG_PREFIX
#define ENV_CONFIG_PREFIX "SCOREP_METRIC_PLUGIN_"
#endif

#include <scorep/plugin/interface.hpp>
#include <scorep/plugin/metric_property.hpp>
#include <scorep/plugin/policy/fwd.hpp>
#include <scorep/plugin/traits/build_info.hpp>
#include <scorep/plugin/traits/has_all_of.hpp>
#include <scorep/plugin/traits/has_one_of.hpp>
#include <scorep/plugin/traits/has_policy.hpp>
#include <scorep/plugin/traits/static_polymorph_resolve.hpp>

#include <scorep/plugin/log.hpp>
#include <scorep/plugin/util/environment.hpp>
#include <scorep/plugin/util/memory.hpp>
#include <scorep/plugin/util/plugin.hpp>

#include <cassert>
#include <cstdio>
#include <cstring>
#include <memory>
#include <type_traits>
#include <vector>

namespace scorep
{
namespace plugin
{

    template <typename Child, template <typename T, typename Policies> class... Args>
    class base : public Args<Child, traits::meta_list<Args...>>...
    {

        static_assert(
            traits::has_one_of<traits::list<policy::async<Child, traits::meta_list<Args...>>,
                                            policy::sync<Child, traits::meta_list<Args...>>,
                                            policy::sync_strict<Child, traits::meta_list<Args...>>>,
                               traits::list<Args<Child, traits::meta_list<Args...>>...>>::value,
            "A plugin must ether have the async, the sync, or the sync_strict policy!");

        static_assert(
            !traits::has_policy<policy::post_mortem<Child, traits::meta_list<Args...>>,
                                traits::list<Args<Child, traits::meta_list<Args...>>...>>::value ||
                traits::has_all_of<
                    traits::list<policy::async<Child, traits::meta_list<Args...>>,
                                 policy::post_mortem<Child, traits::meta_list<Args...>>>,
                    traits::list<Args<Child, traits::meta_list<Args...>>...>>::value,
            "The post_mortem policy depends on the async policy, which is missing in the plugin!");

        static_assert(
            !traits::has_policy<policy::frequent<Child, traits::meta_list<Args...>>,
                                traits::list<Args<Child, traits::meta_list<Args...>>...>>::value ||
                traits::has_all_of<
                    traits::list<policy::async<Child, traits::meta_list<Args...>>,
                                 policy::frequent<Child, traits::meta_list<Args...>>>,
                    traits::list<Args<Child, traits::meta_list<Args...>>...>>::value,
            "The frequent policy depends on the async policy, which is missing in the plugin!");

        static_assert(
            !traits::has_all_of<traits::list<policy::post_mortem<Child, traits::meta_list<Args...>>,
                                             policy::frequent<Child, traits::meta_list<Args...>>>,
                                traits::list<Args<Child, traits::meta_list<Args...>>...>>::value,
            "The frequent policy and post_mortem policy conflict with each other!");

        static_assert(
            !traits::has_policy<policy::async<Child, traits::meta_list<Args...>>,
                                traits::list<Args<Child, traits::meta_list<Args...>>...>>::value ||
                traits::has_all_of<
                    traits::list<policy::async<Child, traits::meta_list<Args...>>,
                                 policy::scorep_clock<Child, traits::meta_list<Args...>>>,
                    traits::list<Args<Child, traits::meta_list<Args...>>...>>::value,
            "The async policy depends on the scorep_clock policy, which is missing in the plugin!");

        static_assert(
            traits::has_one_of<traits::list<policy::once<Child, traits::meta_list<Args...>>,
                                            policy::per_host<Child, traits::meta_list<Args...>>,
                                            policy::per_process<Child, traits::meta_list<Args...>>,
                                            policy::per_thread<Child, traits::meta_list<Args...>>>,
                               traits::list<Args<Child, traits::meta_list<Args...>>...>>::value,
            "A plugin must ether have the once, the per_host, the per_process, or the per_thread "
            "policy!");

        static_assert(
            !traits::has_all_of<traits::list<policy::synchronize<Child, traits::meta_list<Args...>>,
                                             policy::async<Child, traits::meta_list<Args...>>>,
                                traits::list<Args<Child, traits::meta_list<Args...>>...>>::value,
            "A plugin cannot have both async and synchronize policies. async comes with implicit "
            "synchronize functionality.");

    public:
        template <bool Test = false>
        std::vector<scorep::plugin::metric_property> get_metric_properties(const std::string&)
        {
            static_assert(Test, "The base plugin requires the definition of the method "
                                "`std::vector<scorep::plugin::metric_property> "
                                "get_metric_properties(const std::string&)`");
            return {};
        }

        template <bool Test = false>
        int32_t add_metric(const std::string&)
        {
            static_assert(Test, "The base plugin requires the definition of the method "
                                "`int32_t add_metric(const std::string&)`");

            return 0;
        }

        template <typename Handle, bool Test = false>
        void add_metric(Handle&)
        {
            static_assert(Test, "The object_id policy requires the definition of the method "
                                "`void add_metric(Handle&)`");
        }

        template <typename Handle, typename Cursor, bool Test = false>
        void get_all_values(Handle&, Cursor&)
        {
            static_assert(Test,
                          "The object_id policy requires the definition of the method "
                          "`template <typename Cursor> void get_all_values(Handle&, Cursor&)`");
        }

        template <typename Handle, typename Proxy, bool Test = false>
        void get_current_value(Handle&, Proxy&)
        {
            static_assert(Test,
                          "The object_id policy requires the definition of the method "
                          "`template <typename Proxy> void get_current_value(Handle&, Proxy&)`");
        }

        template <typename Handle, typename Proxy, bool Test = false>
        void get_optional_value(Handle&, Proxy&)
        {
            static_assert(Test,
                          "The object_id policy requires the definition of the method "
                          "`template <typename Proxy> void get_optional_value(Handle&, Proxy&)`");
        }

    public:
        static Child& instance()
        {
            assert(_instance_);
            return *_instance_;
        }

        static bool is_initialized()
        {
            return static_cast<bool>(_instance_);
        }

        static SCOREP_Metric_Plugin_MetricProperties* get_event_info_handler(char* name)
        {
            // In order, that the exception handling works, we have to trust on a few things here.
            // Better we check them!
            // ps: If you trigger this assertions, good luck boy. I'm out. And don't use vtti!
            static_assert(noexcept(std::vector<metric_property>()) == true,
                          "std::vector::vector() constructor must not throw.");
            static_assert(noexcept(std::vector<metric_property>().clear()) == true,
                          "std::vector::clear() must not throw.");

            // This call is noexcept(), so it isn't my fault, if it throws >:O (See above)
            std::vector<metric_property> properties;

            try
            {
                properties = Child::instance().get_metric_properties(std::string(name));
            }
            catch (std::exception& e)
            {
                print_uncaught_exception(e);

                // This call is noexcept(), so it isn't my fault, if it throws >:O
                properties.clear();
            }

            SCOREP_Metric_Plugin_MetricProperties* results;

            try
            {
                results = memory::allocate_c_memory<SCOREP_Metric_Plugin_MetricProperties>(
                    properties.size() + 1);
            }
            catch (std::exception& e)
            {
                // something REALLY bad happend. While trying to allocate the return structure for
                // scorep, there was thrown an exception. Well, what to do now ?:(

                print_uncaught_exception(e);

                // I have to fucking return something, which looks valid. So here we go. EAT THIS!
                return nullptr;
            }

            // copy properties from C++ to C struct
            std::size_t i = 0;
            for (auto& prop : properties)
            {
                auto& result = results[i];

                result.name = strdup(prop.name.c_str());
                result.description = strdup(prop.description.c_str());
                result.unit = strdup(prop.unit.c_str());

                result.mode = prop.mode;
                result.value_type = prop.type;
                result.base = prop.base;
                result.exponent = prop.exponent;

                i++;
            }

            return results;
        }

        static int32_t add_counter_handler(char* event)
        {
            try
            {
                // this is an very complicated way to statically call the correct version of
                // add_metric callback
                return static_cast<typename traits::static_polymorph_resolve<
                    Child, traits::meta_list<Args...>>::type*>(&Child::instance())
                    ->add_metric(std::string(event));
            }
            catch (std::exception& e)
            {
                print_uncaught_exception(e);

                return -1;
            }
        }

        static void print_uncaught_exception(std::exception& e) noexcept
        {
            try
            {
                // that shit actually may throw too, BUT we are already in an exception catch
                scorep::plugin::log::logging::fatal() << "Uncaught exception with message:";
                scorep::plugin::log::logging::fatal() << e.what();
            }
            catch (...)
            {
                // gracefully ignore ALL exceptions
                // and I can't even tell the user something went wrong, nor I can't do my job,
                // print the error message.
                // This here is an uter mess. :'(

                // Let's try a last effort to let the user know, that there went something really
                // badly wrong.
                static const char* message = "WARNING: Something went wrong in a metric plugin, "
                                             "while trying to print an error message.\n"
                                             "I'm almost out of options at this point.\n"
                                             "I wish you all the luck with your trace.\n";
                int res = fputs(message, stderr);

                // I don't care for the result, I'm already falling to death, while being shot and
                // biten by a deadly snake, a deadly spider and my neighbors pet rat. I'm going to
                // die anyway. Add these words to the list of famous last words:
                (void)res;
            }
        }

        static int32_t initialize_handler()
        {
            try
            {
                // initialy set severity level to info, such that the access to VERBOSE isn't logged
                scorep::plugin::log::set_min_severity_level(nitro::log::severity_level::info);
                auto log_verbose = environment_variable::get("VERBOSE", "WARN");
                auto level = severity_from_string(log_verbose, nitro::log::severity_level::info);
                scorep::plugin::log::set_min_severity_level(level);

                // construct plugin instance
                _instance_.reset(new Child());

                return 0;
            }
            catch (std::exception& e)
            {
                print_uncaught_exception(e);

                return -1;
            }
        }

        static void finalize_handler()
        {
            try
            {
                // reset unique_ptr, thus calling destructor of the plugin instance
                Child::_instance_.reset(nullptr);
            }
            catch (std::exception& e)
            {
                print_uncaught_exception(e);

                scorep::plugin::log::logging::fatal() << "WTF?! Some idiot really let the "
                                                         "destructor throw an exception O.O";
                scorep::plugin::log::logging::fatal() << "THIS IS SUPER BAD CODE!";
                scorep::plugin::log::logging::fatal() << "You should REALLY do a git blame on this "
                                                         "and fucking teach the prospect, how NOT "
                                                         "to throw in a destructor...";
            }
        }

        static SCOREP_Metric_Plugin_Info get_info()
        {
            SCOREP_Metric_Plugin_Info info;
            memset(&info, 0, sizeof(SCOREP_Metric_Plugin_Info));

            // first, set my own handlers, so policies can overwrite them
            info.plugin_version = SCOREP_METRIC_PLUGIN_VERSION;
            info.get_event_info = get_event_info_handler;
            info.add_counter = add_counter_handler;

            try
            {
                // ask other policies to insert their plugin info
                traits::build_info<Args<Child, traits::meta_list<Args...>>...>()(info);

                // Check, if initialze and finalize are set by a policy
                // This might not be bad, but it would be hard to do it correct, so it's not allowed
                // per definition.
                if (info.initialize != nullptr || info.finalize != nullptr)
                {
                    scorep::plugin::log::logging::error() << "Another policy tried to set its own "
                                                             "handler for initialize or finalize.";
                    scorep::plugin::log::logging::error() << "This is not allowed per definition.";
                    scorep::plugin::log::logging::info() << "Overwriting with the base class ones.";
                }
            }
            catch (std::exception& e)
            {
                print_uncaught_exception(e);
            }

            // override initialize and finalize handlers
            info.initialize = initialize_handler;
            info.finalize = finalize_handler;

            return info;
        }

    private:
        static std::unique_ptr<Child> _instance_;
    };

    template <typename Child, template <typename T, typename Policies> class... Args>
    std::unique_ptr<Child> base<Child, Args...>::_instance_;
}
}

#define SCOREP_METRIC_PLUGIN_CLASS(CLASS_NAME, PLUGIN_NAME)                                        \
    SCOREP_METRIC_PLUGIN_ENTRY(CLASS_NAME)                                                         \
    {                                                                                              \
        try                                                                                        \
        {                                                                                          \
            scorep::plugin::name() = PLUGIN_NAME;                                                  \
        }                                                                                          \
        catch (std::exception&)                                                                    \
        {                                                                                          \
        }                                                                                          \
        return CLASS_NAME::get_info();                                                             \
    }

#endif // INCLUDE_SCOREP_PLUGIN_BASE_HPP
