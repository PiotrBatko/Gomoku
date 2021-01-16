#include "Player.hpp"

Player::Player(const Board* const gameBoard, const PlayerType playerType, const PawnColor playerColor) :
    m_Board(gameBoard),
    m_PlayerType(playerType),
    m_PlayerColor(playerColor)
{
}

void Player::NotifyAboutRequestedMove(Coordinates)
{
}

PlayerType Player::GetPlayerType()
{
    return m_PlayerType;
}

PawnColor Player::GetColor() const
{
    return m_PlayerColor;
}
