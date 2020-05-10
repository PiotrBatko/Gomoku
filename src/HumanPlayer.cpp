#include "HumanPlayer.hpp"

#include <iostream>

#include "Board.hpp"
#include "DebugInfo.h"

HumanPlayer::HumanPlayer(Board* gameBoard): Player(gameBoard) {
}

HumanPlayer::~HumanPlayer() {
}

bool HumanPlayer::MakeMove(std::size_t& x, std::size_t& y) {
	static unsigned int turnCount = 1u;
	bool isInputDataCorrect = false;

	LOG_LN("Turn ", turnCount++, ".");
	LOG_LN("Please put column and row indices of your movement.");
	while (!isInputDataCorrect) {
		std::cin >> x;
		std::cin >> y;

		if (!board->IsFieldOnBoard(x, y)) {
			LOG_LN("Coordinates are out of the game board. Please enter not occupied coordinates.");
			continue;
		}
		bool result = false;
		bool isFieldEmpty = board->IsFieldEmpty(x, y, result, false);
		if (!result) {
			return 1;
		}
		if (!isFieldEmpty) {
			LOG_LN("There is already a pawn on given coordinates. Please enter valid coordinates.");
			continue;
		}
		isInputDataCorrect = true;
	}
	return true;
}
