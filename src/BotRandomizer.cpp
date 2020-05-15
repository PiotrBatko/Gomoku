#include "Board.hpp"
#include "BotRandomizer.hpp"
#include "Random.h"

BotRandomizer::BotRandomizer(const Board* const gameBoard) : Player(gameBoard) {
}

BotRandomizer::~BotRandomizer() {
}

bool BotRandomizer::MakeMove(std::size_t& x, std::size_t& y) {
	bool movementPlaceFound = false;
	while (!movementPlaceFound) {
		x = static_cast<std::size_t>(Random::RandomizeInt(board->getWidth ()));
		y = static_cast<std::size_t>(Random::RandomizeInt(board->getHeight()));

		bool result = false;
		bool isFieldEmpty = board->IsFieldEmpty(x, y, result, false);
		if (!result) {
			return false;
		}
		movementPlaceFound = isFieldEmpty;
	}

	// Some additional bot actions could be placed here.

	// Ensure that field which we want to set as player result movement, is not occupied.
	bool result = false;
	bool isFieldEmpty = board->IsFieldEmpty(x, y, result);
	if (!result || !isFieldEmpty) {
		return false;
	}
	return true;
}
