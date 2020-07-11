#ifndef COORDINATES_HPP_
#define COORDINATES_HPP_

#include <cstddef>

struct Coordinates
{
    std::size_t x = 0;
    std::size_t y = 0;

    Coordinates() = default;
    Coordinates(std::size_t x, std::size_t y);
};

#endif
