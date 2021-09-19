#ifndef _BOTCM_OFFENSIVE_POTENTIALCOORDINATES_HPP_
#define _BOTCM_OFFENSIVE_POTENTIALCOORDINATES_HPP_

#include "../../Coordinates.hpp"
#include "../MovementCoordinatesWithGrade.h"
#include "../CMEnums.hpp"

namespace CM {

class PotentialCoordinates {
public:
	PotentialCoordinates(const Coordinates& coordinates, PawnSeriesOrientation pawnSeriesOrientation, const MovementGrade& grade);
	virtual ~PotentialCoordinates();

	MovementCoordinatesWithGrade movementCoordinatesWithGrade;

	PawnSeriesOrientation pawnSeriesOrientation;
};

}

#endif
