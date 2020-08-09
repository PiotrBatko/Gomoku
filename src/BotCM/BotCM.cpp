#include "BotCM.hpp"

#include <stdexcept>

#include "../Board.hpp"
#include "../Random.hpp"
#include "../DebugInfo.hpp"
#include "AllocationCounter.hpp"

namespace CM {

BotCM::BotCM(const Board* const gameBoard, const PlayerType playerType, const PawnColor playerColor):
     Player(gameBoard, playerType, playerColor) {
}

BotCM::~BotCM() {
    (void)verifyAllocationCounter();
}

void BotCM::NotifyAboutOpponentMove(Coordinates) {
}

Coordinates BotCM::MakeMove() {
    //TODO
	throw std::runtime_error("Not implemented");
}

bool BotCM::verifyAllocationCounter() {
    unsigned long long int allocationCounter = AllocationCounter::GetCounter();
    if (allocationCounter != 0uLL) {
        LOG_LN("Allocation counter is not zero. At least one 'new' has no corresponding 'delete'.");
        return false;
    }
    return true;
}

}
