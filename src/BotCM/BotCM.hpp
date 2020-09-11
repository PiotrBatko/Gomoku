#ifndef _BOT_CM_HPP
#define _BOT_CM_HPP

#include <vector>

#include "../Player.hpp"

class Board;

namespace CM {

using SingleGapT = std::vector<Coordinates>;
using GapsCollectionT = std::vector<SingleGapT>;

class MovementGrade;

// Orientation of possible pawn series to win.
enum class PawnSeriesOrientation {
    // Column is not changing, rows are incrementing.
    VERTICAL,
    // Row is not changing, columns are incrementing.
    HORIZONTAL,
    // Columns are incrementing, rows are decrementing.
    INCREASING,
    // Columns are incrementing and rows are incrementing too.
    DECREASING
};

class BotCM : public Player {
public:
    BotCM(const Board* gameBoard, PlayerType playerType, Field playerColor);
    virtual ~BotCM();

    void NotifyAboutOpponentMove(Coordinates opponentMove) override;
    Coordinates MakeMove() override;

private:
    bool opponentDidAtLeastOneMovement;

    Coordinates opponentLastMove;

    // Identifier of current turn for current player. Opponent turns are not counted here.
    unsigned int currentTurnId;

    // Color of the opponent player.
    Field opponentPlayerColor;

    bool MakeMoveMain(Coordinates& outputCoordinates);
    bool determineOpponentPlayerColor();
    bool verifyAllocationCounter();
    bool determineGaps(const PawnSeriesOrientation pawnSeriesOrientation, GapsCollectionT& gaps, const int minX, const int maxX);
    void makeMoveDecision(const PawnSeriesOrientation pawnSeriesOrientation, Coordinates& outputCoordinates, const GapsCollectionT& gaps, MovementGrade& movementImportanceGrade);
    bool determineCoordinatesAndGradeInOneOrientation(PawnSeriesOrientation pawnSeriesOrientation, Coordinates& outputCoordinates, MovementGrade& movementImportanceGrade);

    bool determineFieldAsGap(
            const Field currentField,
            bool& opponentSymbolAlreadyFound,
            bool& lastNotEmptySymbolWasCurrentPlayerSymbol,
            std::size_t& lastOppopentSymbolFoundIndex,
            unsigned int& opponentSymbolsCount,
            SingleGapT*& currentGap,
            GapsCollectionT& gaps,
            const PawnSeriesOrientation pawnSeriesOrientation,
            const int maxX,
            const int i,
            const Coordinates currentCoordinates);
};

}

#endif
