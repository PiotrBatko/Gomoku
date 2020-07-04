#include "BotPB.hpp"

#include <stdexcept>

namespace PB {

BotPB::BotPB(const Board* const gameBoard, const PlayerType playerType) :
    Player(gameBoard, playerType)
{

}

void BotPB::NotifyAboutOpponentMove(Coordinates opponentMove)
{
    (void) opponentMove;
}

Coordinates BotPB::MakeMove()
{
    throw std::runtime_error("Not implemented");
}

}
