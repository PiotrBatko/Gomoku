#include "PotentialPawnSeries.hpp"

#include "../../DebugInfo.hpp"

namespace CM {

PotentialPawnSeries::PotentialPawnSeries() {
	pawnSeriesOrientation = PawnSeriesOrientation::NONE;
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

void PotentialPawnSeries::SetSeriesOrientation(PawnSeriesOrientation pawnSeriesOrientation) {
	this->pawnSeriesOrientation = pawnSeriesOrientation;
}

PawnSeriesOrientation PotentialPawnSeries::GetSeriesOrientation() {
    return pawnSeriesOrientation;
}

bool PotentialPawnSeries::DetermineEnlargementPawnCoordinates(Coordinates& a, Coordinates& b) {
	Coordinates& firstPawn = pawnSeries.front();
	Coordinates& lastPawn = pawnSeries.back();

	switch (pawnSeriesOrientation) {
		case PawnSeriesOrientation::VERTICAL:
			a = Coordinates(firstPawn.x, firstPawn.y - 1u);
			b = Coordinates( lastPawn.x,  lastPawn.y + 1u);
			break;
		case PawnSeriesOrientation::HORIZONTAL:
			a = Coordinates(firstPawn.x - 1u, firstPawn.y);
			b = Coordinates( lastPawn.x + 1u,  lastPawn.y);
			break;
		case PawnSeriesOrientation::INCREASING:
			a = Coordinates(firstPawn.x - 1, firstPawn.y + 1);
			b = Coordinates( lastPawn.x + 1,  lastPawn.y - 1);
			break;
		case PawnSeriesOrientation::DECREASING:
			a = Coordinates(firstPawn.x - 1, firstPawn.y - 1);
			b = Coordinates( lastPawn.x + 1,  lastPawn.y + 1);
			break;
		case PawnSeriesOrientation::NONE:
			LOG_ERROR("pawnSeriesOrientation == PawnSeriesOrientation::NONE");
			return false;
	}
	return true;
}

}
