#include "HumanPlayer.hpp"

#include <iostream>

#include "Board.hpp"

HumanPlayer::HumanPlayer(Board* gameBoard): Player(gameBoard) {
}

HumanPlayer::~HumanPlayer() {
}

bool HumanPlayer::MakeMove(std::size_t& x, std::size_t& y) {
	bool isInputDataCorrect = false;
	std::cout << "Please put column and row indices of your movement." << std::endl;
	while (!isInputDataCorrect) {
		std::cin >> x;
		std::cin >> y;

		if (!board->IsFieldOnBoard(x, y)) {
			std::cout << "Coordinates are out of the game board. Please enter valid coordinates." << std::endl;
			continue;
		}
		bool result = false;
		bool isFieldEmpty = board->IsFieldEmpty(x, y, result, false);
		if (!result) {
			return 1;
		}
		if (!isFieldEmpty) {
			std::cout << "There is already a pawn on given coordinates. Please enter valid coordinates." << std::endl;
			continue;
		}
		isInputDataCorrect = true;
	}
	return true;
}
