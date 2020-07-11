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
    void RemoveAdjacentIfPresent(Coordinates move);
    void InsertOccupied(Coordinates move);
    bool IsOccupied(Coordinates move) const;

    std::size_t m_BoardWidth;
    std::size_t m_BoardHeight;

    std::vector<Coordinates> m_OccupiedFields;
    std::vector<Coordinates> m_AdjacentFields;
};

}

#endif