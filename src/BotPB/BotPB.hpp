#ifndef _BOT_PB_HPP
#define _BOT_PB_HPP

#include "../Player.hpp"

class Board;

namespace PB {

class BotPB : public Player
{
public:

    BotPB(const Board* gameBoard, PlayerType playerType, Field playerColor);

    void NotifyAboutOpponentMove(Coordinates opponentMove) override;
    Coordinates MakeMove() override;
};

}

#endif
