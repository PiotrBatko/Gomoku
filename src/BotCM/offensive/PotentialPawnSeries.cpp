#include "PotentialPawnSeries.hpp"

#include "../../DebugInfo.hpp"

namespace CM {

PotentialPawnSeries::PotentialPawnSeries() {
}

PotentialPawnSeries::~PotentialPawnSeries() {
}

void PotentialPawnSeries::AddPawn(const Coordinates& coordinates) {
    pawnSeries.push_back(coordinates);
}

std::size_t PotentialPawnSeries::GetStartingPawnColumn() {
    if (pawnSeries.size() == 0u) {
        LOG_ERROR("pawnSeries.size() == 0u");
        return 0u;
    }

    Coordinates& firstPawnCoordinates = pawnSeries[0];
    return firstPawnCoordinates.x;
}

}
