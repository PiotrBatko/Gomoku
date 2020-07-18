#include "BotCM.hpp"

#include <stdexcept>

#include "../Random.hpp"
#include "../Board.hpp"
#include "../DebugInfo.hpp"
#include "../AppConfig/FileAppConfigContainer.h"

namespace CM {

BotCM::BotCM(const Board* const gameBoard, const PlayerType playerType, const Field playerColor):
     Player(gameBoard, playerType, playerColor),
     opponentDidAtLeastOneMovement(false),
     opponentPlayerColor(Field::Empty),
     currentTurnId(0u) {
}

BotCM::~BotCM() {
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

    unsigned int opponentSymbolsCount = 0u;
    unsigned int gapCount = 0u;
    std::size_t lastOppopentSymbolFoundIndex = 0u;
    bool opponentSymbolAlreadyFound = false;

    for (std::size_t i = minX; i <= maxX; ++i) {
        const Field currentField = board->at(opponentLastMove.x, i);

        if (currentField == opponentPlayerColor) {
            opponentSymbolAlreadyFound = true;
            lastOppopentSymbolFoundIndex = i;
            opponentSymbolsCount++;
        } else { // Current field is empty or contains current player color.
            if (opponentSymbolAlreadyFound) {
                gapCount++;
            }
            if (i == maxX) { // If we are processing last index to process:
                std::size_t lastGapLength = maxX - lastOppopentSymbolFoundIndex;
                gapCount -= lastGapLength;
            }
        }
    }

    //TODO: continue.

    outputCoordinates.x = 0u;
    outputCoordinates.y = 0u;
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

}
