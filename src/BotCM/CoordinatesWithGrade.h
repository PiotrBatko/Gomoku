#ifndef _BOT_CM_COORDINATES_WITH_GRADE_H_
#define _BOT_CM_COORDINATES_WITH_GRADE_H_

#include "../Coordinates.hpp"
#include "MovementGrade.h"

namespace CM {

class CoordinatesWithGrade {
public:
    CoordinatesWithGrade();
    virtual ~CoordinatesWithGrade();

    Coordinates   movementCoordinates;
    MovementGrade movementGrade;
};

}

#endif
