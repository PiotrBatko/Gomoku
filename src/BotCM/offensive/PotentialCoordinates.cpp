#include "PotentialCoordinates.hpp"

namespace CM {

PotentialCoordinates::PotentialCoordinates(const Coordinates& coordinates, PawnSeriesOrientation pawnSeriesOrientation, const MovementGrade& grade)
		: pawnSeriesOrientation(pawnSeriesOrientation) {
	movementCoordinatesWithGrade.Set(coordinates, grade.GetGrade());
}

PotentialCoordinates::~PotentialCoordinates() {
}

}
