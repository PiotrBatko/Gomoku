#ifndef _BOT_CONSOLE_PLAYER_HPP
#define _BOT_CONSOLE_PLAYER_HPP

#include "Player.hpp"

class Board;

class ConsolePlayer : public Player {
public:
    ConsolePlayer(const Board* gameBoard, PlayerType playerType, PawnColor playerColor);
    virtual ~ConsolePlayer();

    void NotifyAboutOpponentMove(Coordinates opponentMove) override;
    Coordinates MakeMove() override;

private:
    bool checkForNotNumberValue();

    unsigned int turnCount;
};

#endif
