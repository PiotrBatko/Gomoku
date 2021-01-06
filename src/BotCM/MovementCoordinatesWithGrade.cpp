#include "MovementCoordinatesWithGrade.h"

#include "../DebugInfo.hpp"

namespace CM {

MovementCoordinatesWithGrade::MovementCoordinatesWithGrade()
    : valuesSet(false), movementCoordinates(), movementImportanceGrade() {
}

bool MovementCoordinatesWithGrade::Set(Coordinates& movementCoordinates, const MovementGrade::GradeNumberType movementImportanceGrade) {
    if (valuesSet) {
        LOG_ERROR("'valuesSet' equals true!");
        return false;
    }

    this->movementCoordinates = movementCoordinates;

    const bool result = this->movementImportanceGrade.SetGrade(movementImportanceGrade);
    if (!result) {
        return false;
    }

    valuesSet = true;
    return true;
}

bool MovementCoordinatesWithGrade::SetZeroGrade() {
    if (valuesSet) {
        LOG_ERROR("'valuesSet' equals true!");
        return false;
    }
    const bool result = this->movementImportanceGrade.SetGrade(0u);
    if (!result) {
        return false;
    }

    valuesSet = true;
    return true;
}

Coordinates MovementCoordinatesWithGrade::GetMovementCoordinates() {
    return movementCoordinates;
}
MovementGrade::GradeNumberType MovementCoordinatesWithGrade::GetMovementImportanceGrade() {
    return movementImportanceGrade.GetGrade();
}

}
