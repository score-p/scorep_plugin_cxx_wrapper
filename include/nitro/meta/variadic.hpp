/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_META_VARIADIC_HPP
#define INCLUDE_NITRO_META_VARIADIC_HPP

#include <type_traits>

namespace nitro { namespace meta { 

    /**
     * @brief meta function to check if a variadic type pack contains a given type.
     */
    template <typename U, typename ...Attributes> 
    struct is_variadic_member;
    
    /**
     * @brief meta function to check if a variadic type pack contains a given type.
     */
    template <typename U, typename first, typename ...Attributes>
    struct is_variadic_member<U, first, Attributes...>
    {
        /**
         * @brief value contains true if input type is contained in the pack
         */
        static const bool value = std::is_same<U, first>::value || is_variadic_member<U, Attributes...>::value;
    };
    
    /**
     * @brief meta function to check if a variadic type pack contains a given type.
     */
    template <typename U>
    struct is_variadic_member<U>
    {
        /**
         * @brief value contains true if input type is contained in the pack
         */
        static const bool value = false;
    };
    
}} //namespace nitro::meta

#endif //INCLUDE_NITRO_META_VARIADIC_HPP
