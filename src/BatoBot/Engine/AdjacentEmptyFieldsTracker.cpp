#include "AdjacentEmptyFieldsTracker.hpp"

namespace batobot
{

AdjacentEmptyFieldsTracker::AdjacentEmptyFieldsTracker(std::size_t, std::size_t)
{
}

const std::vector<Coordinates>& AdjacentEmptyFieldsTracker::GetAdjacentFields() const
{
    return m_TrackedFields;
}

}
