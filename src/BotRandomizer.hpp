#ifndef _BOT_RANDOMIZER_H_
#define _BOT_RANDOMIZER_H_

#include "Player.hpp"

class Board;

class BotRandomizer : public Player {
public:
    BotRandomizer(const Board* gameBoard, PlayerType playerType, Field playerColor);
    virtual ~BotRandomizer();

    void NotifyAboutOpponentMove(Coordinates opponentMove) override;
    Coordinates MakeMove() override;
};

#endif
