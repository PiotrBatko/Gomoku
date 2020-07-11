#include "AdjacentEmptyFieldsTracker.hpp"

#include <algorithm>

namespace batobot
{

AdjacentEmptyFieldsTracker::AdjacentEmptyFieldsTracker(std::size_t, std::size_t)
{
}

void AdjacentEmptyFieldsTracker::RegisterMove(Coordinates move)
{
    const auto InsertMoveIfNotPresent = [this](const Coordinates move)
    {
        const auto MoveIter = std::lower_bound(
            std::begin(m_TrackedFields),
            std::end(m_TrackedFields),
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

        if (MoveIter == std::end(m_TrackedFields))
        {
            m_TrackedFields.push_back(move);
            return;
        }

        if (*MoveIter == move)
        {
            return;
        }

        m_TrackedFields.insert(MoveIter, move);
    };

    InsertMoveIfNotPresent(move.WithOffset(-1, -1));
    InsertMoveIfNotPresent(move.WithOffset( 0, -1));
    InsertMoveIfNotPresent(move.WithOffset(+1, -1));

    InsertMoveIfNotPresent(move.WithOffset(-1,  0));
    InsertMoveIfNotPresent(move.WithOffset(+1,  0));

    InsertMoveIfNotPresent(move.WithOffset(-1, +1));
    InsertMoveIfNotPresent(move.WithOffset( 0, +1));
    InsertMoveIfNotPresent(move.WithOffset(+1, +1));
}

const std::vector<Coordinates>& AdjacentEmptyFieldsTracker::GetAdjacentFields() const
{
    return m_TrackedFields;
}

}
