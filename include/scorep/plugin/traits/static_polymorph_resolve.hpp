/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 *
 */
#ifndef INCLUDE_SCOREP_PLUGIN_TRAITS_SPR_HPP
#define INCLUDE_SCOREP_PLUGIN_TRAITS_SPR_HPP

#include <scorep/plugin/policy/fwd.hpp>
#include <scorep/plugin/traits/list.hpp>

namespace scorep
{
namespace plugin
{
    namespace traits
    {

        // the sole purpose of the following 100 lines of f***ing meta code is,
        // to save the users of 2 lines of using ....fooo....

        // The problem: Inherited symbols are shadowed, if the derived class
        // declares them again.

        namespace detail
        {

            // <!--- Something SFINEA something, I've got absolutly no idea how
            // that works, but in the end, this is a meta function to check,
            // if a given type has the typedef "handle_type". A plugin will only
            // have this type, if it uses the object_id policy.
            template <typename... Ts>
            using void_t = void;

            template <typename T, typename = void>
            struct has_typedef_handle_type : std::false_type
            {
            };

            template <typename T>
            struct has_typedef_handle_type<T, void_t<typename T::handle_type>>
                : std::true_type
            {
            };
            // ---!>

            // <!---
            // This meta function checks, if a given Policy is the object_id
            // policy. However, it can only do so, if the plugin has the typedef
            // handle_type, which it only has, if it uses the object_id policy.
            // All in all, that may be a bit redundant. Future self, have fun
            // fixing that :-P
            template <bool, typename Plugin,
                      template <typename, typename> class Policy>
            class meta_is_same
            {
            public:
                const static bool value = false;
            };

            template <typename Plugin,
                      template <typename, typename> class Policy>
            class meta_is_same<true, Plugin, Policy>
            {
                template <typename P1, typename P2>
                using tmp_oid =
                    policy::object_id<typename Plugin::handle_type, P1, P2>;

            public:
                const static bool value =
                    std::is_same<Policy<void, traits::meta_list<>>,
                                 tmp_oid<void, traits::meta_list<>>>::value;
            };
            // ---!>

            // <!---
            // This meta function checks, if a list of Policies contains the
            // object_id policy.
            template <typename Plugin, typename Policies>
            class meta_has_oid_policy;

            template <typename Plugin,
                      template <typename, typename> class FirstPolicy,
                      template <typename, typename> class... Policies>
            class meta_has_oid_policy<
                Plugin, traits::meta_list<FirstPolicy, Policies...>>
            {

            public:
                const static bool value =
                    meta_is_same<has_typedef_handle_type<Plugin>::value, Plugin,
                                 FirstPolicy>::value ||
                    meta_has_oid_policy<Plugin,
                                        traits::meta_list<Policies...>>::value;
            };

            template <typename Plugin>
            class meta_has_oid_policy<Plugin, traits::meta_list<>>
            {
            public:
                const static bool value = false;
            };
            // ---!>

            // <!---
            // This meta functions returns the object_id policy, if present in
            // the Policies parameter. Else it returns the Plugin parameter.
            template <typename Plugin, typename Policies, bool>
            class static_polymorph_resolve_dispatch
            {
            public:
                using type = policy::object_id<typename Plugin::handle_type,
                                               Plugin, Policies>;
            };

            template <typename Plugin, typename Policies>
            class static_polymorph_resolve_dispatch<Plugin, Policies, false>
            {
            public:
                using type = Plugin;
            };
            // ---!>
        }

        // This meta functions returns the object_id policy, if present in
        // the Policies parameter. Else it returns the Plugin parameter.
        // It just a dispatch thing here
        template <typename Plugin, typename Policies>
        class static_polymorph_resolve
        {
        public:
            using type = typename detail::static_polymorph_resolve_dispatch<
                Plugin, Policies,
                detail::meta_has_oid_policy<Plugin, Policies>::value>::type;
        };
    }
}
}

#endif // INCLUDE_SCOREP_PLUGIN_TRAITS_SPR_HPP
