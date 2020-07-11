#include "AdjacentEmptyFieldsTracker.hpp"

#include <algorithm>

namespace batobot
{

AdjacentEmptyFieldsTracker::AdjacentEmptyFieldsTracker(std::size_t, std::size_t)
{
}

void AdjacentEmptyFieldsTracker::RegisterMove(Coordinates move)
{
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

void AdjacentEmptyFieldsTracker::InsertAdjacentIfNotPresent(Coordinates move)
{
    const auto MoveIter = std::lower_bound(
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

}
