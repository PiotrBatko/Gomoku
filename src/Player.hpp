#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <utility>

#include "Coordinates.hpp"
#include "CommonEnums.hpp"

class Board;

class Player {
public:
	Player(const Board* gameBoard, PlayerType playerType);
	virtual ~Player();

	// Player is notified after opponent's move. It is good moment
	// to update some local informations about game progress.
	virtual void NotifyAboutOpponentMove(Coordinates opponentMove) = 0;

	virtual Coordinates MakeMove() = 0;

	PlayerType GetPlayerType();

protected:
	const Board* const board;

	PlayerType playerType;
};

#endif
