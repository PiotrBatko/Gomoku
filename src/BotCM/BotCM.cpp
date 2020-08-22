#include "BotCM.hpp"

#include <stdexcept>

#include "../Board.hpp"
#include "../Random.hpp"
#include "../DebugInfo.hpp"
#include "AllocationCounter.hpp"
#include "../AppConfig/FileAppConfigContainer.hpp"

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
    // If this is the first turn for current player, do some initialization stuff.
    if (currentTurnId == 0u) {
        bool result = determineOpponentPlayerColor();
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

    const int pawnsLineLenghtToWin = fileAppConfigContainer.PawnsLineLenghtToWin;
    const int K = pawnsLineLenghtToWin - 1;
    const int boardSize = fileAppConfigContainer.BoardSize;

    std::size_t minX = 0u;
    if (static_cast<int>(opponentLastMove.y) - K >= 0) {
        minX = static_cast<std::size_t>(opponentLastMove.y - K);
    } else {
        minX = 0u;
    }
    std::size_t maxX = 0u;
    if (static_cast<int>(opponentLastMove.y) + K < boardSize) {
        maxX = static_cast<std::size_t>(opponentLastMove.y + K);
    } else {
        maxX = static_cast<std::size_t>(boardSize - 1);
    }

    if (maxX < minX) {
        LOG_ERROR("maxX < minX");
        return false;
    }

    // First dimension: the whole gaps between enemy symbols.
    // Second dimension: indices of gap fields in the given gap.
    GapsCollectionT gaps;

    determineGaps(gaps, minX, maxX);
    makeMoveDecision(outputCoordinates, gaps);
    return true;
}

void BotCM::determineGaps(GapsCollectionT& gaps, const std::size_t minX, const std::size_t maxX) {
    unsigned int opponentSymbolsCount = 0u;
    std::size_t lastOppopentSymbolFoundIndex = 0u;
    bool opponentSymbolAlreadyFound = false;
    bool lastNotEmptySymbolWasCurrentPlayerSymbol = false;

    // Pointer to the current gap, from 'gaps'. If recent analyzed field
    // was not a gap field, 'currentGap' will equal nullptr.
    SingleGapT* currentGap = nullptr;

    // This loop fills 'gaps' collection with real gaps.
    for (std::size_t i = minX; i <= maxX; ++i) {
        const Field currentField = board->at(opponentLastMove.x, i);

        if (currentField == opponentPlayerColor) {
            opponentSymbolAlreadyFound = true;
            lastNotEmptySymbolWasCurrentPlayerSymbol = false;
            lastOppopentSymbolFoundIndex = i;
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
                        gaps.push_back(SingleGapT());
                        const std::size_t newGapId = gaps.size()-1u;
                        currentGap = &(gaps[newGapId]);
                    }
                    currentGap->push_back(i);
                }
            }
            if (i == maxX) { // If we are processing last index to process:
                // Here we know that we are processing last index to process, and the
                // field on this index is not enemy symbol. So, we have to remove
                // the last gap.
                const std::size_t lastGapLength = maxX - lastOppopentSymbolFoundIndex;
                if (lastGapLength > 0u) {
                    // Remove the last gap.
                    gaps.erase(gaps.end()-1);
                }

            }
        }
    }
}

void BotCM::makeMoveDecision(Coordinates& outputCoordinates, const GapsCollectionT& gaps) {
    const std::size_t gapsCount = gaps.size();
    if (gapsCount == 1u) {
        const SingleGapT& gap = gaps.at(0);
        const std::size_t gapSize = gap.size();
        const std::size_t randomizedIndexInGap = Random::RandomizeInt(gapSize);
        const std::size_t finalMoveIndex = gap.at(randomizedIndexInGap);

        outputCoordinates.x = opponentLastMove.x;
        outputCoordinates.y = finalMoveIndex;
    } else {
        //TODO: continue.
        // Default movement coordinates.
        outputCoordinates.x = 0u;
        outputCoordinates.y = 0u;
    }
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
