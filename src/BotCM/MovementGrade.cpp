#include "MovementGrade.h"

#include "../DebugInfo.hpp"

namespace CM {

MovementGrade::MovementGrade()
    : movementGrade(0u) {
}

MovementGrade::~MovementGrade() {
}

bool MovementGrade::SetGrade(const GradeNumberType grade) {
    if (grade > MovementGradeMaxValue) {
        LOG_ERROR("grade > MovementGradeMaxValue. grade = ", grade);
        return false;
    }
    movementGrade = grade;
    return true;
}

MovementGrade::GradeNumberType MovementGrade::GetGrade() {
    return movementGrade;
}

bool MovementGrade::operator >(const MovementGrade& other) {
    return movementGrade > other.movementGrade;
}

}
