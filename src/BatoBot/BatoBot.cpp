#include "BatoBot.hpp"

#include <stdexcept>

namespace batobot
{

BatoBot::BatoBot(const Board* const gameBoard, const PlayerType playerType, const Field playerColor) :
    Player(gameBoard, playerType, playerColor)
{

}

void BatoBot::NotifyAboutOpponentMove(Coordinates opponentMove)
{
    (void) opponentMove;
}

Coordinates BatoBot::MakeMove()
{
    throw std::runtime_error("Not implemented");
}

}
