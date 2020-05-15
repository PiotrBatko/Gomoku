#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <utility>

class Board;

class Player {
public:
	Player(const Board* gameBoard);
	virtual ~Player();

	// On input, x and y parameters hold last enemy's move. When there was no
	// movement before current call, zeros are passed to this function.
	// On output, x and y should contain current player's move.
	virtual bool MakeMove(std::size_t& x, std::size_t& y) = 0;

protected:
	const Board* const board;
};

#endif
