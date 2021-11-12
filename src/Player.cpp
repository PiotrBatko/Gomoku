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

bool Player::IsPlayerBot() const {
    switch (m_PlayerType) {
        case PlayerType::BOT_RANDOMIZER:
        case PlayerType::BOT_CM:
        case PlayerType::BOT_BATOBOT:
            return true;
        default:
            return false;
    }
}

bool Player::IsPlayerHuman() const {
    switch (m_PlayerType) {
        case PlayerType::HUMAN_CONSOLE:
        case PlayerType::HUMAN:
            return true;
        default:
            return false;
    }
}
