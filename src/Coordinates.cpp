#include "Coordinates.hpp"

Coordinates::Coordinates(std::size_t x, std::size_t y) :
    x(x),
    y(y)
{
}

bool Coordinates::operator==(const Coordinates& rhs) const
{
    return x == rhs.x and y == rhs.y;
}
