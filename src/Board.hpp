#ifndef BOARD_HPP_
#define BOARD_HPP_

#include <cstddef>
#include <vector>

#include "Field.hpp"

// For the whole Board class:
// 'x' - column index,
// 'y' - row index.
class Board
{
public:

    Board() = default;
    Board(std::size_t width, std::size_t height);

    void SetSize(std::size_t width, std::size_t height);

    const Field& at(std::size_t x, std::size_t y) const;

    std::size_t getHeight() const;
    std::size_t getWidth() const;

    bool IsFieldOnBoard(std::size_t x, std::size_t y) const;
    bool IsFieldEmpty(std::size_t x, std::size_t y, bool& executionStatus, bool logErrorWhenOccured = true) const;

    bool SetField(std::size_t x, std::size_t y, Field fieldValueToSet);

private:

    std::size_t m_Width = 0;
    std::size_t m_Height = 0;
    std::vector<Field> m_Fields;
};

#endif
