#ifndef _BOT_CM_MOVEMENT_GRADE_H_
#define _BOT_CM_MOVEMENT_GRADE_H_

namespace CM {

class MovementGrade {
public:
    using GradeNumberType = unsigned int;

    MovementGrade();
    MovementGrade(GradeNumberType movementGrade);
    virtual ~MovementGrade();

    bool SetGrade(GradeNumberType grade);
    GradeNumberType GetGrade() const;

    bool operator >(const MovementGrade& other);

    static const GradeNumberType MovementGradeMaxValue = 12u;

private:
    GradeNumberType movementGrade;
};

}

#endif
