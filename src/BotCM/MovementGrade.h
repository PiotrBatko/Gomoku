#ifndef _BOT_CM_MOVEMENT_GRADE_H_
#define _BOT_CM_MOVEMENT_GRADE_H_

namespace CM {

class MovementGrade {
public:
    using GradeNumberType = unsigned int;

    MovementGrade();
    virtual ~MovementGrade();

    bool SetGrade(GradeNumberType grade);
    GradeNumberType GetGrade();

    bool operator >(const MovementGrade& other);

    static const GradeNumberType MovementGradeMaxValue = 6u;

private:
    GradeNumberType movementGrade;
};

}

#endif
