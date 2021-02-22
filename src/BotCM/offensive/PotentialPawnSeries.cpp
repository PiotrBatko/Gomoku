#include "PotentialPawnSeries.hpp"

namespace CM {

PotentialPawnSeries::PotentialPawnSeries() {
}

PotentialPawnSeries::~PotentialPawnSeries() {
}

void PotentialPawnSeries::AddPawn(const Coordinates& coordinates) {
    pawnSeries.push_back(coordinates);
}

}
