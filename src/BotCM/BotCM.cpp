#include "BotCM.hpp"

#include <stdexcept>

#include "../Board.hpp"
#include "../Random.hpp"

namespace CM {

BotCM::BotCM(const Board* const gameBoard, const PlayerType playerType):
     Player(gameBoard, playerType) {
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
