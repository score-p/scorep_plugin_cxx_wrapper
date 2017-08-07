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
        metric_property(std::string name, std::string description = "", std::string unit = "")
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

        self& value_exponent(int64_t new_exponent) noexcept
        {
            exponent = new_exponent;

            return *this;
        }
    };
}
}
#endif // INCLUDE_SCOREP_PLUGIN_HPP
