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
                if (_name_to_id_.count(name) == 0)
                {
                    int32_t id = static_cast<int32_t>(_id_to_handle_.size());

                    _id_to_handle_.emplace_back(std::forward<Args>(args)...);

                    _name_to_id_[name] = id;
                }

                return _id_to_handle_[_name_to_id_[name]];
            }

            Handle& get_handle(const std::string& name)
            {
                return _id_to_handle_[_name_to_id_.at(name)];
            }

            const std::vector<Handle>& get_handles() const
            {
                return _id_to_handle_;
            }

        public:
            template <bool Test = false>
            void add_metric(Handle&)
            {
                static_assert(Test, "The object_id policy requires the definition of the method"
                                    "`void add_metric(Handle&)`");
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
            int32_t add_metric(const std::string& event)
            {
                auto id = _name_to_id_.at(event);

                Plugin::instance().add_metric(_id_to_handle_[id]);

                return id;
            }

            template <typename C>
            void get_all_values(int32_t id, C& cursor)
            {
                Plugin::instance().get_all_values(_id_to_handle_[id], cursor);
            }

            template <typename P>
            void get_current_value(int32_t id, P& proxy)
            {
                Plugin::instance().get_current_value(_id_to_handle_[id], proxy);
            }

            template <typename P>
            void get_optional_value(int32_t id, P& proxy)
            {
                Plugin::instance().get_optional_value(_id_to_handle_[id], proxy);
            }

        private:
            std::map<std::string, int32_t> _name_to_id_;
            std::vector<Handle> _id_to_handle_;
        };
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_POLICY_OBJECT_ID_HPP
