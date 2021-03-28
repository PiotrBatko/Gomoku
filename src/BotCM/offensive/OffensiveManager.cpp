#include <BotCM/offensive/OffensiveManager.hpp>

#include "../../AppConfig/FileAppConfigContainer.hpp"
#include "../../Board.hpp"
#include "../../DebugInfo.hpp"

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

    // Update potential pawn series for all of four line orientations.
    result = updatePotentialPawnSeriesInOneOrientation(PawnSeriesOrientation::VERTICAL);   if (!result) return false;
    result = updatePotentialPawnSeriesInOneOrientation(PawnSeriesOrientation::HORIZONTAL); if (!result) return false;
    result = updatePotentialPawnSeriesInOneOrientation(PawnSeriesOrientation::INCREASING); if (!result) return false;
    result = updatePotentialPawnSeriesInOneOrientation(PawnSeriesOrientation::DECREASING); if (!result) return false;
    return true;
}

bool OffensiveManager::updatePotentialPawnSeriesInOneOrientation(PawnSeriesOrientation pawnSeriesOrientation) {
	// 1. Collect data about both sides of current orientation.
    PotentialPawnSeriesData potentialPawnSeriesData;
    // First (lower) side.
    updatePotentialPawnSeriesOneSideData(pawnSeriesOrientation, Monotonicity::DECREASING, potentialPawnSeriesData);

    // We are here between both sides, so current player movement is added to the potential pawn series data.
    potentialPawnSeriesData.currentPlayerPawnSeries.push_back(currentPlayerMovement);

    // Second (higher) side.
    updatePotentialPawnSeriesOneSideData(pawnSeriesOrientation, Monotonicity::INCREASING, potentialPawnSeriesData);

    // 2. Do the update of potential pawn series.
    // The value in the switch argument means the full length of current player pawn series in given orientation
    // after applying pawn in current player movement.
    // We are adding/updating a potential pawn series only if there is enough room in at least one side to make
    // it a potential series for the current player to win.
    switch (potentialPawnSeriesData.currentPlayerPawnsInSeriesCount) {
        case 2u:
        // Length 2 means that there is a newly created pawn series, so we need to add it to the potential pawn
        // series list, if there are additional circumstances met.
            if (   potentialPawnSeriesData.emptyFieldsOnLeftSide  >= 2u
                && potentialPawnSeriesData.emptyFieldsOnRightSide >= 2u) {
                AddPotentialPawnSeriesToList(potentialPawn2LongSeriesList, potentialPawnSeriesData);
            }
            break;
        case 3u: {
			const bool result = updatePotentialLength3PawnSeries(potentialPawnSeriesData);
			if (!result) return false;
            break;
        }
        case 4u:
            if (   potentialPawnSeriesData.emptyFieldsOnLeftSide  >= 1u
                || potentialPawnSeriesData.emptyFieldsOnRightSide >= 1u) {
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

    // For each field in one side of the orientation:
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
        	// If current field is not on the board, we should not process it.
            continue;
        }
        const std::size_t currentFieldXSizeT = static_cast<std::size_t>(currentFieldX);
        const std::size_t currentFieldYSizeT = static_cast<std::size_t>(currentFIeldY);
        if (board->IsFieldOnBoard(currentFieldXSizeT, currentFieldYSizeT) == false) {
        	// If current field is not on the board, we should not process it.
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
                potentialPawnSeriesData.emptyFieldsOnLeftSide++;
            } else {
                potentialPawnSeriesData.emptyFieldsOnRightSide++;
            }
        }
    }
}

bool OffensiveManager::updatePotentialLength3PawnSeries(PotentialPawnSeriesData& potentialPawnSeriesData) {
	// First, check if there is enough room in at least one side, to make it a potential
	// series for the current player to win.
	if (potentialPawnSeriesData.emptyFieldsOnLeftSide + potentialPawnSeriesData.emptyFieldsOnRightSide < 2u) {
		return true;
	}

	std::list<Coordinates>& pawnSeries = potentialPawnSeriesData.currentPlayerPawnSeries;
	Coordinates& firstPawn = pawnSeries.front();
	Coordinates& lastPawn = pawnSeries.back();

	if (firstPawn == currentPlayerMovement) {
		// Here we know that newly placed pawn is the first (most left) in pawn series.
		// For example: _NXX_ where N is the new pawn while X means old pawn.
		// So, it should be updated old (shorter) pawn series entry, that it is longer now,
		// at the beginning of the series.
		DebugInfo::LogLn("AAA");

		//TODO: to be implemented.

	} else if (lastPawn == currentPlayerMovement) {
		// Here we know that newly placed pawn is the last in the pawn series.
		// For example: _XXN_ where N is the new pawn while X means old pawn.
		// So, it should be updated old (shorter) pawn series entry that it is longer now,
		// at the back of the series.
		std::size_t currentPlayerMovementX = currentPlayerMovement.x;
		std::size_t potentialPawnSeriesBeginningX = currentPlayerMovementX - 2u; // 2u because length of the new pawn series equals 3.
		std::list<PotentialPawnSeries>& potential2LongSeriesAtCurrentPlayerMovementX = potentialPawn2LongSeriesList.at(potentialPawnSeriesBeginningX);

		PotentialPawnSeries* pawnSeriesToEnlarge = nullptr;

		// Seek for pawn series to enlarge.
		// For each potential pawn series in list of potential pawn series starting at the same column as the current one.
		for (PotentialPawnSeries& currentPawnSeriesEntry : potential2LongSeriesAtCurrentPlayerMovementX) {
			bool pawnSeriesToEnlargeFound = true;

			std::vector<Coordinates>& currentPawnSeries = currentPawnSeriesEntry.GetPawnSeries();

			#if 0 // FOR TESTING
			std::cout << potentialPawnSeriesData.currentPlayerPawnSeries.size() << std::endl;
			std::cout << currentPawnSeriesVector.size() << std::endl;
			#endif

			std::size_t currentPlayerPawnSeriesSize = pawnSeries.size();
			std::size_t currentPawnSeriesSize = currentPawnSeries.size();

			// Perform sanity check.
			if (currentPlayerPawnSeriesSize != currentPawnSeriesSize + 1u) {
				LOG_ERROR("currentPlayerPawnSeriesSize != currentPawnSeriesSize + 1u");
				return false;
			}

			std::size_t i = 0u;
			for (Coordinates& currentCoordinatesA : pawnSeries) {
				if (i == 2u) {
					break;
				}
				Coordinates& currentCoordinatesB = currentPawnSeries.at(i++);

				#if 0 // FOR TESTING
				//std::cout << "c.x: " << c.x << "c.y: " << c.y << std::endl;
				//std::cout << "j.x: " << j.x << "j.y: " << j.y << std::endl;
				#endif

				if (currentCoordinatesA != currentCoordinatesB) {
					pawnSeriesToEnlargeFound = false;
				}
			}
			if (pawnSeriesToEnlargeFound) {
				pawnSeriesToEnlarge = &currentPawnSeriesEntry;
			}
		}
		if (!pawnSeriesToEnlarge) {
			LOG_ERROR("!found");
			return false;
		}

		// So, finally enlarge the pawn series by the current player new pawn.
		pawnSeriesToEnlarge->AddPawn(currentPlayerMovement);

	} else {
		AddPotentialPawnSeriesToList(potentialPawn3LongSeriesList, potentialPawnSeriesData);
	}
	return true;
}

}
