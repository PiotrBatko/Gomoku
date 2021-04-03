#ifndef _BOTCM_POTENTIAL_PAWNSERIES_HPP_
#define _BOTCM_POTENTIAL_PAWNSERIES_HPP_

#include <list>

#include "../../Coordinates.hpp"

namespace CM {

class PotentialPawnSeries {
public:
    PotentialPawnSeries();
    virtual ~PotentialPawnSeries();

    void AddPawnOnFront(const Coordinates& coordinates);
    void AddPawnOnBack(const Coordinates& coordinates);
    std::list<Coordinates>& GetPawnSeries();

    std::size_t GetStartingPawnColumn();

private:
    std::list<Coordinates> pawnSeries;
};

}

#endif
