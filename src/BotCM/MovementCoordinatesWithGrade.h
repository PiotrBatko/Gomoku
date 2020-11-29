#ifndef _CM_MOVEMENT_COORDINATES_WITH_GRADE_H_
#define _CM_MOVEMENT_COORDINATES_WITH_GRADE_H_

#include "../Coordinates.hpp"
#include "MovementGrade.h"

namespace CM {

class MovementCoordinatesWithGrade {
public:
    MovementCoordinatesWithGrade();

    bool Set(Coordinates& movementCoordinates, MovementGrade::GradeNumberType movementImportanceGrade);
    bool SetZeroGrade();

    Coordinates GetMovementCoordinates();
    MovementGrade::GradeNumberType GetMovementImportanceGrade();

private:
    Coordinates movementCoordinates;
    MovementGrade movementImportanceGrade;

    bool valuesSet;
};

}

#endif
