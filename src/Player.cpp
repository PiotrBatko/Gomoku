#include "Player.hpp"

Player::Player(const Board* const gameBoard, const PlayerType playerType, const Field playerColor)
    : board(gameBoard), playerType(playerType), playerColor(playerColor) {
}

Player::~Player() {
}

PlayerType Player::GetPlayerType() {
    return playerType;
}
