#ifndef BOARD_HPP_
#define BOARD_HPP_

#include <cstddef>
#include <vector>

#include "Field.hpp"

class Board
{
public:

    Board(std::size_t width, std::size_t height);

    const Field& at(std::size_t x, std::size_t y) const;
    Field& at(std::size_t x, std::size_t y);

private:

    std::size_t m_Width;
    std::size_t m_Height;
    std::vector<Field> m_Fields;
};

#endif
