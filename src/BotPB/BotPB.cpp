#include "BotPB.hpp"

#include <stdexcept>

namespace PB {

BotPB::BotPB(const Board* const gameBoard, const PlayerType playerType, const Field playerColor) :
    Player(gameBoard, playerType, playerColor)
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
