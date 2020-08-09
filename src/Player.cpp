#include "Player.hpp"

Player::Player(const Board* const gameBoard, const PlayerType playerType, const Field playerColor) :
    m_Board(gameBoard),
    m_PlayerType(playerType),
    m_PlayerColor(playerColor)
{
}

PlayerType Player::GetPlayerType()
{
    return m_PlayerType;
}
