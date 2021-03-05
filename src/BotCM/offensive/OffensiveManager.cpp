#include <BotCM/offensive/OffensiveManager.hpp>

#include "../../AppConfig/FileAppConfigContainer.hpp"
#include "../../Board.hpp"

namespace CM {

OffensiveManager::OffensiveManager() {
    board = nullptr;
    playerColor = PawnColor::White;
    opponentPlayerColor = Field::Empty;

    for (int i = 0; i < fileAppConfigContainer.BoardSize; ++i) {
        potentialPawn4LongSeriesList.push_back(std::list<PotentialPawnSeries>());
        potentialPawn3LongSeriesList.push_back(std::list<PotentialPawnSeries>());
        potentialPawn2LongSeriesList.push_back(std::list<PotentialPawnSeries>());
    }
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
    bool result = false;

    result = updatePotentialPawnSeriesInOneOrientation(PawnSeriesOrientation::VERTICAL);   if (!result) return false;
    result = updatePotentialPawnSeriesInOneOrientation(PawnSeriesOrientation::HORIZONTAL); if (!result) return false;
    result = updatePotentialPawnSeriesInOneOrientation(PawnSeriesOrientation::INCREASING); if (!result) return false;
    result = updatePotentialPawnSeriesInOneOrientation(PawnSeriesOrientation::DECREASING); if (!result) return false;
    return true;
}

bool OffensiveManager::updatePotentialPawnSeriesInOneOrientation(PawnSeriesOrientation pawnSeriesOrientation) {
    PotentialPawnSeriesData potentialPawnSeriesData;
    updatePotentialPawnSeriesOneSideData(pawnSeriesOrientation, Monotonicity::DECREASING, potentialPawnSeriesData);
    potentialPawnSeriesData.currentPlayerPawnSeries.push_back(currentPlayerMovement);
    updatePotentialPawnSeriesOneSideData(pawnSeriesOrientation, Monotonicity::INCREASING, potentialPawnSeriesData);

    bool isPotentialPawnSeries = false;

    switch (potentialPawnSeriesData.currentPlayerPawnsInSeriesCount) {
        case 2u:
            if (   potentialPawnSeriesData.emptyFieldOnLeftSide  >= 2u
                && potentialPawnSeriesData.emptyFieldOnRightSide >= 2u) {
                AddPotentialPawnSeriesToList(potentialPawn2LongSeriesList, potentialPawnSeriesData);
            }
            break;
        case 3u:
            if (potentialPawnSeriesData.emptyFieldOnLeftSide + potentialPawnSeriesData.emptyFieldOnRightSide >= 2u) {
                AddPotentialPawnSeriesToList(potentialPawn3LongSeriesList, potentialPawnSeriesData);
            }
            break;
        case 4u:
            if (   potentialPawnSeriesData.emptyFieldOnLeftSide  >= 1u
                || potentialPawnSeriesData.emptyFieldOnRightSide >= 1u) {
                AddPotentialPawnSeriesToList(potentialPawn4LongSeriesList, potentialPawnSeriesData);
            }
            break;
        default:
            break;
    }

    return true;
}

void OffensiveManager::AddPotentialPawnSeriesToList(std::vector<std::list<PotentialPawnSeries>>& potentialPawnSeriesList, PotentialPawnSeriesData& potentialPawnSeriesData) {
    PotentialPawnSeries potentialPawnSeries;
    for (Coordinates& coordinates : potentialPawnSeriesData.currentPlayerPawnSeries) {
        potentialPawnSeries.AddPawn(coordinates);
    }
    std::size_t startingPawnColumn = potentialPawnSeries.GetStartingPawnColumn();

    std::list<PotentialPawnSeries>& potentialPawnSeriesListAtGivenStartingX = potentialPawnSeriesList.at(startingPawnColumn);
    potentialPawnSeriesListAtGivenStartingX.push_back(potentialPawnSeries);
}

void OffensiveManager::updatePotentialPawnSeriesOneSideData(PawnSeriesOrientation pawnSeriesOrientation, Monotonicity direction, PotentialPawnSeriesData& potentialPawnSeriesData) {
    const int K = fileAppConfigContainer.PawnsLineLenghtToWin;

    bool firstEmptyFieldAlreadyFound = false;

    for (int i = 1; i < K; ++i) {

        int iOriented = i;
        if (direction == Monotonicity::DECREASING) {
            iOriented *= -1;
        }

        int currentFieldX;
        int currentFIeldY;

        switch (pawnSeriesOrientation) {
            case PawnSeriesOrientation::VERTICAL:
                currentFieldX = static_cast<int>(currentPlayerMovement.x);
                currentFIeldY = static_cast<int>(currentPlayerMovement.y) + iOriented;
                break;
            case PawnSeriesOrientation::HORIZONTAL:
                currentFieldX = static_cast<int>(currentPlayerMovement.x) + iOriented;
                currentFIeldY = static_cast<int>(currentPlayerMovement.y);
                break;
            case PawnSeriesOrientation::INCREASING:
                currentFieldX = static_cast<int>(currentPlayerMovement.x) + iOriented;
                currentFIeldY = static_cast<int>(currentPlayerMovement.y) - iOriented;
                break;
            case PawnSeriesOrientation::DECREASING:
                currentFieldX = static_cast<int>(currentPlayerMovement.x) + iOriented;
                currentFIeldY = static_cast<int>(currentPlayerMovement.y) + iOriented;
                break;
            default:
                break;
        }

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
            if (firstEmptyFieldAlreadyFound) {
                break;
            }

            potentialPawnSeriesData.currentPlayerPawnsInSeriesCount++;
            std::list<Coordinates>& currentPalyerPawnSeries = potentialPawnSeriesData.currentPlayerPawnSeries;

            if (direction == Monotonicity::DECREASING) {
                currentPalyerPawnSeries.push_front(Coordinates(currentFieldXSizeT, currentFieldYSizeT));
            } else {
                currentPalyerPawnSeries.push_back(Coordinates(currentFieldXSizeT, currentFieldYSizeT));
            }

        } else if (field == opponentPlayerColor) {
            break;
        } else { // Empty field.
            firstEmptyFieldAlreadyFound = true;
            if (direction == Monotonicity::DECREASING) {
                potentialPawnSeriesData.emptyFieldOnLeftSide++;
            } else {
                potentialPawnSeriesData.emptyFieldOnRightSide++;
            }
        }
    }
}

}
