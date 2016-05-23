/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_SCOREP_PLUGIN_HPP
#define INCLUDE_SCOREP_PLUGIN_HPP

#include <string>

#include <scorep/plugin/interface.hpp>

namespace scorep
{
namespace plugin
{

    class metric_property
    {
        using self = metric_property;

    public:
        metric_property() = default;
        metric_property(const std::string& name, const std::string& description,
                        const std::string& unit)
        : name(name), description(description), unit(unit)
        {
        }

        std::string name;
        std::string description;
        std::string unit;

        SCOREP_MetricMode mode = SCOREP_METRIC_MODE_ABSOLUTE_POINT;
        SCOREP_MetricValueType type = SCOREP_METRIC_VALUE_DOUBLE;
        SCOREP_MetricBase base = SCOREP_METRIC_BASE_DECIMAL;
        int64_t exponent = 0;

    public:
        self& absolute_point() noexcept
        {
            mode = SCOREP_METRIC_MODE_ABSOLUTE_POINT;

            return *this;
        }

        self& absolute_last() noexcept
        {
            mode = SCOREP_METRIC_MODE_ABSOLUTE_LAST;

            return *this;
        }

        self& absolute_next() noexcept
        {
            mode = SCOREP_METRIC_MODE_ABSOLUTE_NEXT;

            return *this;
        }

    public:
        self& accumulated_point() noexcept
        {
            mode = SCOREP_METRIC_MODE_ACCUMULATED_POINT;

            return *this;
        }

        self& accumulated_last() noexcept
        {
            mode = SCOREP_METRIC_MODE_ACCUMULATED_LAST;

            return *this;
        }

        self& accumulated_next() noexcept
        {
            mode = SCOREP_METRIC_MODE_ACCUMULATED_NEXT;

            return *this;
        }

        self& accumulated_start() noexcept
        {
            mode = SCOREP_METRIC_MODE_ACCUMULATED_START;

            return *this;
        }


    public:
        self& relative_point() noexcept
        {
            mode = SCOREP_METRIC_MODE_RELATIVE_POINT;

            return *this;
        }

        self& relative_last() noexcept
        {
            mode = SCOREP_METRIC_MODE_RELATIVE_LAST;

            return *this;
        }

        self& relative_next() noexcept
        {
            mode = SCOREP_METRIC_MODE_RELATIVE_NEXT;

            return *this;
        }

    public:
        self& value_double() noexcept
        {
            type = SCOREP_METRIC_VALUE_DOUBLE;

            return *this;
        }

        self& value_int() noexcept
        {
            type = SCOREP_METRIC_VALUE_INT64;

            return *this;
        }

        self& value_uint() noexcept
        {
            type = SCOREP_METRIC_VALUE_UINT64;

            return *this;
        }

    public:
        self& decimal() noexcept
        {
            base = SCOREP_METRIC_BASE_DECIMAL;

            return *this;
        }

        self& binary() noexcept
        {
            base = SCOREP_METRIC_BASE_BINARY;

            return *this;
        }
    };
}
}
#endif // INCLUDE_SCOREP_PLUGIN_HPP
