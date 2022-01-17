#ifndef _BOTCM_POTENTIAL_PAWNSERIES_HPP_
#define _BOTCM_POTENTIAL_PAWNSERIES_HPP_

#include <list>

#include "../CMEnums.hpp"
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
    void LogPawnSeries();

    void SetSeriesOrientation(PawnSeriesOrientation pawnSeriesOrientation);
    PawnSeriesOrientation GetSeriesOrientation();

    // Determine coordinates of a pawn which could be placed as an enlargement of
    // the potential pawn series.
    bool DetermineEnlargementPawnCoordinates(Coordinates& a, Coordinates& b);

private:
    std::list<Coordinates> pawnSeries;

    PawnSeriesOrientation pawnSeriesOrientation;
};

}

#endif
