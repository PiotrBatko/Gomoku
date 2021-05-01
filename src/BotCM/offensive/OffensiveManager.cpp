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
    	case 1u:
    		break;
        case 2u:
        // Length 2 means that there is a newly created pawn series, so we need to add it to the potential pawn
        // series list, if there are additional circumstances met.
            if (   potentialPawnSeriesData.emptyFieldsOnLeftSide  >= 2u
                && potentialPawnSeriesData.emptyFieldsOnRightSide >= 2u) {
                AddPotentialPawnSeriesToList(potentialPawn2LongSeriesList, potentialPawnSeriesData);
            }
            break;
        case 3u: {
        	const bool result = updatePotentialPawnSeries(potentialPawnSeriesData, 2u);
			if (!result) return false;
			break;
        }
        case 4u: {
			const bool result = updatePotentialPawnSeries(potentialPawnSeriesData, 3u);
			if (!result) return false;
            break;
        }
        case 5u:
        	// This means that we win!
        	break;
        default:
        	LOG_ERROR("Wrong value of 'potentialPawnSeriesData.currentPlayerPawnsInSeriesCount'.");
            break;
    }

    return true;
}

void OffensiveManager::AddPotentialPawnSeriesToList(std::vector<std::list<PotentialPawnSeries>>& potentialPawnSeriesList, PotentialPawnSeriesData& potentialPawnSeriesData) {
    PotentialPawnSeries potentialPawnSeries;
    if (LogOffensiveData) {
		LOG_LN("BotCM-offense: Added new PPS to list (potentiality check passed):");
	}

    for (Coordinates& coordinates : potentialPawnSeriesData.currentPlayerPawnSeries) {
        potentialPawnSeries.AddPawnOnBack(coordinates);
        if (LogOffensiveData) {
			LOG_LN(coordinates.x, ", ", coordinates.y);
		}
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

bool OffensiveManager::updatePotentialPawnSeries(PotentialPawnSeriesData& potentialPawnSeriesData, std::size_t lenghtOfPawnSeriesToUpdate) {
    // When the function is called, we know that there is currently new 3- or 4-pawns long
    // pawn series which is build on current player pawns. But we don't know yet if
    // we want to mark it as a POTENTIAL-offensive pawn series.

	if (lenghtOfPawnSeriesToUpdate != 2u && lenghtOfPawnSeriesToUpdate != 3u) {
		LOG_ERROR("Wrong value of 'lenghtOfPawnSeriesToUpdate'.");
		return false;
	}

	// First, check if there is enough room in at least one side, to make it a potential
	// series for the current player to win.
	if (lenghtOfPawnSeriesToUpdate == 2u) {
		if (potentialPawnSeriesData.emptyFieldsOnLeftSide + potentialPawnSeriesData.emptyFieldsOnRightSide < 2u) {
			return true;
		}
	} else {
		if (!(   potentialPawnSeriesData.emptyFieldsOnLeftSide  >= 1u
			  || potentialPawnSeriesData.emptyFieldsOnRightSide >= 1u)) {
			return true;
		}
	}

	std::list<Coordinates>& pawnSeries = potentialPawnSeriesData.currentPlayerPawnSeries;
	Coordinates& firstPawnCoordinates = pawnSeries.front();
	Coordinates& lastPawnCoordinates = pawnSeries.back();

	if (firstPawnCoordinates == currentPlayerMovement) {
		// Here we know that newly placed pawn is the first (most left) in pawn series.
		// For example: _NXX_ or _NXXX_ where N is the new pawn while X means old pawn.
		// So, it should be updated old (shorter) pawn series entry, that it is longer now,
		// at the beginning of the series.
		std::size_t currentPlayerMovementX = currentPlayerMovement.x;

		// Plus 1 because there is placed new pawn at the front of currently
		// existing potential pawn series.
		std::size_t potentialPawnSeriesBeginningX = 0u;

		// Plus one, because we are enlarging an existing potential pawn series from front, so
		// origin of old series is starting at one column right.
		potentialPawnSeriesBeginningX = currentPlayerMovementX + 1u;

		const bool status = enlargeExistingPotentialPawnSeries(lenghtOfPawnSeriesToUpdate, pawnSeries, potentialPawnSeriesBeginningX, potentialPawnSeriesData, End::FRONT);
		if (!status) return false;

	} else if (lastPawnCoordinates == currentPlayerMovement) {
		// Here we know that newly placed pawn is the last in the pawn series.
		// For example: _XXN_ or _XXXN_ where N is the new pawn while X means old pawn.
		// So, it should be updated old (shorter) pawn series entry that it is longer now,
		// at the back of the series.
		std::size_t currentPlayerMovementX = currentPlayerMovement.x;

	    std::size_t potentialPawnSeriesBeginningX = 0u;

	    if (lenghtOfPawnSeriesToUpdate == 2u) {
			// 2u because length of the new pawn series equals 3.
			potentialPawnSeriesBeginningX = currentPlayerMovementX - 2u;
	    } else {
	    	// 3u because length of the new pawn series equals 4.
	    	potentialPawnSeriesBeginningX = currentPlayerMovementX - 3u;
	    }

	    const bool status = enlargeExistingPotentialPawnSeries(lenghtOfPawnSeriesToUpdate, pawnSeries, potentialPawnSeriesBeginningX, potentialPawnSeriesData, End::BACK);
	    if (!status) return false;

	} else {

	    // Here we know that the newly placed pawn is in the middle of two other pawns
		// (or pawn serieses) of the same player.
		// For example: _XNX_ or _XXNX_ where N is the new pawn while X means old pawn.
		// Or, there is enlarged 2-long or 3-long pawn series, which while creation, didn't pass
		// potentiality check.
		if (lenghtOfPawnSeriesToUpdate == 2u) {
			if (LogOffensiveData) LOG_LN("BotCM-offense: Trying to add a new 3-long PPS.");
			AddPotentialPawn3LongSeriesToListIfPotential(potentialPawnSeriesData);
		} else {
			if (LogOffensiveData) LOG_LN("BotCM-offense: Trying to add a new 4-long PPS.");
			AddPotentialPawn4LongSeriesToListIfPotential(potentialPawnSeriesData);
		}
	}
	return true;
}

void OffensiveManager::AddPotentialPawn3LongSeriesToListIfPotential(PotentialPawnSeriesData& potentialPawnSeriesData) {
	// Here is the check for "potentiality" of the pawn series.
	if (   potentialPawnSeriesData.emptyFieldsOnLeftSide  >= 1u
		&& potentialPawnSeriesData.emptyFieldsOnRightSide >= 1u) {
		AddPotentialPawnSeriesToList(potentialPawn3LongSeriesList, potentialPawnSeriesData);
	} else {
		if (LogOffensiveData) {
			LOG_LN("But potentiality check not passed.");
		}
	}
}

void OffensiveManager::AddPotentialPawn4LongSeriesToListIfPotential(PotentialPawnSeriesData& potentialPawnSeriesData) {
	// Here is the check for "potentiality" of the pawn series.
	if (   potentialPawnSeriesData.emptyFieldsOnLeftSide  >= 1u
		|| potentialPawnSeriesData.emptyFieldsOnRightSide >= 1u) {
		AddPotentialPawnSeriesToList(potentialPawn4LongSeriesList, potentialPawnSeriesData);
	} else {
		if (LogOffensiveData) {
			LOG_LN("But potentiality check not passed.");
		}
	}
}

bool OffensiveManager::enlargeExistingPotentialPawnSeries(
		std::size_t lenghtOfPawnSeriesToUpdate,
		std::list<Coordinates>& pawnSeries,
		std::size_t potentialPawnSeriesBeginningX,
		PotentialPawnSeriesData& potentialPawnSeriesData,
		End endOfEnlargingPotentialSeries) {

	PotentialPawnSeries* pawnSeriesToEnlarge = nullptr;

	std::list<PotentialPawnSeries>* potentialSeriesAtCurrentPlayerMovementX;
	if (lenghtOfPawnSeriesToUpdate == 2) {
		potentialSeriesAtCurrentPlayerMovementX = &potentialPawn2LongSeriesList.at(potentialPawnSeriesBeginningX);
	} else {
		potentialSeriesAtCurrentPlayerMovementX = &potentialPawn3LongSeriesList.at(potentialPawnSeriesBeginningX);
	}

    // Seek for pawn series to enlarge.
    // For each potential pawn series in list of potential pawn series starting at the same column as the current one.
    //for (PotentialPawnSeries& currentPawnSeriesEntry : *potentialSeriesAtCurrentPlayerMovementX) {
	for (std::list<PotentialPawnSeries>::iterator currentPawnSeriesEntryIt = potentialSeriesAtCurrentPlayerMovementX->begin();
		 currentPawnSeriesEntryIt != potentialSeriesAtCurrentPlayerMovementX->end();
		 ++currentPawnSeriesEntryIt) {

        bool pawnSeriesToEnlargeFound = true;

        std::list<Coordinates>& currentPawnSeries = currentPawnSeriesEntryIt->GetPawnSeries();

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

        #if 0 // FOR TESTING
        for (Coordinates& c : pawnSeries) {
        	std::cout << c.x << " " << c.y << std::endl;
        }
        for (Coordinates& c : currentPawnSeries) {
			std::cout << c.x << " " << c.y << std::endl;
		}
        #endif

        std::list<Coordinates>::iterator it1 = pawnSeries.begin();
        if (endOfEnlargingPotentialSeries == End::FRONT) {
        	it1++;
        }

        std::list<Coordinates>::iterator it2 = currentPawnSeries.begin();
        for(; it1 != pawnSeries.end() && it2 != currentPawnSeries.end(); ++it1, ++it2) {
        	if (lenghtOfPawnSeriesToUpdate == 2) {
				if (i == 2u) {
					break;
				}
			} else {
            	if (i == 3u) {
					break;
				}
            }
            Coordinates& currentCoordinatesA = *it1;
            Coordinates& currentCoordinatesB = *it2;

            #if 0 // FOR TESTING
            std::cout << "currentCoordinatesA.x: " << currentCoordinatesA.x << "currentCoordinatesA.y: " << currentCoordinatesA.y << std::endl;
            std::cout << "currentCoordinatesB.x: " << currentCoordinatesB.x << "currentCoordinatesB.y: " << currentCoordinatesB.y << std::endl;
            #endif

            if (currentCoordinatesA != currentCoordinatesB) {
                pawnSeriesToEnlargeFound = false;
            }

            i++;
        }

        if (pawnSeriesToEnlargeFound) {
        	std::size_t newColumnOfPotentialPawnSeries = potentialPawnSeriesBeginningX;
            if (endOfEnlargingPotentialSeries == End::FRONT) {
            	// When we are enlarging potential pawn series from the front, we have to move origin of
            	// potential pawn series one column left.
            	newColumnOfPotentialPawnSeries -= 1u;
            }

            std::list<PotentialPawnSeries>* potentialPawnLongerSeriesList = nullptr;
            if (lenghtOfPawnSeriesToUpdate == 2) {
            	potentialPawnLongerSeriesList = &potentialPawn3LongSeriesList.at(newColumnOfPotentialPawnSeries);
            } else {
            	potentialPawnLongerSeriesList = &potentialPawn4LongSeriesList.at(newColumnOfPotentialPawnSeries);
            }

            potentialPawnLongerSeriesList->emplace_front(*currentPawnSeriesEntryIt);
            pawnSeriesToEnlarge = &(potentialPawnLongerSeriesList->front());
            potentialSeriesAtCurrentPlayerMovementX->erase(currentPawnSeriesEntryIt);
            break;
        }
    }
    if (TreatNotFoundPotentialPawnSeriesToEnlargeAsError) {
        LOG_ERROR("Error because 'TreatNotFoundPotentialPawnSeriesToEnlargeAsError' set to true.");
        return false;
    }

    if (pawnSeriesToEnlarge) {
		if (LogOffensiveData) {
			if (lenghtOfPawnSeriesToUpdate == 2u) {
				if (endOfEnlargingPotentialSeries == End::FRONT) {
					LOG_LN("BotCM-offense: Enlarging 2-long PPS to 3-long from FRONT.");
				} else {
					LOG_LN("BotCM-offense: Enlarging 2-long PPS to 3-long from BACK.");
				}
			} else {
				LOG_LN("BotCM-offense: Enlarging 3-long PPS to 4-long from FRONT.");
			}
		}
		// So, finally enlarge the pawn series by the current player new pawn.
		if (endOfEnlargingPotentialSeries == End::FRONT) {
			pawnSeriesToEnlarge->AddPawnOnFront(currentPlayerMovement);
		} else {
			pawnSeriesToEnlarge->AddPawnOnBack(currentPlayerMovement);
		}

		if (LogOffensiveData) {
			pawnSeriesToEnlarge->LogPawnSeries();
		}
	} else {
		if (LogOffensiveData) {
			if (lenghtOfPawnSeriesToUpdate == 2u) {
				LOG_LN("BotCM-offense: Trying to add new 3-long PPS because shorter doesn't exist on PPS list.");
			} else {
				LOG_LN("BotCM-offense: Trying to add new 4-long PPS because shorter doesn't exist on PPS list.");
			}
		}
		// There is already not existing 2-pawn/3-pawn long potential pawn series to enlarge,
		// so we are adding a new 3-pawn/4-pawn long potential pawn series.
		if (lenghtOfPawnSeriesToUpdate == 2u) {
			AddPotentialPawn3LongSeriesToListIfPotential(potentialPawnSeriesData);
		} else {
			AddPotentialPawn4LongSeriesToListIfPotential(potentialPawnSeriesData);
		}
	}
    return true;
}

}
