#ifndef _BOTCM_OFFENSIVE_POTENTIALCOORDINATES_HPP_
#define _BOTCM_OFFENSIVE_POTENTIALCOORDINATES_HPP_

#include "../../Coordinates.hpp"
#include "../CMEnums.hpp"

namespace CM {

class PotentialCoordinates {
public:
	PotentialCoordinates();
	virtual ~PotentialCoordinates();

private:
	// Coordinates of the potential field.
	Coordinates coordinates;

	PawnSeriesOrientation pawnSeriesOrientation;
};

}

#endif
