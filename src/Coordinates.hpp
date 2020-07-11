#ifndef COORDINATES_HPP_
#define COORDINATES_HPP_

#include <cstddef>

struct Coordinates
{
    std::size_t x = 0;
    std::size_t y = 0;

    constexpr Coordinates() = default;
    constexpr Coordinates(std::size_t x, std::size_t y) :
        x(x),
        y(y)
    {
    }

    constexpr bool operator==(const Coordinates& rhs) const
    {
        return x == rhs.x and y == rhs.y;
    }
};

#endif
