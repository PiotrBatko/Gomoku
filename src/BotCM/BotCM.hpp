#ifndef _BOT_CM_HPP
#define _BOT_CM_HPP

#include <vector>

#include "../Player.hpp"

class Board;

namespace CM {

using SingleGapT = std::vector<std::size_t>;
using GapsCollectionT = std::vector<SingleGapT>;

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
    bool verifyAllocationCounter();
    void determineGaps(GapsCollectionT& gaps, const std::size_t minX, const std::size_t maxX);
    void makeMoveDecision(Coordinates& outputCoordinates, const GapsCollectionT& gaps);
};

}

#endif
