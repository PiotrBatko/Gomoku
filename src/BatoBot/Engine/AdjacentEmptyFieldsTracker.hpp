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

    void RegisterMove(Coordinates move);
    const std::vector<Coordinates>& GetAdjacentFields() const;

private:

    std::vector<Coordinates>::iterator FindAdjacent(Coordinates move);
    void InsertAdjacentIfNotPresent(Coordinates move);

    std::vector<Coordinates> m_AdjacentFields;
};

}

#endif
