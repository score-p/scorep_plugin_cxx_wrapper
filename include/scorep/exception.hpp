/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 *
 */
#ifndef INCLUDE_SCOREP_PLUGIN_EXCEPTION_HPP
#define INCLUDE_SCOREP_PLUGIN_EXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace scorep
{
namespace exception
{

    class semantic_error : public std::runtime_error
    {
        using base = std::runtime_error;

    public:
        semantic_error(const std::string& what) : base(what)
        {
        }
    };

    class memory : public std::runtime_error
    {
        using base = std::runtime_error;

    public:
        memory(const std::string& what) : base(what)
        {
        }
    };

    class null_pointer : public std::runtime_error
    {
        using base = std::runtime_error;

    public:
        null_pointer(const std::string& what) : base(what)
        {
        }
    };
}
}

#endif // INCLUDE_SCOREP_PLUGIN_EXCEPTION_HPP
