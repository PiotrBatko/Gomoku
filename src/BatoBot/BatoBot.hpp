#ifndef BATOBOT_HPP
#define BATOBOT_HPP

#include "Player.hpp"

class Board;

namespace batobot
{

class BatoBot : public Player
{
public:

    BatoBot(const Board* gameBoard, PlayerType playerType, Field playerColor);

    void NotifyAboutOpponentMove(Coordinates opponentMove) override;
    Coordinates MakeMove() override;
};

}

#endif
