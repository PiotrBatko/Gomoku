#ifndef _CM_INITIAL_FIELD_CAPTURER_HPP_
#define _CM_INITIAL_FIELD_CAPTURER_HPP_

#include <vector>

#include "BotCM.hpp"

class Board;
class Player;
class GameView;

namespace CM {

// Class for debug purposes while developing BotCM.
// This class sets player pawns on game board before starting the game.
class InitialFieldCapturer {
public:
    InitialFieldCapturer();
    virtual ~InitialFieldCapturer();

    bool Run(Board& board, Player* blackPlayer, std::vector<GameView*>& views);

private:
    bool SetFieldOnBoardAndNotifyView(const std::size_t x, const std::size_t y, Field field);

    std::vector<GameView*> views;
    Board* board;
    CM::EmptyFieldsManager* emptyFieldsManager;
};

}

#endif
