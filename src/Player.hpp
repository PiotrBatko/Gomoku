#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <utility>

class Board;

class Player {
public:
	Player(Board* gameBoard);
	virtual ~Player();

	virtual bool MakeMove(std::size_t& x, std::size_t& y) = 0;

protected:
	Board* board;
};

#endif
