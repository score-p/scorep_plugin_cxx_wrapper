/*
 *
 *  c_abstraction_test,
 *  a c++ plugin intereface for Score-P plugins.
 *  Copyright (C) 2015 TU Dresden, ZIH
 * 
 */
#ifndef INCLUDE_SCOREP_CHRONO_TICKS_HPP
#define INCLUDE_SCOREP_CHRONO_TICKS_HPP

#include <cstdint>

namespace scorep
{
namespace chrono
{
    class ticks
    {
    public:
        ticks() : count_(0)
        {
        }

        explicit ticks(std::uint64_t count) : count_(count)
        {
        }

        std::uint64_t count() const
        {
            return count_;
        }

    private:
        std::uint64_t count_;
    };

    inline ticks operator-(const ticks& a, const ticks& b)
    {
        return ticks(a.count() - b.count());
    }

    inline ticks operator+(const ticks& a, const ticks& b)
    {
        return ticks(a.count() + b.count());
    }

    template <typename T>
    inline ticks operator*(const T& a, const ticks& b)
    {
        return ticks(static_cast<std::uint64_t>(a * b.count()));
    }

    inline bool operator<=(const ticks& a, const ticks& b)
    {
        return a.count() <= b.count();
    }

    inline bool operator<(const ticks& a, const ticks& b)
    {
        return a.count() < b.count();
    }

    inline bool operator>=(const ticks& a, const ticks& b)
    {
        return a.count() >= b.count();
    }

    inline bool operator>(const ticks& a, const ticks& b)
    {
        return a.count() > b.count();
    }

    inline bool operator==(const ticks& a, const ticks& b)
    {
        return a.count() == b.count();
    }

    inline bool operator!=(const ticks& a, const ticks& b)
    {
        return a.count() != b.count();
    }
}
}

#endif // INCLUDE_SCOREP_CHRONO_TICKS_HPP
