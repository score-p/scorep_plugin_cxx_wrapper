/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_NITRO_LOG_LOG_HPP
#define INCLUDE_NITRO_LOG_LOG_HPP

#include <nitro/log/severity.hpp>
#include <type_traits>
    
#ifndef NITRO_LOG_MIN_SEVERITY
    #define NITRO_LOG_MIN_SEVERITY trace
#else
static_assert(std::is_same< decltype(nitro::log::severity_level::NITRO_LOG_MIN_SEVERITY), nitro::log::severity_level >::value, "NITRO_LOG_MIN_SEVERITY has to be of type nitro::log::severity_level");
#endif

#include <nitro/log/record.hpp>
#include <nitro/log/logger.hpp>

#endif //INCLUDE_NITRO_LOG_LOG_HPP
