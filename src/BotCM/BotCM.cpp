#include "BotCM.hpp"

#include <stdexcept>

#include "../Board.hpp"
#include "../Random.hpp"
#include "../DebugInfo.hpp"
#include "AllocationCounter.hpp"
#include "../AppConfig/FileAppConfigContainer.hpp"
#include "MovementGrade.h"
#include "CoordinatesWithGrade.h"

namespace CM {

BotCM::BotCM(const Board* const gameBoard, const PlayerType playerType, const Field playerColor):
     Player(gameBoard, playerType, playerColor),
     opponentDidAtLeastOneMovement(false),
     opponentPlayerColor(Field::Empty),
     currentTurnId(0u) {
}

BotCM::~BotCM() {
    (void)verifyAllocationCounter();
}

void BotCM::NotifyAboutOpponentMove(const Coordinates opponentMove) {
    if (!opponentDidAtLeastOneMovement) {
        opponentDidAtLeastOneMovement = true;
    }
    opponentLastMove = opponentMove;
}

Coordinates BotCM::MakeMove() {
    Coordinates coordinates;
    const bool result = MakeMoveMain(coordinates);
    if (!result) {
        throw std::runtime_error("");
        return coordinates;
    }
    currentTurnId++;
    return coordinates;
}

bool BotCM::MakeMoveMain(Coordinates& outputCoordinates) {
    bool result = false;

    // If this is the first turn for current player, do some initialization stuff.
    if (currentTurnId == 0u) {
        result = determineOpponentPlayerColor();
        if (!result) {
            return false;
        }
    }

    if (!opponentDidAtLeastOneMovement) {
        // BotCM begins the battle, so random coordinates will be returned.
        outputCoordinates.x = static_cast<std::size_t>(Random::RandomizeInt(board->getWidth ()));
        outputCoordinates.y = static_cast<std::size_t>(Random::RandomizeInt(board->getHeight()));
        return true;
    }

    const unsigned int PawnSeriesOrientationCount = 4u;
    CoordinatesWithGrade coordinatesWithGrade[PawnSeriesOrientationCount];
    std::size_t i = 0u;

    result = determineCoordinatesAndGradeInOneOrientationAndIncrementI(coordinatesWithGrade, PawnSeriesOrientation::VERTICAL  , i);
    if (!result) return false;

    result = determineCoordinatesAndGradeInOneOrientationAndIncrementI(coordinatesWithGrade, PawnSeriesOrientation::HORIZONTAL, i);
    if (!result) return false;

    result = determineCoordinatesAndGradeInOneOrientationAndIncrementI(coordinatesWithGrade, PawnSeriesOrientation::DECREASING, i);
    if (!result) return false;

    result = determineCoordinatesAndGradeInOneOrientationAndIncrementI(coordinatesWithGrade, PawnSeriesOrientation::INCREASING, i);
    if (!result) return false;

    CoordinatesWithGrade& coordinatesWithBestGrade = coordinatesWithGrade[0];
    for (unsigned int i = 1u; i < PawnSeriesOrientationCount; ++i) {
        if (coordinatesWithGrade[i].movementGrade > coordinatesWithBestGrade.movementGrade) {
            coordinatesWithBestGrade = coordinatesWithGrade[i];
        }
    }

    outputCoordinates = coordinatesWithBestGrade.movementCoordinates;
    return true;
}

bool BotCM::determineCoordinatesAndGradeInOneOrientationAndIncrementI(CoordinatesWithGrade coordinatesWithGrade[], PawnSeriesOrientation pawnSeriesOrientation, std::size_t& i) {
    const bool result = determineCoordinatesAndGradeInOneOrientation(
        pawnSeriesOrientation,
        coordinatesWithGrade[i].movementCoordinates,
        coordinatesWithGrade[i].movementGrade);

    if (!result) {
        return false;
    }
    i++;
    return true;
}

bool BotCM::determineCoordinatesAndGradeInOneOrientation(
        const PawnSeriesOrientation pawnSeriesOrientation,
        Coordinates& outputCoordinates,
        MovementGrade& movementImportanceGrade) {

    const int pawnsLineLenghtToWin = fileAppConfigContainer.PawnsLineLenghtToWin;
    const int K = pawnsLineLenghtToWin - 1;
    const int boardSize = fileAppConfigContainer.BoardSize;

    int minX = 0, centerX = 0, maxX = 0;

    std::size_t opponentLastMoveValue = 0u;

    if (   pawnSeriesOrientation == PawnSeriesOrientation::VERTICAL
        || pawnSeriesOrientation == PawnSeriesOrientation::HORIZONTAL) {

        if (pawnSeriesOrientation == PawnSeriesOrientation::VERTICAL) {
            // For vertical pawns series, column (x) is constant but row (y) is varying.
            opponentLastMoveValue = opponentLastMove.y;
        } else {
            // For horizontal pawns series, row (y) is constant but column (x) is varying.
            opponentLastMoveValue = opponentLastMove.x;
        }

        if (static_cast<int>(opponentLastMoveValue) - K >= 0) {
            minX = static_cast<int>(opponentLastMoveValue) - K;
        } else {
            minX = 0;
        }
        centerX = opponentLastMoveValue;
        if (static_cast<int>(opponentLastMoveValue) + K < boardSize) {
            maxX = static_cast<int>(opponentLastMoveValue) + K;
        } else {
            maxX = boardSize - 1;
        }

    } else if (pawnSeriesOrientation == PawnSeriesOrientation::DECREASING) {
        const int minCoordinate = static_cast<int>(std::min(opponentLastMove.x, opponentLastMove.y));
        if (minCoordinate < K) {
            minX = -minCoordinate;
        } else {
            minX = -K;
        }

        const int maxCoordinate = static_cast<int>(std::max(opponentLastMove.x, opponentLastMove.y));
        if (maxCoordinate > boardSize-1 - K) {
            maxX = boardSize-1 - maxCoordinate;
        } else {
            maxX = K;
        }
    } else if (pawnSeriesOrientation == PawnSeriesOrientation::INCREASING){
        const int minValue = static_cast<int>(std::min(opponentLastMove.x, boardSize-1 - opponentLastMove.y));
        if (minValue < K) {
            minX = -minValue;
        } else {
            minX = -K;
        }

        const int maxValue = static_cast<int>(std::min(boardSize-1 - opponentLastMove.x, opponentLastMove.y));
        if (maxValue < K) {
            maxX = maxValue;
        } else {
            maxX = K;
        }
    } else {
        LOG_ERROR("Wrong value of 'pawnSeriesOrientation'.");
        return false;
    }

    if (maxX < minX) {
        LOG_ERROR("maxX < minX");
        return false;
    }

    // First dimension: the whole gaps between enemy symbols.
    // Second dimension: indices of gap fields in the given gap.
    GapsCollectionT gaps;

    bool result = determineGaps(pawnSeriesOrientation, gaps, minX, centerX, maxX);
    if (!result) {
        return false;
    }
    makeMoveDecision(pawnSeriesOrientation, outputCoordinates, gaps, movementImportanceGrade);

    return true;
}

bool BotCM::determineGaps(const PawnSeriesOrientation pawnSeriesOrientation, GapsCollectionT& gaps, const int minX, const int centerX, const int maxX) {
    unsigned int opponentSymbolsCount = 0u;
    std::size_t lastOppopentSymbolFoundIndex = 0u;
    bool opponentSymbolAlreadyFound = false;
    bool lastNotEmptySymbolWasCurrentPlayerSymbol = false;

    // Pointer to the current gap, from 'gaps'. If recent analyzed field
    // was not a gap field, 'currentGap' will equal nullptr.
    SingleGapT* currentGap = nullptr;

    // Vector with all fields which are processed during determining fields as gaps.
    std::vector<Field> fieldsProcessed;
    // Index of opponent last move field in 'fieldsProcessed'.
    std::size_t fieldsProcessedOpponentLastMoveIndex = 0u;

    // 1. Loop for filling 'gaps' collection with real gaps.
    for (int i = minX; i <= maxX; ++i) {
        Coordinates currentCoordinates;
        bool result = determineCurrentCoordinatesForGapProcessing(pawnSeriesOrientation, i, currentCoordinates);
        if (!result) {
            return false;
        }

        result = board->IsFieldOnBoard(currentCoordinates.x, currentCoordinates.y);
        if (!result) {
            LOG_ERROR("Field (", currentCoordinates.x, ", ", currentCoordinates.y, ") not on board!");
            return false;
        }

        Field currentField = board->at(currentCoordinates.x, currentCoordinates.y);
        fieldsProcessed.push_back(currentField);
        if (currentCoordinates == opponentLastMove) {
            fieldsProcessedOpponentLastMoveIndex = fieldsProcessed.size() - 1u;
        }

        result = determineFieldAsGap(currentField, opponentSymbolAlreadyFound, lastNotEmptySymbolWasCurrentPlayerSymbol, lastOppopentSymbolFoundIndex, opponentSymbolsCount, currentGap, gaps, pawnSeriesOrientation, maxX, i, currentCoordinates);
        if (!result) {
            return false;
        }
    }

    // 2. Loop for filling data when there is no gap in current orientation.
    const int pawnsLineLenghtToWin = fileAppConfigContainer.PawnsLineLenghtToWin;

    NotGapOneSideData notGapLeftSideData, notGapRightSideData;

    for (std::size_t i = 1u; i <= static_cast<std::size_t>(pawnsLineLenghtToWin)-2u; ++i) {
        // Process right side.
        const size_t leftSideCurrentIndex = fieldsProcessedOpponentLastMoveIndex + i;
        if (leftSideCurrentIndex < fieldsProcessed.size()) {
            const Field currentField = fieldsProcessed[leftSideCurrentIndex];
            bool result = processNotGapOrientationOneSide(currentField, notGapRightSideData);
            if (!result) { return false; }
        }

        // Process left side.
        int rightSideCurrentIndex = static_cast<int>(fieldsProcessedOpponentLastMoveIndex) - static_cast<int>(i);
        if (rightSideCurrentIndex >= 0) {
            const Field currentField = fieldsProcessed[rightSideCurrentIndex];
            bool result = processNotGapOrientationOneSide(currentField, notGapLeftSideData);
            if (!result) { return false; }
        }
    }

    int opponentPawnSeriesWithHisLastMoveLength
        = notGapLeftSideData.opponentPawnSeriesLength
        + notGapRightSideData.opponentPawnSeriesLength
        + 1;

    return true;
}

bool BotCM::processNotGapOrientationOneSide(
        const Field currentField,
        NotGapOneSideData& notGapOneSideData) {

    if (currentField == opponentPlayerColor) {
        if (!notGapOneSideData.notOpponentPawnMet) {
            notGapOneSideData.opponentPawnSeriesLength++;
        }

    } else if (currentField == playerColor) {
        notGapOneSideData.notOpponentPawnMet = true;
        notGapOneSideData.currentPlayerSymbolFound = true;

    } else if (currentField == Field::Empty) {
        notGapOneSideData.notOpponentPawnMet = true;
        if (!notGapOneSideData.currentPlayerSymbolFound) {
            notGapOneSideData.emptyFieldsCountAfterOpponentPawnSeries++;
        }

    } else {
        LOG_ERROR("Wrong field type.");
        return false;
    }
    return true;
}

bool BotCM::determineCurrentCoordinatesForGapProcessing(const PawnSeriesOrientation pawnSeriesOrientation, int i, Coordinates& currentCoordinates) {
    const std::size_t iStdSizeT = static_cast<std::size_t>(i);

    switch (pawnSeriesOrientation) {
        case PawnSeriesOrientation::VERTICAL:
            currentCoordinates.x = opponentLastMove.x;
            currentCoordinates.y = iStdSizeT;
            break;
        case PawnSeriesOrientation::HORIZONTAL:
            currentCoordinates.x = iStdSizeT;
            currentCoordinates.y = opponentLastMove.y;
            break;
        case PawnSeriesOrientation::DECREASING:
            currentCoordinates.x = opponentLastMove.x + iStdSizeT;
            currentCoordinates.y = opponentLastMove.y + iStdSizeT;
            break;
        case PawnSeriesOrientation::INCREASING:
            currentCoordinates.x = opponentLastMove.x + iStdSizeT;
            currentCoordinates.y = opponentLastMove.y - iStdSizeT;
            break;
        default:
            LOG_ERROR("Not implemented yet!");
            return false;
    }
    return true;
}

bool BotCM::determineFieldAsGap(
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
        const Coordinates currentCoordinates) {

    if (currentField == opponentPlayerColor) {
        opponentSymbolAlreadyFound = true;
        lastNotEmptySymbolWasCurrentPlayerSymbol = false;
        lastOppopentSymbolFoundIndex = static_cast<std::size_t>(i);
        opponentSymbolsCount++;
        currentGap = nullptr;
    } else if (currentField == playerColor) {
        lastNotEmptySymbolWasCurrentPlayerSymbol = true;

        // If there is currently built a gap and we have found current player symbol in the gap,
        // it is no longer a gap.
        if (currentGap) {
            gaps.pop_back();
            currentGap = nullptr;
        }
    } else { // Current field is empty.
        if (opponentSymbolAlreadyFound) {
            // If last not empty symbol was current player symbol, we mark current gap as not gap,
            // because gap definition says, that is shall contain only empty fields and it shall be
            // between enemy player symbols (none of these symbols shall be current player symbol).
            if (!lastNotEmptySymbolWasCurrentPlayerSymbol) {
                if (!currentGap) {
                    gaps.emplace_back(SingleGapT());
                    const std::size_t newGapId = gaps.size()-1u;
                    currentGap = &(gaps[newGapId]);
                }
                currentGap->emplace_back(currentCoordinates);
            }
        }
        if (i == maxX) { // If we are processing last index to process:
            // Here we know that we are processing last index to process, and the
            // field on this index is not enemy symbol. So, if there is currently gap built,
            // we have to remove the (this) last gap.
            if (currentGap) {
                const std::size_t lastGapLength = maxX - lastOppopentSymbolFoundIndex;
                if (lastGapLength > 0u) {
                    // Remove the last gap.
                    gaps.erase(gaps.end()-1);
                }
            }
        }
    }
    return true;
}

bool BotCM::makeMoveDecision(
        const PawnSeriesOrientation pawnSeriesOrientation,
        Coordinates& outputCoordinates,
        const GapsCollectionT& gaps,
        MovementGrade& movementImportanceGrade) {

    const std::size_t gapsCount = gaps.size();
    if (gapsCount == 0u) {
        //TODO: to be implemented.

        // Default movement coordinates.
        outputCoordinates.x = 0u;
        outputCoordinates.y = 0u;
        movementImportanceGrade.SetGrade(0u);

    } else if (gapsCount == 1u) {
        const SingleGapT& gap = gaps.at(0);

        const bool result = randomizeFieldInGap(gap, outputCoordinates);
        if (!result) {
            return false;
        }
        movementImportanceGrade.SetGrade(3u);

    } else if (gapsCount > 1u) {
        std::size_t shortestGapSize = SIZE_MAX;
        const SingleGapT* shortestGap = nullptr;

        // Search for the shortest gap.
        for (const SingleGapT& gap : gaps) {
            const std::size_t currentGapSize = gap.size();
            if (currentGapSize < shortestGapSize) {
                shortestGapSize = currentGapSize;
                shortestGap = &gap;
            }
        }
        const bool result = randomizeFieldInGap(*shortestGap, outputCoordinates);
        if (!result) {
            return false;
        }

        movementImportanceGrade.SetGrade(4u);
    }
    return true;
}

bool BotCM::randomizeFieldInGap(const SingleGapT& gap, Coordinates& outputCoordinates) {
    const std::size_t gapSize = gap.size();
    if (gapSize == 0) {
        LOG_ERROR("gapSize == 0");
        return false;
    }
    const std::size_t randomizedIndexInGap = Random::RandomizeInt(gapSize);
    outputCoordinates = gap.at(randomizedIndexInGap);
    return true;
}

bool BotCM::determineOpponentPlayerColor() {
    switch (playerColor) {
        case Field::White:
            opponentPlayerColor = Field::Black;
            break;
        case Field::Black:
            opponentPlayerColor = Field::White;
            break;
        default:
            LOG_ERROR("Wrong value of 'playerColor'.");
            return false;
    }
    return true;
}

bool BotCM::verifyAllocationCounter() {
    unsigned long long int allocationCounter = AllocationCounter::GetCounter();
    if (allocationCounter != 0uLL) {
        LOG_LN("Allocation counter is not zero. At least one 'new' has no corresponding 'delete'.");
        return false;
    }
    return true;
}

}
