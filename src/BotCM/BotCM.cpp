#include "BotCM.hpp"

#include <stdexcept>

#include "../Board.hpp"
#include "../Random.hpp"
#include "../DebugInfo.hpp"
#include "AllocationCounter.hpp"
#include "../AppConfig/FileAppConfigContainer.hpp"
#include "MovementGrade.h"
#include "MovementCoordinatesWithGrade.h"
#include "CommonUtils.hpp"

namespace CM {

BotCM::BotCM(const Board* const gameBoard, const PlayerType playerType, const PawnColor playerColor):
     Player(gameBoard, playerType, playerColor),
     opponentDidAtLeastOneMovement(false),
     currentTurnId(0u),
     opponentPlayerColor(Field::Empty) {
}

BotCM::~BotCM() {
    (void)verifyAllocationCounter();
}

void BotCM::NotifyAboutOpponentMove(const Coordinates opponentMove) {
    if (!emptyFieldsManager.IsFieldsCollectionInitialized()) {
        emptyFieldsManager.InitializeCollection(m_Board->getWidth(), m_Board->getHeight());
    }

    if (!opponentDidAtLeastOneMovement) {
        opponentDidAtLeastOneMovement = true;
    }
    opponentLastMove = opponentMove;
    emptyFieldsManager.SetFieldNotEmpty(opponentLastMove);

    offensiveManager.RemoveFromPotentialCoordinates(opponentMove);
}

Coordinates BotCM::MakeMove() {
    Coordinates coordinates;
    const bool result = makeMoveMain(coordinates);
    if (!result) {
        throw std::runtime_error("");
        return coordinates;
    }
    currentTurnId++;
    return coordinates;
}

bool BotCM::makeMoveMain(Coordinates& outputCoordinates) {
    bool result = false;

    if (LogMovementDeterminingData) {
    	LOG_LN("----------------------------------------");
    }

    // If this is the first turn for current player, do some initialization stuff.
    if (currentTurnId == 0u) {
    	result = doFirstTurnInitializations();
    	if (!result) return false;
    }

    if (MakeDummyMovementsForTesting) {
    	makeTestingMovements(outputCoordinates);
    } else {
    	result = determineMovementCoordinates(outputCoordinates);
    	if (!result) return false;
    }

    result = doActionsAfterMovementChoosen(outputCoordinates);
    return result;
}

bool BotCM::doFirstTurnInitializations() {
	const bool result = determineOpponentPlayerColor();
	if (!result) {
		return false;
	}

	offensiveManager.Initialize(m_Board, m_PlayerColor, opponentPlayerColor, &emptyFieldsManager);
	dangerousFieldManager.Initialize(m_Board);

	if (!emptyFieldsManager.IsFieldsCollectionInitialized()) {
		emptyFieldsManager.InitializeCollection(m_Board->getWidth(), m_Board->getHeight());
	}
	return true;
}

void BotCM::makeTestingMovements(Coordinates& outputCoordinates) {
	static std::size_t i = 0u;
	//const std::size_t X = 6u;

	switch (i) {
	/*
	case 0u:
		outputCoordinates = Coordinates(5,6); break;
	case 1u:
		outputCoordinates = Coordinates(6,6); break;
	case 2u:
		outputCoordinates = Coordinates(7,6); break;
	default:
		outputCoordinates = Coordinates(0,0); break;
		break;
	}
	*/
	/*
	case 0u:
		outputCoordinates = Coordinates(5,X); break;
	case 1u:
		outputCoordinates = Coordinates(6,X); break;
	case 2u:
		outputCoordinates = Coordinates(7,X); break;
	case 3u:
		outputCoordinates = Coordinates(8,X); break;
	case 4u:
		outputCoordinates = Coordinates(9,X); break;
	default:
		outputCoordinates = Coordinates(0,0); break;
		break;
	}
	*/
	/*
	case 0u:
		outputCoordinates = Coordinates(X,6); break;
	case 1u:
		outputCoordinates = Coordinates(X,5); break;
	case 2u:
		outputCoordinates = Coordinates(X,4); break;
	case 3u:
		outputCoordinates = Coordinates(X,3); break;
	case 4u:
		outputCoordinates = Coordinates(X,2); break;
	default:
		outputCoordinates = Coordinates(0,0); break;
		break;
	}
	*/
	case 0u:
		outputCoordinates = Coordinates(8,8); break;
	default:
		outputCoordinates = Coordinates(0,0); break;
		break;
	}
	i++;
}

bool BotCM::determineMovementCoordinates(Coordinates& outputCoordinates) {
    #if 0 // Code for testing:
    if (currentTurnId == 4u) {
        std::cout << "AAA" << std::endl;
    }
    #endif

	bool result = false;
	if (!opponentDidAtLeastOneMovement) {
		// BotCM begins the battle, so random coordinates will be returned.
		const std::size_t boardWidth  = m_Board->getWidth();
		const std::size_t boardHeight = m_Board->getHeight();

		const std::size_t halfBoardWidth      = boardWidth / 2;
		const std::size_t halfBoardHeight     = boardHeight / 2;
		const std::size_t oneForthBoardWidth  = boardWidth / 4;
		const std::size_t oneForthBoardHeight = boardHeight / 4;

		outputCoordinates.x = static_cast<std::size_t>(Random::RandomizeInt(halfBoardWidth)) + oneForthBoardWidth;
		outputCoordinates.y = static_cast<std::size_t>(Random::RandomizeInt(halfBoardHeight)) + oneForthBoardHeight;
		return true;
	}

	const unsigned int PawnSeriesOrientationCount = 4u;
	MovementCoordinatesWithGrade coordinatesWithGrade[PawnSeriesOrientationCount];
	std::size_t i = 0u;

	result = determineCoordinatesAndGradeInOneOrientationAndIncrementI(coordinatesWithGrade, PawnSeriesOrientation::VERTICAL  , i);
	if (!result) return false;

	result = determineCoordinatesAndGradeInOneOrientationAndIncrementI(coordinatesWithGrade, PawnSeriesOrientation::HORIZONTAL, i);
	if (!result) return false;

	result = determineCoordinatesAndGradeInOneOrientationAndIncrementI(coordinatesWithGrade, PawnSeriesOrientation::DECREASING, i);
	if (!result) return false;

	result = determineCoordinatesAndGradeInOneOrientationAndIncrementI(coordinatesWithGrade, PawnSeriesOrientation::INCREASING, i);
	if (!result) return false;

	MovementCoordinatesWithGrade& defensiveCoordinatesWithBestGrade = coordinatesWithGrade[0];
	bool areAllGradesSame = true;

	for (unsigned int i = 1u; i < PawnSeriesOrientationCount; ++i) {
		MovementCoordinatesWithGrade& currentCoordinatesWithGrade = coordinatesWithGrade[i];

		if (currentCoordinatesWithGrade.GetMovementImportanceGrade() != defensiveCoordinatesWithBestGrade.GetMovementImportanceGrade()) {
			areAllGradesSame = false;
		}
		if (currentCoordinatesWithGrade.GetMovementImportanceGrade() > defensiveCoordinatesWithBestGrade.GetMovementImportanceGrade()) {
			defensiveCoordinatesWithBestGrade = currentCoordinatesWithGrade;
		}
	}
	// If grades for movements for all orientations are the same (it occurs usually
	// by game start, after opponent made his first movement), randomize increasing
	// or decreasing movement.
	if (areAllGradesSame) {
		unsigned int randomized = Random::RandomizeInt(1);
		if (randomized == 0u) {
			defensiveCoordinatesWithBestGrade = coordinatesWithGrade[2];
		} else {
			defensiveCoordinatesWithBestGrade = coordinatesWithGrade[3];
		}
	}

	const MovementGrade::GradeNumberType bestDefensiveGrade = defensiveCoordinatesWithBestGrade.GetMovementImportanceGrade();
	const Coordinates& bestDefensiveCoordinates = defensiveCoordinatesWithBestGrade.GetMovementCoordinates();
	if (LogMovementDeterminingData) {
		LOG_LN("bestDefensiveGrade: ", bestDefensiveGrade);
		LOG_LN("bestDefensiveCoordinates: ", bestDefensiveCoordinates.x, ", ", bestDefensiveCoordinates.y);
	}

	MovementCoordinatesWithGrade offensiveMovementCoordinatesWithGrade;
	result = offensiveManager.DetermineBestOffensiveMovement(offensiveMovementCoordinatesWithGrade);
	if (!result) return result;

	const MovementGrade::GradeNumberType bestOffensiveGrade = offensiveMovementCoordinatesWithGrade.GetMovementImportanceGrade();
	const Coordinates& bestOffensiveCoordinates = offensiveMovementCoordinatesWithGrade.GetMovementCoordinates();
	if (LogMovementDeterminingData) {
		LOG_LN("bestOffensiveGrade: ", bestOffensiveGrade);
		LOG_LN("bestOffensiveCoordinates: ", bestOffensiveCoordinates.x, ", ", bestOffensiveCoordinates.y);
	}

	if (   bestDefensiveGrade == 0u
		&& bestDefensiveGrade == bestOffensiveGrade) {
		// Best defensive grade 0u means that there is no room to place pawn in the nearby of opponent last move.
		// In that case, we have to select another free field to place pawn there.
		const bool result = emptyFieldsManager.RandomizeEmptyField(outputCoordinates);
		if (!result) {
			return false;
		}
	}

	if (bestDefensiveGrade > bestOffensiveGrade) {
        const std::size_t lastTwoMovementsDistance = CalculateCoordinatesDistance(currentPlayerLastMove, opponentLastMove);

        // If BotCM started the battle by a first movement, then opponent player put a pawn on a distant
        // (distance >= 3 fields) coordinates from the first pawn on the board, and now there is the second
        // movement of the BotCM player (currentTurnId == 1u, not 0u):
        if (currentTurnId == 1u && lastTwoMovementsDistance >= 3u) {
            // In that case we decide that we are attacking, not defending.
            outputCoordinates = offensiveMovementCoordinatesWithGrade.GetMovementCoordinates();
        } else {
            outputCoordinates = defensiveCoordinatesWithBestGrade.GetMovementCoordinates();
        }
        if (LogMovementDeterminingData) {
            LOG("Defensive");
        }
	} else {
		outputCoordinates = offensiveMovementCoordinatesWithGrade.GetMovementCoordinates();
		if (LogMovementDeterminingData) {
			LOG("Offensive");
		}
	}
	if (LogMovementDeterminingData) {
		LOG_LN(" movement chosen: ", outputCoordinates.x, ", ", outputCoordinates.y);
	}
	return true;
}

bool BotCM::doActionsAfterMovementChoosen(Coordinates& outputCoordinates) {
	offensiveManager.RemoveFromPotentialCoordinates(outputCoordinates);
	emptyFieldsManager.SetFieldNotEmpty(outputCoordinates);

	// After the movement coordinates are chosen by the current bot player, it should be updated
	// potential pawn series data for offensive purposes for the current bot player.
	const bool result = offensiveManager.UpdatePotentialPawnSeriesAfterCurrentPlayerMovement(outputCoordinates);

	currentPlayerLastMove = outputCoordinates;

	return result;
}

bool BotCM::determineCoordinatesAndGradeInOneOrientationAndIncrementI(MovementCoordinatesWithGrade coordinatesWithGrade[], PawnSeriesOrientation pawnSeriesOrientation, std::size_t& i) {
    const bool result = determineCoordinatesAndGradeInOneOrientation(
        pawnSeriesOrientation,
        coordinatesWithGrade[i]);

    if (!result) {
        return false;
    }
    i++;
    return true;
}

bool BotCM::determineCoordinatesAndGradeInOneOrientation(
        const PawnSeriesOrientation pawnSeriesOrientation,
        MovementCoordinatesWithGrade& outputCoordinatesAndGrade) {

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

    // Vector with all fields which are processed during determining fields as gaps.
    std::vector<Field> fieldsProcessed;
    // Vector with coordinates corresponding to fields in 'fieldsProcessed'.
    std::vector<Coordinates> coordinatesProcessed;
    // Index of opponent last move field in 'fieldsProcessed'.
    std::size_t fieldsProcessedOpponentLastMoveIndex = 0u;

    bool result = determineGaps(pawnSeriesOrientation, gaps, minX, centerX, maxX, fieldsProcessed, coordinatesProcessed, fieldsProcessedOpponentLastMoveIndex);
    if (!result) {
        return false;
    }

    NotGapOneSideData notGapLeftSideData, notGapRightSideData;

    // 2. Filling data for no gap case, for current orientation.
    result = determineNoGapData(fieldsProcessed, fieldsProcessedOpponentLastMoveIndex, notGapLeftSideData, notGapRightSideData, coordinatesProcessed);
    if (!result) {
        return false;
    }

    result = makeMoveDecision(pawnSeriesOrientation, gaps, notGapLeftSideData, notGapRightSideData, outputCoordinatesAndGrade);
    return result;
}

bool BotCM::determineGaps(
        const PawnSeriesOrientation pawnSeriesOrientation,
        GapsCollectionT& gaps,
        const int minX,
        const int centerX,
        const int maxX,
        std::vector<Field>& fieldsProcessed,
        std::vector<Coordinates>& coordinatesProcessed,
        std::size_t& fieldsProcessedOpponentLastMoveIndex) {

    unsigned int opponentSymbolsCount = 0u;
    std::size_t lastOppopentSymbolFoundIndex = 0u;
    bool opponentSymbolAlreadyFound = false;
    bool lastNotEmptySymbolWasCurrentPlayerSymbol = false;

    // Pointer to the current gap, from 'gaps'. If recent analyzed field
    // was not a gap field, 'currentGap' will equal nullptr.
    SingleGapT* currentGap = nullptr;

    // Loop for filling 'gaps' collection with real gaps.
    for (int i = minX; i <= maxX; ++i) {
        Coordinates currentCoordinates;
        bool result = determineCurrentCoordinatesForGapProcessing(pawnSeriesOrientation, i, currentCoordinates);
        if (!result) {
            return false;
        }

        result = m_Board->IsFieldOnBoard(currentCoordinates.x, currentCoordinates.y);
        if (!result) {
            LOG_ERROR("Field (", currentCoordinates.x, ", ", currentCoordinates.y, ") not on board!");
            return false;
        }

        Field currentField = m_Board->at(currentCoordinates.x, currentCoordinates.y);
        fieldsProcessed.push_back(currentField);
        coordinatesProcessed.push_back(currentCoordinates);
        if (currentCoordinates == opponentLastMove) {
            fieldsProcessedOpponentLastMoveIndex = fieldsProcessed.size() - 1u;
        }

        result = determineFieldAsGap(currentField, opponentSymbolAlreadyFound, lastNotEmptySymbolWasCurrentPlayerSymbol, lastOppopentSymbolFoundIndex, opponentSymbolsCount, currentGap, gaps, pawnSeriesOrientation, maxX, i, currentCoordinates);
        if (!result) {
            return false;
        }
    }
    return true;
}

bool BotCM::determineNoGapData(
        std::vector<Field>& fieldsProcessed,
        std::size_t fieldsProcessedOpponentLastMoveIndex,
        NotGapOneSideData& notGapLeftSideData,
        NotGapOneSideData& notGapRightSideData,
        std::vector<Coordinates>& coordinatesProcessed) {

    const int pawnsLineLenghtToWin = fileAppConfigContainer.PawnsLineLenghtToWin;

    for (std::size_t i = 1u; i <= static_cast<std::size_t>(pawnsLineLenghtToWin)-1u; ++i) {
        // Process left side.
        int leftSideCurrentIndex = static_cast<int>(fieldsProcessedOpponentLastMoveIndex) - static_cast<int>(i);
        if (leftSideCurrentIndex >= 0) {
            const Field currentField = fieldsProcessed[leftSideCurrentIndex];
            const Coordinates& currentCoordinates = coordinatesProcessed[leftSideCurrentIndex];
            bool result = processNotGapOrientationOneSide(currentField, notGapLeftSideData, currentCoordinates);
            if (!result) { return false; }
        }

        // Process right side.
        const size_t rightSideCurrentIndexX = fieldsProcessedOpponentLastMoveIndex + i;
        if (rightSideCurrentIndexX < fieldsProcessed.size()) {
            const Field currentField = fieldsProcessed[rightSideCurrentIndexX];
            const Coordinates& currentCoordinates = coordinatesProcessed[rightSideCurrentIndexX];
            bool result = processNotGapOrientationOneSide(currentField, notGapRightSideData, currentCoordinates);
            if (!result) { return false; }
        }
    }
    return true;
}

bool BotCM::processNotGapOrientationOneSide(
        const Field currentField,
        NotGapOneSideData& notGapOneSideData,
        const Coordinates& currentCoordinates) {

    if (currentField == opponentPlayerColor) {
        if (!notGapOneSideData.notOpponentPawnMet) {
            notGapOneSideData.opponentPawnSeriesLength++;
        }

    } else if (currentField == m_PlayerColor) {
        notGapOneSideData.notOpponentPawnMet = true;
        notGapOneSideData.currentPlayerSymbolFound = true;

    } else if (currentField == Field::Empty) {
        notGapOneSideData.notOpponentPawnMet = true;

        if (!notGapOneSideData.firstFreeFieldFound) {
            notGapOneSideData.firstFreeFieldCoordinates = currentCoordinates;
            notGapOneSideData.firstFreeFieldFound = true;
        }

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
    } else if (currentField == m_PlayerColor) {
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
        const GapsCollectionT& gaps,
        NotGapOneSideData& notGapLeftSideData,
        NotGapOneSideData& notGapRightSideData,
        MovementCoordinatesWithGrade& outputCoordinatesAndGrade) {

    int opponentPawnSeriesWithHisLastMoveLength
        = notGapLeftSideData.opponentPawnSeriesLength
        + notGapRightSideData.opponentPawnSeriesLength
        + 1;

    bool addDangerousFields = false;

    const std::size_t gapsCount = gaps.size();
    if (gapsCount == 0u) {

        // If there is free place only at left side of opponent pawn series, put pawn there.
        if (    notGapLeftSideData.emptyFieldsCountAfterOpponentPawnSeries != 0u
            && notGapRightSideData.emptyFieldsCountAfterOpponentPawnSeries == 0u) {
            // A zatem stawiamy nasz symbol na polu jeden na lewo.

            if (!notGapLeftSideData.firstFreeFieldFound) {
                LOG_ERROR("!notGapLeftSideData.firstFreeFieldFound");
            }

            // Adjust movement grade to the length of opponent pawns series.
            MovementGrade::GradeNumberType movementGrade = static_cast<MovementGrade::GradeNumberType>(opponentPawnSeriesWithHisLastMoveLength);
            adjustMovementGradeToOpponentPawnSeriesLenght(movementGrade);

            outputCoordinatesAndGrade.Set(notGapLeftSideData.firstFreeFieldCoordinates, movementGrade * 2u);

        // If there is free place only at right side of opponent pawn series, put pawn there.
        } else if (  notGapRightSideData.emptyFieldsCountAfterOpponentPawnSeries != 0u
                   && notGapLeftSideData.emptyFieldsCountAfterOpponentPawnSeries == 0u) {
            // A zatem stawiamy nasz symbol na polu jeden na prawo.

            if (!notGapRightSideData.firstFreeFieldFound) {
                LOG_ERROR("!notGapLeftSideData.firstFreeFieldFound");
            }

            // Adjust movement grade to the length of opponent pawns series.
            MovementGrade::GradeNumberType movementGrade = static_cast<MovementGrade::GradeNumberType>(opponentPawnSeriesWithHisLastMoveLength);
            adjustMovementGradeToOpponentPawnSeriesLenght(movementGrade);

            outputCoordinatesAndGrade.Set(notGapRightSideData.firstFreeFieldCoordinates, movementGrade * 2u);

        // If opponent pawn series is blocked by current player pawns, from both left and right sides,
        // place pawn at any free field in the nearby, if there is any.
        } else if (    notGapLeftSideData.emptyFieldsCountAfterOpponentPawnSeries == 0u
                   && notGapRightSideData.emptyFieldsCountAfterOpponentPawnSeries == 0u) {

            if (   !notGapLeftSideData.firstFreeFieldFound
                && !notGapRightSideData.firstFreeFieldFound) {
                // In that case, no movement has sense for current orientation.
                // And there are no coordinates in the nearby to pick.
                outputCoordinatesAndGrade.SetZeroGrade();
            } else {
                const unsigned int randomizedValue = Random::RandomizeInt(2u);
                Coordinates outputCoordinates;

                if (randomizedValue == 0u) {
                    if (notGapLeftSideData.firstFreeFieldFound) {
                        outputCoordinates = notGapLeftSideData.firstFreeFieldCoordinates;
                    } else {
                        outputCoordinates = notGapRightSideData.firstFreeFieldCoordinates;
                    }
                } else {
                    if (notGapRightSideData.firstFreeFieldFound) {
                        outputCoordinates = notGapRightSideData.firstFreeFieldCoordinates;
                    } else {
                        outputCoordinates = notGapLeftSideData.firstFreeFieldCoordinates;
                    }
                }
                outputCoordinatesAndGrade.Set(outputCoordinates, 2u);
            }

        // If there is free place at both left and right side of opponent pawn series:
        } else {
            Coordinates outputCoordinates;

            if (   notGapLeftSideData.emptyFieldsCountAfterOpponentPawnSeries
                < notGapRightSideData.emptyFieldsCountAfterOpponentPawnSeries) {
                outputCoordinates = notGapRightSideData.firstFreeFieldCoordinates;

            } else if (  notGapRightSideData.emptyFieldsCountAfterOpponentPawnSeries
                       <  notGapLeftSideData.emptyFieldsCountAfterOpponentPawnSeries) {
                outputCoordinates = notGapLeftSideData.firstFreeFieldCoordinates;

            // If both variables are the same:
            // (This case includes situation when there are no symbols other than opponent
            // last pawn, in current direction).
            } else {
                unsigned int randomizedValue = Random::RandomizeInt(2u);
                if (randomizedValue == 0u) {
                    outputCoordinates = notGapLeftSideData.firstFreeFieldCoordinates;
                } else {
                    outputCoordinates = notGapRightSideData.firstFreeFieldCoordinates;
                }
            }

            // Adjust movement grade to the length of opponent pawns series.
            MovementGrade::GradeNumberType movementGrade = static_cast<MovementGrade::GradeNumberType>(opponentPawnSeriesWithHisLastMoveLength);
            adjustMovementGradeToOpponentPawnSeriesLenght(movementGrade);

            // Because there is a free place at both left and right sides of opponent
            // pawn series, the danger of opponent win is very large, so we are making
            // larger the grade, if possible.
            if (movementGrade < MovementGrade::MovementGradeMaxValue) {
            	movementGrade++;
            }
            movementGrade *= 2u;
            if (movementGrade > MovementGrade::MovementGradeMaxValue) {
            	movementGrade = MovementGrade::MovementGradeMaxValue;
            }

            if (opponentPawnSeriesWithHisLastMoveLength == 3) {
                if (movementGrade < MovementGrade::MovementGradeMaxValue) {
                    movementGrade++;
                }
            }

            outputCoordinatesAndGrade.Set(outputCoordinates, movementGrade);
            addDangerousFields = true;
        }

    } else if (gapsCount == 1u) {
        const SingleGapT& gap = gaps.at(0);
        Coordinates outputCoordinates;

        const bool result = randomizeFieldInGap(gap, outputCoordinates);
        if (!result) {
            return false;
        }

        // If we have situation like this: _oXOo_ where O is the most recent placed
        // opponent pawn, and X is our currently decided movement, we would like to make
        // this movement the most important.
        MovementGrade::GradeNumberType movementImportanceGrade = 6u;
        if (    notGapLeftSideData.emptyFieldsCountAfterOpponentPawnSeries > 0u
        	&& notGapRightSideData.emptyFieldsCountAfterOpponentPawnSeries > 0u
			&& (    notGapLeftSideData.opponentPawnSeriesLength > 0u
			    || notGapRightSideData.opponentPawnSeriesLength > 0u)
			   ) {
        	movementImportanceGrade = 10u;
        }
        outputCoordinatesAndGrade.Set(outputCoordinates, movementImportanceGrade);

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

        Coordinates outputCoordinates;
        const bool result = randomizeFieldInGap(*shortestGap, outputCoordinates);
        if (!result) {
            return false;
        }
        outputCoordinatesAndGrade.Set(outputCoordinates, 8u);
    }

    const MovementGrade::GradeNumberType grade = outputCoordinatesAndGrade.GetMovementImportanceGrade();
    if (grade < 10u) {
        // In the current function, there is first
        std::optional<Coordinates> dangerousField = dangerousFieldManager.GetDangerousField();
        if (dangerousField) {
            outputCoordinatesAndGrade.Set(*dangerousField, 10u);
        }
    }

    if (addDangerousFields) {
        if (opponentPawnSeriesWithHisLastMoveLength == 3u) {
            dangerousFieldManager.AddDangerousFields(
                    notGapLeftSideData.firstFreeFieldCoordinates,
                    notGapRightSideData.firstFreeFieldCoordinates);
        }
    }

    return true;
}

void BotCM::adjustMovementGradeToOpponentPawnSeriesLenght(MovementGrade::GradeNumberType& movementGrade) {
    switch (movementGrade) {
        case 1u:
            movementGrade = 1u;
            break;
        case 2u:
            movementGrade = 2u;
            break;
        case 3u:
            movementGrade = 3u;
            break;
        case 4u:
            movementGrade = 6u; // Yes, because it makes the priority the highest.
            break;
        default:
            movementGrade = MovementGrade::MovementGradeMaxValue;
    }
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
    switch (m_PlayerColor) {
        case PawnColor::White:
            opponentPlayerColor = Field::Black;
            break;
        case PawnColor::Black:
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

EmptyFieldsManager& BotCM::GetEmptyFieldsManager() {
    return emptyFieldsManager;
}

}
