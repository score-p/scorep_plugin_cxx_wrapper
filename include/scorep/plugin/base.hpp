/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 *
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

#include <cassert>
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
            "A plugin cannot have both async and synchroinze policies. async comes with implicit "
            "synchronize functionality.");

        template <bool Test = false>
        std::vector<scorep::plugin::metric_property> get_event_info(const std::string&)
        {
            static_assert(Test, "The base plugin requires the definition of the method "
                                "`std::vector<scorep::plugin::metric_property> "
                                "get_event_info(const std::string&)`");
            return {};
        }

        template <bool Test = false>
        int32_t add_counter(const std::string&)
        {
            static_assert(Test, "The base plugin requires the definition of the method "
                                "`int32_t add_counter(const std::string&)`");

            return 0;
        }

    public:
        static Child& instance()
        {
            assert(instance_);
            return *instance_;
        }

        static SCOREP_Metric_Plugin_MetricProperties* get_event_info_handler(char* name)
        {
            std::vector<metric_property> properties;

            try
            {
                properties = Child::instance().get_event_info(std::string(name));
            }
            catch (std::exception& e)
            {
                properties.clear();
                print_uncaught_exception(e);
            }

            SCOREP_Metric_Plugin_MetricProperties* results =
                memory::allocate_c_memory<SCOREP_Metric_Plugin_MetricProperties>(properties.size() +
                                                                                 1);

            std::size_t i = 0;

            for (auto& prop : properties)
            {
                auto& result = results[i];

                // FIXME: possible memory leak
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
                return static_cast<typename traits::static_polymorph_resolve<
                    Child, traits::meta_list<Args...>>::type*>(&Child::instance())
                    ->add_counter(std::string(event));
            }
            catch (std::exception& e)
            {
                print_uncaught_exception(e);

                return -1;
            }
        }

        static void print_uncaught_exception(std::exception& e)
        {
            scorep::plugin::log::logging::fatal() << "Uncaught exception with message:";
            scorep::plugin::log::logging::fatal() << e.what();
        }

        static int32_t initialize_handler()
        {
            // initialize class instance and catch possible exceptions
            try
            {
                auto log_verbose = environment::get_variable("VERBOSE", "WARN");
                auto level = severity_from_string(log_verbose, nitro::log::severity_level::info);
                scorep::plugin::log::set_min_severity_level(level);

                instance_.reset(new Child());

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
            Child::instance_.reset(nullptr);
        }

        static SCOREP_Metric_Plugin_Info get_info()
        {
            SCOREP_Metric_Plugin_Info info;
            memset(&info, 0, sizeof(SCOREP_Metric_Plugin_Info));

            info.plugin_version = SCOREP_METRIC_PLUGIN_VERSION;

            // <!---technically this is not required
            info.initialize = initialize_handler;
            info.finalize = finalize_handler;
            info.get_event_info = get_event_info_handler;
            info.add_counter = add_counter_handler;
            // info.get_current_value = nullptr;
            // info.get_optional_value = nullptr;
            // info.get_all_values = nullptr;
            // info.set_clock_function = nullptr;
            // --->

            traits::build_info<Args<Child, traits::meta_list<Args...>>...>()(info);

            return info;
        }

    private:
        static std::unique_ptr<Child> instance_;
    };

    template <typename Child, template <typename T, typename Policies> class... Args>
    std::unique_ptr<Child> base<Child, Args...>::instance_;
}
}

#define SCOREP_METRIC_PLUGIN_CLASS(CLASS_NAME, PLUGIN_NAME)                                        \
    SCOREP_METRIC_PLUGIN_ENTRY(CLASS_NAME)                                                         \
    {                                                                                              \
        scorep::plugin::log::plugin_name() = PLUGIN_NAME;                                          \
        scorep::environment::plugin_name() = PLUGIN_NAME;                                          \
        return CLASS_NAME::get_info();                                                             \
    }

#endif // INCLUDE_SCOREP_PLUGIN_BASE_HPP
