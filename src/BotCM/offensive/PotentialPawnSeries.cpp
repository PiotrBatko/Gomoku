#include "PotentialPawnSeries.hpp"

#include "../../DebugInfo.hpp"

namespace CM {

PotentialPawnSeries::PotentialPawnSeries() {
}

PotentialPawnSeries::~PotentialPawnSeries() {
}

void PotentialPawnSeries::AddPawnOnFront(const Coordinates& coordinates) {
    pawnSeries.push_front(coordinates);
}

void PotentialPawnSeries::AddPawnOnBack(const Coordinates& coordinates) {
    pawnSeries.push_back(coordinates);
}

std::size_t PotentialPawnSeries::GetStartingPawnColumn() {
    if (pawnSeries.size() == 0u) {
        LOG_ERROR("pawnSeries.size() == 0u");
        return 0u;
    }

    Coordinates& firstPawnCoordinates = pawnSeries.front();
    return firstPawnCoordinates.x;
}

std::list<Coordinates>& PotentialPawnSeries::GetPawnSeries() {
    return pawnSeries;
}

void PotentialPawnSeries::LogPawnSeries() {
	for (Coordinates& coordinates : pawnSeries) {
		LOG_LN(coordinates.x, ", ", coordinates.y);
	}
}

}
