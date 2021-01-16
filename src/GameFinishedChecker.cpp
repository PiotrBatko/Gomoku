#include "GameFinishedChecker.hpp"

#include "AppConfig/FileAppConfigContainer.hpp"
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
    pawnsLineLenghtToWin = static_cast<std::size_t>(fileAppConfigContainer.PawnsLineLenghtToWin);
    K = pawnsLineLenghtToWin - 1u;
    boardSize = static_cast<std::size_t>(fileAppConfigContainer.BoardSize);
}

GameFinishedChecker::~GameFinishedChecker() {
}

bool GameFinishedChecker::CheckIfGameFinished(
        Coordinates lastPlayerMovement,
        const PawnColor lastPlayerColor,
        bool& checkingResult) {

    this->lastPlayerMovementX = lastPlayerMovement.x;
    this->lastPlayerMovementY = lastPlayerMovement.y;
    this->lastPlayerColor = lastPlayerColor;
    pawnsInLineCounter = 0u;
    checkingResult = false;

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
    int K_Int = static_cast<int>(K);

    for (int i = -1 * K_Int; i <= K_Int; ++i) {
        int iForColumns = i;
        int iForRows    = i;
        if (negateIForColumns) {
            iForColumns = -i;
        }

        const std::size_t x = static_cast<std::size_t>(lastPlayerMovementX_Int + iForColumns);
        const std::size_t y = static_cast<std::size_t>(lastPlayerMovementY_Int + iForRows);

        const bool isFieldOnBoard = board.IsFieldOnBoard(x, y);
        if (!isFieldOnBoard) {
            continue;
        }

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
