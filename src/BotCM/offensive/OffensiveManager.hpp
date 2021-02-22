#ifndef BOTCM_OFFENSIVE_MANAGER_HPP_
#define BOTCM_OFFENSIVE_MANAGER_HPP_

#include "../CMEnums.hpp"
#include "../../Coordinates.hpp"
#include "PawnColor.hpp"
#include "../../Field.hpp"

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

        // Count of empty fields on left (lower)/right (higher) fields from current player pawn series.
        std::size_t emptyFieldOnLeftSide = 0u;
        std::size_t emptyFieldOnRightSide = 0u;
    };

    void updatePotentialPawnSeriesOneSideData(Direction direction, PotentialPawnSeriesData& potentialPawnSeriesData);

    Coordinates currentPlayerMovement;

    const Board* board;
    PawnColor playerColor;
    Field opponentPlayerColor;
};

}

#endif
