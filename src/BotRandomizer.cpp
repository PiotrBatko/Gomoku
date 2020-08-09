#include <stdexcept>

#include "Board.hpp"
#include "BotRandomizer.hpp"
#include "Random.hpp"

BotRandomizer::BotRandomizer(const Board* const gameBoard, const PlayerType playerType, const PawnColor playerColor)
    : Player(gameBoard, playerType, playerColor) {
}

BotRandomizer::~BotRandomizer() {
}

void BotRandomizer::NotifyAboutOpponentMove(Coordinates) {
}

Coordinates BotRandomizer::MakeMove() {
    Coordinates nextMove;

	bool movementPlaceFound = false;
	while (!movementPlaceFound) {
		nextMove.x = static_cast<std::size_t>(Random::RandomizeInt(m_Board->getWidth()));
		nextMove.y = static_cast<std::size_t>(Random::RandomizeInt(m_Board->getHeight()));

		bool result = false;
		bool isFieldEmpty = m_Board->IsFieldEmpty(nextMove.x, nextMove.y, result, false);
		if (!result) {
		    throw std::runtime_error("Unnamed error");
		}
		movementPlaceFound = isFieldEmpty;
	}

	// Some additional bot actions could be placed here.

	// Ensure that field which we want to set as player result movement, is not occupied.
	bool result = false;
	bool isFieldEmpty = m_Board->IsFieldEmpty(nextMove.x, nextMove.y, result);
	if (!result || !isFieldEmpty) {
		throw std::runtime_error("Unnamed error");
	}
	return nextMove;
}
