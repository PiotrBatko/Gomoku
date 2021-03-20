#ifndef BOTCM_OFFENSIVE_MANAGER_HPP_
#define BOTCM_OFFENSIVE_MANAGER_HPP_

#include <vector>
#include <list>

#include "../CMEnums.hpp"
#include "../../Coordinates.hpp"
#include "PawnColor.hpp"
#include "../../Field.hpp"
#include "PotentialPawnSeries.hpp"

class Board;

namespace CM {

class OffensiveManager {
public:
    OffensiveManager();
    virtual ~OffensiveManager();

    void Initialize(const Board* const board, PawnColor playerColor, Field opponentPlayerColor);

    bool UpdatePotentialPawnSeriesAfterCurrentPlayerMovement(Coordinates& currentPlayerMovement);


private:
    struct PotentialPawnSeriesData {
        std::size_t currentPlayerPawnsInSeriesCount = 1u;

        std::list<Coordinates> currentPlayerPawnSeries;

        // Count of empty fields on left (lower)/right (higher) fields from current player pawn series.
        std::size_t emptyFieldOnLeftSide = 0u;
        std::size_t emptyFieldOnRightSide = 0u;
    };

    bool updatePotentialPawnSeriesInOneOrientation(PawnSeriesOrientation pawnSeriesOrientation);
    void updatePotentialPawnSeriesOneSideData(PawnSeriesOrientation pawnSeriesOrientation, Monotonicity direction, PotentialPawnSeriesData& potentialPawnSeriesData);
    void AddPotentialPawnSeriesToList(std::vector<std::list<PotentialPawnSeries>>& potentialPawnSeriesList, PotentialPawnSeriesData& potentialPawnSeriesData);

    Coordinates currentPlayerMovement;

    const Board* board;
    PawnColor playerColor;
    Field opponentPlayerColor;

    // For each collection below:
    // First dimension - column index of starting pawn of given potential series
    // Second dimension - collection of pawn series which starting pawn has given column index.
    std::vector<std::list<PotentialPawnSeries>> potentialPawn4LongSeriesList;
    std::vector<std::list<PotentialPawnSeries>> potentialPawn3LongSeriesList;
    std::vector<std::list<PotentialPawnSeries>> potentialPawn2LongSeriesList;
};

}

#endif