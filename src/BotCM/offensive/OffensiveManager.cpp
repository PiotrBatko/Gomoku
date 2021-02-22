#include <BotCM/offensive/OffensiveManager.hpp>

#include "../../AppConfig/FileAppConfigContainer.hpp"
#include "../../Board.hpp"

namespace CM {

OffensiveManager::OffensiveManager() {
    board = nullptr;
    playerColor = PawnColor::White;
    opponentPlayerColor = Field::Empty;
}

OffensiveManager::~OffensiveManager() {
}

void OffensiveManager::Initialize(const Board* const board, PawnColor playerColor, Field opponentPlayerColor) {
    this->board = board;
    this->playerColor = playerColor;
    this->opponentPlayerColor = opponentPlayerColor;
}

bool OffensiveManager::UpdatePotentialPawnSeriesAfterCurrentPlayerMovement(Coordinates& currentPlayerMovement) {
    this->currentPlayerMovement = currentPlayerMovement;

    PotentialPawnSeriesData potentialPawnSeriesData;
    updatePotentialPawnSeriesOneSideData(Direction::LEFT, potentialPawnSeriesData);
    updatePotentialPawnSeriesOneSideData(Direction::RIGHT, potentialPawnSeriesData);
    return true;
}

void OffensiveManager::updatePotentialPawnSeriesOneSideData(Direction direction, PotentialPawnSeriesData& potentialPawnSeriesData) {
    const int K = fileAppConfigContainer.PawnsLineLenghtToWin;

    for (int i = 1; i < K; ++i) {

        int iOriented = i;
        if (direction == Direction::LEFT) {
            iOriented *= -1;
        }

        const int currentFieldX = static_cast<int>(currentPlayerMovement.x) + iOriented;
        const int currentFIeldY = static_cast<int>(currentPlayerMovement.y);

        if (currentFieldX < 0 || currentFIeldY < 0) {
            continue;
        }
        const std::size_t currentFieldXSizeT = static_cast<std::size_t>(currentFieldX);
        const std::size_t currentFieldYSizeT = static_cast<std::size_t>(currentFIeldY);
        if (board->IsFieldOnBoard(currentFieldXSizeT, currentFieldYSizeT) == false) {
            continue;
        }

        const Field field = board->at(currentFieldXSizeT, currentFieldYSizeT);

        if (field == playerColor) {
            potentialPawnSeriesData.currentPlayerPawnsInSeriesCount++;
        } else if (field == opponentPlayerColor) {
            break;
        } else { // Empty field.
            if (direction == Direction::LEFT) {
                potentialPawnSeriesData.emptyFieldOnLeftSide++;
            } else {
                potentialPawnSeriesData.emptyFieldOnRightSide++;
            }
        }
    }
}

}
