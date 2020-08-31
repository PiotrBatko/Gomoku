#include "UserControlledPlayer.hpp"

UserControlledPlayer::UserControlledPlayer(const Board* gameBoard,
                                           PlayerType playerType,
                                           PawnColor playerColor,
                                           const std::atomic<bool>& canRun) :
    Player(gameBoard, playerType, playerColor),
    m_CanRun(canRun)
{
}

void UserControlledPlayer::NotifyAboutOpponentMove(Coordinates)
{
}

Coordinates UserControlledPlayer::MakeMove()
{
    m_NextMove = std::nullopt;
    m_IsWaitingOnMoveRequest = true;

    while (m_CanRun)
    {
        if (m_NextMove.has_value())
        {
            m_IsWaitingOnMoveRequest = false;

            return m_NextMove.value();
        }
    }

    return Coordinates();
}

void UserControlledPlayer::NotifyAboutRequestedMove(Coordinates requestedMove)
{
    if (not m_IsWaitingOnMoveRequest)
    {
        return;
    }

    m_NextMove = requestedMove;
}
