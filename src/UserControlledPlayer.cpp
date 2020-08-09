#include "UserControlledPlayer.hpp"

UserControlledPlayer::UserControlledPlayer(const Board* gameBoard, PlayerType playerType, PawnColor playerColor) :
    Player(gameBoard, playerType, playerColor)
{
}

void UserControlledPlayer::NotifyAboutOpponentMove(Coordinates)
{
}

Coordinates UserControlledPlayer::MakeMove()
{
    m_NextMove = std::nullopt;
    m_IsWaitingOnMoveRequest = true;

    while (not m_NextMove.has_value())
    {
    }

    m_IsWaitingOnMoveRequest = false;

    return m_NextMove.value();
}

void UserControlledPlayer::NotifyAboutRequestedMove(Coordinates requestedMove)
{
    if (not m_IsWaitingOnMoveRequest)
    {
        return;
    }

    m_NextMove = requestedMove;
}
