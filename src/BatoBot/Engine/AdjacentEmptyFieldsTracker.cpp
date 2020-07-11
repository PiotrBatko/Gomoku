#include "AdjacentEmptyFieldsTracker.hpp"

#include <algorithm>
#include <cassert>

namespace batobot
{

AdjacentEmptyFieldsTracker::AdjacentEmptyFieldsTracker(std::size_t boardWidth, std::size_t boardHeight) :
     m_BoardWidth(boardWidth),
     m_BoardHeight(boardHeight)
{
}

void AdjacentEmptyFieldsTracker::RegisterMove(Coordinates move)
{
    assert(not IsOccupied(move));
    RemoveAdjacentIfPresent(move);
    InsertOccupied(move);

    InsertAdjacentIfNotPresent(move.WithOffset(-1, -1));
    InsertAdjacentIfNotPresent(move.WithOffset( 0, -1));
    InsertAdjacentIfNotPresent(move.WithOffset(+1, -1));

    InsertAdjacentIfNotPresent(move.WithOffset(-1,  0));
    InsertAdjacentIfNotPresent(move.WithOffset(+1,  0));

    InsertAdjacentIfNotPresent(move.WithOffset(-1, +1));
    InsertAdjacentIfNotPresent(move.WithOffset( 0, +1));
    InsertAdjacentIfNotPresent(move.WithOffset(+1, +1));
}

const std::vector<Coordinates>& AdjacentEmptyFieldsTracker::GetAdjacentFields() const
{
    return m_AdjacentFields;
}

std::vector<Coordinates>::iterator AdjacentEmptyFieldsTracker::FindAdjacent(Coordinates move)
{
    return std::lower_bound(
        std::begin(m_AdjacentFields),
        std::end(m_AdjacentFields),
        move,
        [](const Coordinates& lhs, const Coordinates& rhs)
        {
            if (lhs.y < rhs.y)
            {
                return true;
            }
            else if (lhs.y > rhs.y)
            {
                return false;
            }

            if (lhs.x < rhs.x)
            {
                return true;
            }

            return false;
        }
    );
}

void AdjacentEmptyFieldsTracker::InsertAdjacentIfNotPresent(Coordinates move)
{
    if (IsOccupied(move))
    {
        return;
    }

    const auto MoveIter = FindAdjacent(move);

    if (MoveIter == std::end(m_AdjacentFields))
    {
        m_AdjacentFields.push_back(move);
        return;
    }

    if (*MoveIter == move)
    {
        return;
    }

    m_AdjacentFields.insert(MoveIter, move);
}

void AdjacentEmptyFieldsTracker::InsertOccupied(Coordinates move)
{
    m_OccupiedFields.push_back(move);
}

void AdjacentEmptyFieldsTracker::RemoveAdjacentIfPresent(Coordinates move)
{
    const auto FoundMove = FindAdjacent(move);

    if (FoundMove == std::end(m_AdjacentFields))
    {
        return;
    }

    if (*FoundMove == move)
    {
        m_AdjacentFields.erase(FoundMove);
    }
}

bool AdjacentEmptyFieldsTracker::IsOccupied(Coordinates move) const
{
    return std::find(
        std::begin(m_OccupiedFields),
        std::end(m_OccupiedFields),
        move
    ) != std::end(m_OccupiedFields);
}

}
