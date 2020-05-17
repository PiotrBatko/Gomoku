#include "GameFinishedChecker.h"

#include "AppConfig/FileAppConfigContainer.h"
#include "Field.hpp"
#include "Board.hpp"

GameFinishedChecker::GameFinishedChecker(const Board& board)
	: board(board) {
	pawnsInLineCounter = 0u;
	lastPlayerMovementX_Int = 0;
	lastPlayerMovementY_Int = 0;
	iFirst = 0;
	iLast = 0;
	lastPlayerMovementX = 0u,
	lastPlayerMovementY = 0u,
	lastPlayerColor = Field::Empty;
	pawnsLineLenghtToWin = static_cast<std::size_t>(fileAppConfigContainer.PawnsLineLenghtToWin);
	K = pawnsLineLenghtToWin - 1u;
	boardSize = static_cast<std::size_t>(fileAppConfigContainer.BoardSize);
}

GameFinishedChecker::~GameFinishedChecker() {
}

bool GameFinishedChecker::CheckIfGameFinished(
		Coordinates lastPlayerMovement,
		const Field lastPlayerColor,
		bool& checkingResult) {

	this->lastPlayerMovementX = lastPlayerMovement.x;
	this->lastPlayerMovementY = lastPlayerMovement.y;
	this->lastPlayerColor = lastPlayerColor;
	pawnsInLineCounter = 0u;

	// 1. Check non-slanting lines.
	processNonSlantingLines(checkingResult);
	if (checkingResult) {
		return true;
	}

	// 2. Check slanting lines.
	processSlantingLines(checkingResult);
	return true;
}

void GameFinishedChecker::processNonSlantingLines(bool& checkingResult) {
	// TODO: checking for columns and checking for rows are extremely similar - extract it
	// to one function called twice.

	// 1. Check column.

	// First and last column to check when checking pawns line in single row.
	std::size_t firstColumn = 0u;
	std::size_t lastColumn = boardSize - 1u;

	if (K < lastPlayerMovementX) {
		firstColumn = lastPlayerMovementX - K;
	}
	if (boardSize - lastPlayerMovementX > K) {
		lastColumn = lastPlayerMovementX + K;
	}

	pawnsInLineCounter = 0u;
	for (std::size_t x = firstColumn; x <= lastColumn; ++x) {
		const Field currentField = board.at(x, lastPlayerMovementY);
		if (currentField == lastPlayerColor) {
			if (++pawnsInLineCounter >= pawnsLineLenghtToWin) {
				checkingResult = true;
				return;
			}
		} else {
			pawnsInLineCounter = 0u;
		}
	}

	// 2. Check row.

	// First and last row to check when checking pawns line in single column.
	std::size_t firstRow = 0u;
	std::size_t lastRow = boardSize - 1u;

	if (K < lastPlayerMovementY) {
		firstRow = lastPlayerMovementY - K;
	}
	if (boardSize - lastPlayerMovementY > K) {
		lastRow = lastPlayerMovementY + K;
	}

	pawnsInLineCounter = 0u;
	for (std::size_t y = firstRow; y <= lastRow; ++y) {
		const Field currentField = board.at(lastPlayerMovementX, y);
		if (currentField == lastPlayerColor) {
			if (++pawnsInLineCounter >= pawnsLineLenghtToWin) {
				checkingResult = true;
				return;
			}
		} else {
			pawnsInLineCounter = 0u;
		}
	}
}

void GameFinishedChecker::processSlantingLines(bool& checkingResult) {
	int K_Int= static_cast<int>(K);
	std::size_t lastPlayerMovementMinValue = std::min(lastPlayerMovementX, lastPlayerMovementY);
	iFirst = -K_Int;
	if (lastPlayerMovementMinValue < pawnsLineLenghtToWin) {
		iFirst = -1 * static_cast<int>(lastPlayerMovementMinValue);
	}

	std::size_t lastPlayerMovementMaxValue = std::max(lastPlayerMovementX, lastPlayerMovementY);
	iLast = K_Int;
	if (boardSize - lastPlayerMovementMaxValue < pawnsLineLenghtToWin) {
		iLast = -1 * static_cast<int>(lastPlayerMovementMaxValue);
	}

	lastPlayerMovementX_Int = static_cast<int>(lastPlayerMovementX);
	lastPlayerMovementY_Int = static_cast<int>(lastPlayerMovementY);

	// Check slanting line from top left to bottom right.
	checkingResult = checkSlantingLine(false);
	if (checkingResult) {
		return;
	}
	// Check slanting line from top right to bottom left.
	checkingResult = checkSlantingLine(true);
}

bool GameFinishedChecker::checkSlantingLine(const bool negateIForColumns) {
	pawnsInLineCounter = 0u;

	for (int i = iFirst; i <= iLast; ++i) {
		int iForColumns = i;
		if (negateIForColumns) {
			iForColumns = -i;
		}

		const std::size_t x = static_cast<std::size_t>(lastPlayerMovementX_Int + iForColumns);
		const std::size_t y = static_cast<std::size_t>(lastPlayerMovementY_Int + i);

		const Field currentField = board.at(x, y);
		if (currentField == lastPlayerColor) {
			if (++pawnsInLineCounter >= pawnsLineLenghtToWin) {
				// Player has won.
				return true;
			}
		} else {
			pawnsInLineCounter = 0u;
		}
	}
	// Player didn't win.
	return false;
}
