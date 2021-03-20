#ifndef _BOTCM_POTENTIAL_PAWNSERIES_HPP_
#define _BOTCM_POTENTIAL_PAWNSERIES_HPP_

#include <vector>

#include "../../Coordinates.hpp"

namespace CM {

class PotentialPawnSeries {
public:
    PotentialPawnSeries();
    virtual ~PotentialPawnSeries();

    void AddPawn(const Coordinates& coordinates);
    std::vector<Coordinates>& GetPawnSeries();

    std::size_t GetStartingPawnColumn();

private:
    std::vector<Coordinates> pawnSeries;
};

}

#endif