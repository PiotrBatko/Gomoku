#ifndef USERCONTROLLEDPLAYER_HPP
#define USERCONTROLLEDPLAYER_HPP

#include <atomic>
#include <optional>

#include "Player.hpp"

class UserControlledPlayer :
    public Player
{
public:

    UserControlledPlayer(const Board* gameBoard,
                         PlayerType playerType,
                         PawnColor playerColor,
                         const std::atomic<bool>& canRun);

    virtual void NotifyAboutOpponentMove(Coordinates opponentMove);
    virtual Coordinates MakeMove();
    virtual void NotifyAboutRequestedMove(Coordinates requestedMove);

private:

    bool m_IsWaitingOnMoveRequest = false;
    std::optional<Coordinates> m_NextMove;
    const std::atomic<bool>& m_CanRun;
};

#endif
