#include "BotCM.hpp"

#include <stdexcept>

namespace CM {

BotCM::BotCM(const Board* const gameBoard): Player(gameBoard) {
}

BotCM::~BotCM() {
}

void BotCM::NotifyAboutOpponentMove(Coordinates) {
}

Coordinates BotCM::MakeMove() {
	//TODO
	throw std::runtime_error("Not implemented");
}

}
