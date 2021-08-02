#ifndef _BOT_CM_HPP
#define _BOT_CM_HPP

#include <vector>

#include "CMEnums.hpp"
#include "../Player.hpp"
#include "MovementGrade.h"
#include "EmptyFieldsManager.hpp"
#include "../Coordinates.hpp"
#include "../Field.hpp"
#include "offensive/OffensiveManager.hpp"

class Board;

namespace CM {

using SingleGapT = std::vector<Coordinates>;
using GapsCollectionT = std::vector<SingleGapT>;

class CoordinatesWithGrade;
class MovementCoordinatesWithGrade;

class BotCM : public Player {
public:
	BotCM(const Board* gameBoard, PlayerType playerType, PawnColor playerColor);
    virtual ~BotCM();

    void NotifyAboutOpponentMove(Coordinates opponentMove) override;
    Coordinates MakeMove() override;

    // For testing:
    EmptyFieldsManager& GetEmptyFieldsManager();

private:
    bool opponentDidAtLeastOneMovement;

    Coordinates opponentLastMove;

    // Identifier of current turn for current player. Opponent turns are not counted here.
    unsigned int currentTurnId;

    // Color of the opponent player.
    Field opponentPlayerColor;

    EmptyFieldsManager emptyFieldsManager;

    OffensiveManager offensiveManager;

    // Struct having data of not gap processing in one orientation, for one (left or right) side.
    struct NotGapOneSideData {
        bool notOpponentPawnMet = false;
        // Length of opponent pawn series, for pawns adjacent from left side to the last opponent player movement's placed pawn.
        std::size_t opponentPawnSeriesLength = 0;

        bool currentPlayerSymbolFound = false;
        std::size_t emptyFieldsCountAfterOpponentPawnSeries = 0;

        bool firstFreeFieldFound = false;

        // First found free field.
        Coordinates firstFreeFieldCoordinates;
    };

    bool MakeMoveMain(Coordinates& outputCoordinates);

    // The output randomized field is given in 'outputCoordinates'.
    bool randomizeFieldInGap(const SingleGapT& gap, Coordinates& outputCoordinates);

    bool determineOpponentPlayerColor();
    bool verifyAllocationCounter();
    bool determineGaps(
            const PawnSeriesOrientation pawnSeriesOrientation,
            GapsCollectionT& gaps,
            const int minX,
            const int centerX,
            const int maxX,
            std::vector<Field>& fieldsProcessed,
            std::vector<Coordinates>& coordinatesProcessed,
            std::size_t& fieldsProcessedOpponentLastMoveIndex);
    bool determineNoGapData(
            std::vector<Field>& fieldsProcessed,
            std::size_t fieldsProcessedOpponentLastMoveIndex,
            NotGapOneSideData& notGapLeftSideData,
            NotGapOneSideData& notGapRightSideData,
            std::vector<Coordinates>& coordinatesProcessed);
    bool determineCoordinatesAndGradeInOneOrientationAndIncrementI(MovementCoordinatesWithGrade coordinatesWithGrade[], PawnSeriesOrientation pawnSeriesOrientation, std::size_t& i);
    bool makeMoveDecision(
            const PawnSeriesOrientation pawnSeriesOrientation,
            const GapsCollectionT& gaps,
            NotGapOneSideData& notGapLeftSideData,
            NotGapOneSideData& notGapRightSideData,
            MovementCoordinatesWithGrade& outputCoordinatesAndGrade);
    bool determineCoordinatesAndGradeInOneOrientation(PawnSeriesOrientation pawnSeriesOrientation, MovementCoordinatesWithGrade& outputCoordinatesAndGrade);
    bool determineCurrentCoordinatesForGapProcessing(const PawnSeriesOrientation pawnSeriesOrientation, int i, Coordinates& currentCoordinates);
    bool processNotGapOrientationOneSide(
            const Field currentField,
            NotGapOneSideData& notGapLeftSideData,
            const Coordinates& currentCoordinates);
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
    void adjustMovementGradeToOpponentPawnSeriesLenght(MovementGrade::GradeNumberType& movementGrade);

    const bool MakeDummyMovementsForTesting = true;

    // False means that only defensive strategy is processed.
    const bool EnableOffensiveStrategy = true;
};

}

#endif
