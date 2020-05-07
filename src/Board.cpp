#include "Board.hpp"

#include <utility>

Board::Board(std::size_t width, std::size_t height):
    m_Width(width),
    m_Height(height),
    m_Fields(m_Width * m_Height, Field::Empty)
{

}

const Field& Board::at(std::size_t x, std::size_t y) const
{
    return m_Fields[y * m_Height + x];
}

Field& Board::at(std::size_t x, std::size_t y)
{
    return const_cast<Field&>(
        std::as_const(*this).at(x, y)
    );
}

std::size_t Board::getHeight() const
{
    return m_Height;
}

std::size_t Board::getWidth() const
{
    return m_Width;
}
