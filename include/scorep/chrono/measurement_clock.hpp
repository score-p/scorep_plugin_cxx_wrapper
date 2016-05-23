/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_SCOREP_CHRONO_MEASUREMENT_CLOCK_HPP
#define INCLUDE_SCOREP_CHRONO_MEASUREMENT_CLOCK_HPP

#include <scorep/chrono/ticks.hpp>
#include <scorep/exception.hpp>

namespace scorep
{
namespace chrono
{
    class measurement_clock
    {
        using clock_pointer_t = std::uint64_t (*)();

    public:
        static const bool is_steady = true;

        static scorep::chrono::ticks now() noexcept
        {
            if (clock_function() == nullptr)
            {
                throw scorep::exception::null_pointer(
                    "The measurement clock function pointer wasn't set yet.");
            }
            return ticks(clock_function()());
        }

        static clock_pointer_t& clock_function()
        {
            static clock_pointer_t measurement_clock_;

            return measurement_clock_;
        }
    };
}
}

#endif // INCLUDE_SCOREP_CHRONO_MEASUREMENT_CLOCK_HPP
