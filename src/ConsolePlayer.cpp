#include "ConsolePlayer.hpp"

#include <iostream>

#include "Board.hpp"
#include "DebugInfo.h"

ConsolePlayer::ConsolePlayer(const Board* const gameBoard)
	: Player(gameBoard), turnCount(1u) {
}

ConsolePlayer::~ConsolePlayer() {
}

void ConsolePlayer::NotifyAboutOpponentMove(Coordinates) {
}

Coordinates ConsolePlayer::MakeMove() {
	bool isInputDataCorrect = false;
	Coordinates nextMove;

	LOG_LN("Turn ", turnCount++, ".");
	LOG_LN("Please put column and row indices of your movement.");

	while (!isInputDataCorrect) {
		std::cin >> nextMove.x;
		if (checkForNotNumberValue()) {
            continue;
		}
		std::cin >> nextMove.y;
		if (checkForNotNumberValue()) {
            continue;
        }

		if (!board->IsFieldOnBoard(nextMove.x, nextMove.y)) {
			LOG_LN("Coordinates are out of the game board. Please enter not occupied coordinates.");
			continue;
		}
		bool result = false;
		bool isFieldEmpty = board->IsFieldEmpty(nextMove.x, nextMove.y, result, false);
		if (!result) {
			throw std::runtime_error("Unnamed error");
		}
		if (!isFieldEmpty) {
			LOG_LN("There is already a pawn on given coordinates. Please enter valid coordinates.");
			continue;
		}
		isInputDataCorrect = true;
	}
	return nextMove;
}

bool ConsolePlayer::checkForNotNumberValue() {
    if (std::cin.fail()) {
        LOG_LN("Entered value is not a number. Please enter two number values.");
        std::cin.clear();
        std::cin.ignore(10000,'\n');
        return true;
    }
    return false;
}
