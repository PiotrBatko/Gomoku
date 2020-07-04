#include "Player.hpp"

Player::Player(const Board* const gameBoard, const PlayerType playerType)
    : board(gameBoard), playerType(playerType) {
}

Player::~Player() {
}

PlayerType Player::GetPlayerType() {
    return playerType;
}
