#ifndef _CM_INITIAL_FIELD_CAPTURER_HPP_
#define _CM_INITIAL_FIELD_CAPTURER_HPP_

class Board;
class Player;

namespace CM {

// Class for debug purposes while developing BotCM.
// This class sets player pawns on game board before starting the game.
class InitialFieldCapturer {
public:
    InitialFieldCapturer();
    virtual ~InitialFieldCapturer();

    void Run(Board& board, Player* blackPlayer);
};

}

#endif
