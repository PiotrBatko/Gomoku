#include "BotCM.hpp"

#include <stdexcept>

#include "../Random.h"
#include "../Board.hpp"

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
