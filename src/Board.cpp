#include "Board.hpp"

#include <utility>

#include "DebugInfo.h"

Board::Board(const std::size_t width, const std::size_t height):
    m_Width(width),
    m_Height(height),
    m_Fields(m_Width * m_Height, Field::Empty)
{
}

void Board::SetSize(std::size_t width, std::size_t height)
{
    m_Width = width;
    m_Height = height;
    m_Fields.assign(m_Width * m_Height, Field::Empty);
}

const Field& Board::at(const std::size_t x, const std::size_t y) const
{
    return m_Fields[y * m_Height + x];
}

std::size_t Board::getHeight() const
{
    return m_Height;
}

std::size_t Board::getWidth() const
{
    return m_Width;
}

bool Board::IsFieldOnBoard(const std::size_t x, const std::size_t y) const
{
    if (   0U <= x && x < m_Width
        && 0U <= y && y < m_Height)
    {
        return true;
    }

    return false;
}

bool Board::IsFieldEmpty(const std::size_t x, const std::size_t y, bool& executionStatus, bool logErrorWhenOccured) const
{
    if (!IsFieldOnBoard(x, y))
    {
        executionStatus = false;
        if (logErrorWhenOccured)
        {
            LOG_ERROR("Field ", x, ", ", y, ", is not on board.");
        }
        return false;
    }
    executionStatus = true;

    Field fieldValue = at(x, y);
    if (fieldValue == Field::Empty)
    {
        return true;
    }
    return false;
}

bool Board::SetField(const std::size_t x, const std::size_t y, const Field fieldValueToSet)
{
    if (x > m_Width || y > m_Height)
    {
        LOG_ERROR("x > m_Width || y > m_Height");
        return false;
    }
    m_Fields[y * m_Height + x] = fieldValueToSet;
    return true;
}
