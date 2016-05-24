/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 *
 */
#ifndef INCLUDE_SCOREP_PLUGIN_POLICY_OBJECT_ID_HPP
#define INCLUDE_SCOREP_PLUGIN_POLICY_OBJECT_ID_HPP

#include <scorep/plugin/interface.hpp>

#include <map>
#include <string>
#include <type_traits>
#include <vector>

namespace scorep
{
namespace plugin
{
    namespace policy
    {

        template <typename Handle, typename Plugin, typename Policies>
        class object_id
        {
            static_assert(!std::is_convertible<Handle, std::string>::value,
                          "Handle type must not be convertible to std::string");
            static_assert(!std::is_convertible<Handle, int32_t>::value,
                          "Handle type must not be convertible to int32_t");

        public:
            using handle_type = Handle;

            static void build_info(SCOREP_Metric_Plugin_Info&)
            {
            }

            template <typename... Args>
            Handle& make_handle(const std::string& name, Args&&... args)
            {
                if (name_to_id.count(name) == 0)
                {
                    int32_t id = static_cast<int32_t>(id_to_handle.size());

                    id_to_handle.emplace_back(std::forward<Args>(args)...);

                    name_to_id[name] = id;
                }

                return id_to_handle[name_to_id[name]];
            }

            Handle& get_handle(const std::string& name)
            {
                return id_to_handle[name_to_id.at(name)];
            }

            const std::vector<Handle>& get_handles() const
            {
                return id_to_handle;
            }

        public:
            template <bool Test = false>
            void add_counter(Handle&)
            {
                static_assert(Test, "The object_id policy requires the definition of the method"
                                    "`void add_counter(Handle&)`");
            }

            template <typename Cursor, bool Test = false>
            void get_all_values(Handle&, Cursor&)
            {
                static_assert(Test,
                              "The object_id policy requires the definition of the method"
                              "`template <typename Cursor> void get_all_values(Handle&, Cursor&)`");
            }

            template <typename Proxy, bool Test = false>
            void get_current_value(Handle&, Proxy&)
            {
                static_assert(
                    Test, "The object_id policy requires the definition of the method"
                          "`template <typename Proxy> void get_current_value(Handle&, Proxy&)`");
            }

            template <typename Proxy, bool Test = false>
            void get_optional_value(Handle&, Proxy&)
            {
                static_assert(
                    Test, "The object_id policy requires the definition of the method"
                          "`template <typename Proxy> void get_optional_value(Handle&, Proxy&)`");
            }

        public:
            int32_t add_counter(const std::string& event)
            {
                auto id = name_to_id.at(event);

                Plugin::instance().add_counter(id_to_handle[id]);

                return id;
            }

            template <typename C>
            void get_all_values(int32_t id, C& cursor)
            {
                Plugin::instance().get_all_values(id_to_handle[id], cursor);
            }

            template <typename P>
            void get_current_value(int32_t id, P& proxy)
            {
                Plugin::instance().get_current_value(id_to_handle[id], proxy);
            }

            template <typename P>
            void get_optional_value(int32_t id, P& proxy)
            {
                Plugin::instance().get_optional_value(id_to_handle[id], proxy);
            }

        private:
            std::map<std::string, int32_t> name_to_id;
            std::vector<Handle> id_to_handle;
        };
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_POLICY_OBJECT_ID_HPP
