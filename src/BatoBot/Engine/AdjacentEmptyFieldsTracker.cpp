#include "AdjacentEmptyFieldsTracker.hpp"

namespace batobot
{

AdjacentEmptyFieldsTracker::AdjacentEmptyFieldsTracker(std::size_t, std::size_t)
{
}

void AdjacentEmptyFieldsTracker::RegisterMove(Coordinates move)
{
    m_TrackedFields.resize(8);
}

const std::vector<Coordinates>& AdjacentEmptyFieldsTracker::GetAdjacentFields() const
{
    return m_TrackedFields;
}

}
