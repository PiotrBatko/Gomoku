#ifndef _BOT_CM_HPP
#define _BOT_CM_HPP

#include "../Player.hpp"

class Board;

namespace CM {

class BotCM : public Player {
public:
    BotCM(const Board* gameBoard, PlayerType playerType, Field playerColor);
    virtual ~BotCM();

    void NotifyAboutOpponentMove(Coordinates opponentMove) override;
    Coordinates MakeMove() override;

private:
    bool opponentDidAtLeastOneMovement;
    Coordinates opponentLastMove;

    // Identifier of current turn for current player. Opponent turns are not counted here.
    unsigned int currentTurnId;

    // Color of the opponent player.
    Field opponentPlayerColor;

    bool MakeMoveMain(Coordinates& outputCoordinates);
    bool determineOpponentPlayerColor();
};

}

#endif
