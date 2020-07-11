#ifndef BATOBOT_ADJACENTEMPTYFIELDSTRACKER_HPP
#define BATOBOT_ADJACENTEMPTYFIELDSTRACKER_HPP

#include <cstddef>
#include <vector>

#include "Coordinates.hpp"

namespace batobot
{

class AdjacentEmptyFieldsTracker
{
public:

    AdjacentEmptyFieldsTracker(std::size_t boardWidth, std::size_t boardHeight);

    const std::vector<Coordinates>& GetAdjacentFields() const;

private:

    std::vector<Coordinates> m_TrackedFields;
};

}

#endif
